#include "TestCase.h"

#include <list>
#include <string>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


struct Spec
{
	std::string            subject;
	std::list<std::string> matching;
	std::list<std::string> nonMatching;
};


bool listContains(const std::list<std::string>& l, const std::string& s)
{
	for (std::list<std::string>::const_iterator i = l.begin(); i != l.end(); ++i)
	{
		if (*i == s) return true;
	}

	return false;
}


void addWithPrefix(std::list<std::string>& out, const std::list<std::string>& in, const std::string& prefix)
{
	for (std::list<std::string>::const_iterator i = in.begin(); i != in.end(); ++i)
	{
		out.push_back(prefix + *i);
	}
}


int processSpec(const Config& config, const Spec& spec, Test& test)
{
	StdUniquePtr<Connection> conn;

	try
	{
		conn.reset( new Connection(test.getConfig()) );

		conn->connect();

		GMSEC_INFO << "Subscribing to: " << spec.subject.c_str();
		conn->subscribe(spec.subject.c_str());

		// prepare a list of all matching and non-matching subjects
		std::list<std::string> send;
		std::string prefix;
		addWithPrefix(send, spec.matching, prefix);
		addWithPrefix(send, spec.nonMatching, prefix);

		// publish a message to each subject
		for (std::list<std::string>::const_iterator i = send.begin(); i != send.end(); ++i)
		{
			std::string subject = *i;

			Message msg;
			msg.setSubject(subject.c_str());

			// Publish Message
			conn->publish(msg);
			GMSEC_INFO << "Published " << subject.c_str();
		}

		// collect messages for several seconds
		std::list<std::string> received;

		double tStart = TimeUtil::getCurrentTime_s();
		double timeout = 10;

		while (TimeUtil::getCurrentTime_s() < tStart + timeout)
		{
			Message* rcvMsg = conn->receive(1000);

			if (!rcvMsg)
				continue;

			const char* tmp = rcvMsg->getSubject();

			GMSEC_INFO << "Received " << tmp;
			received.push_back(tmp);

			// Destroy the received message
			Message::destroy(rcvMsg);
		}

		unsigned int missing = 0;
		unsigned int extra   = 0;

		// verify that all expected subjects were received
		for (std::list<std::string>::const_iterator i = spec.matching.begin(); i != spec.matching.end(); ++i)
		{
			if (!listContains(received, *i))
			{
				++missing;
				GMSEC_WARNING << "did not receive matching " << (*i).c_str();
			}
		}

		// verify that no un-expected subjects were received
		for (std::list<std::string>::const_iterator i = spec.nonMatching.begin(); i != spec.nonMatching.end(); ++i)
		{
			if (listContains(received, *i))
			{
				++extra;
				GMSEC_WARNING << "received non-matching " << (*i).c_str();
			}
		}

		test.check("received unexpected messages", extra == 0);
		test.check("failed to receive expected messages", missing == 0);

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}


class MyCallback : public Callback
{
public:
	MyCallback() : m_receivedMsg(false) {}
	void onMessage(Connection& conn, const Message& msg) { m_receivedMsg = true; }
	bool receivedMessage() const { return m_receivedMsg; }
	void reset() { m_receivedMsg = false; }
private:
	bool m_receivedMsg;
};


int test_SubjectMatch(Test& test)
{
	test.setDescription("Subject Matching");

	// this block of code should be part of the standard framework
	const Config& config = test.getConfig();

	{ // first sub-test (greater-than wildcard)
		Spec spec;

		spec.subject = test.getSubject("ONE.>");

		spec.matching.push_back(test.getSubject("ONE.TWO"));
		spec.matching.push_back(test.getSubject("ONE.TWO.THREE"));
		spec.matching.push_back(test.getSubject("ONE.TWO.XYZ.FIVE"));

		spec.nonMatching.push_back(test.getSubject("TWO.ONE"));
		spec.nonMatching.push_back(test.getSubject("ONE"));
		spec.nonMatching.push_back(test.getSubject("ONEZ.TWO"));

		int code = processSpec(config, spec, test);
		test.check("spec ONE.> failed", code == 0);
	}

	{ // second sub-test (star wildcard at end)
		Spec spec;
		spec.subject = test.getSubject("ONE.TWO.*");

		spec.matching.push_back(test.getSubject("ONE.TWO.THREE"));
		spec.matching.push_back(test.getSubject("ONE.TWO.SEVEN"));
		spec.matching.push_back(test.getSubject("ONE.TWO.TWO"));

		spec.nonMatching.push_back(test.getSubject("ONE.TWO.THREE.FOUR"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWO"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWOTHREE.FOUR"));

		int code = processSpec(config, spec, test);
		test.check("spec ONE.TWO.* failed", code == 0);
	}

	{ // third sub-test (plus wildcard at end)
		Spec spec;
		spec.subject = test.getSubject("ONE.TWO.THREE.+");

		spec.matching.push_back(test.getSubject("ONE.TWO.THREE"));
		spec.matching.push_back(test.getSubject("ONE.TWO.THREE.FOUR"));
		spec.matching.push_back(test.getSubject("ONE.TWO.THREE.FOUR.XYZ"));

		spec.nonMatching.push_back(test.getSubject("ONE.ONE.THREE"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWOTHREE"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWO.FOUR"));

		int code = processSpec(config, spec, test);
		test.check("spec ONE.TWO.THREE.+ failed", code == 0);
	}

	{ // fourth sub-test (star wildcards in multiple locations)
		Spec spec;
		spec.subject = test.getSubject("ONE.*.THREE.*");

		spec.matching.push_back(test.getSubject("ONE.TWO.THREE.FOUR"));

		spec.nonMatching.push_back(test.getSubject("ONE.TWO.THREE"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWO.THREE.FOUR.XYZ"));
		spec.nonMatching.push_back(test.getSubject("ONE.ONE.THREE"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWOTHREE"));
		spec.nonMatching.push_back(test.getSubject("ONE.TWO.FOUR"));

		int code = processSpec(config, spec, test);
		test.check("spec ONE.*.THREE.* failed", code == 0);
	}

	return 0;
}

TEST_DRIVER(test_SubjectMatch)
