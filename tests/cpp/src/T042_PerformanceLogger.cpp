#include "TestCase.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


std::vector<std::string> split(std::string str, char delimiter)
{
	std::vector<std::string> ret;

	std::string current = "";
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != delimiter)
			current += str[i];
		else
		{
			ret.push_back(current);
			current = "";
		}
	}

	ret.push_back(current);
	return ret;
}


class PerformanceLogger
{
public:
	static int test_PerformanceLogger(Test& test)
	{
		test.setDescription("Performance Logger");

		PerformanceLogger pl(test);

		pl.run();

		return 0;
	}

protected:
	PerformanceLogger(Test& test);

	void run();

private:
	Test& m_test;
};

TEST_DRIVER(PerformanceLogger::test_PerformanceLogger)



PerformanceLogger::PerformanceLogger(Test& test)
	: m_test(test)
{
}


void PerformanceLogger::run()
{
	Config cfg = m_test.getConfig();
	cfg.addValue("GMSEC-LOG-PERFORMANCE", "unit.log");

	const std::string subject  = m_test.getSubject();
	const int iterations = 5;

	std::unique_ptr<Connection> conn;

	try
	{
		conn.reset(new Connection(cfg));

		conn->connect();

		conn->subscribe(subject.c_str());

		Message msg = conn->getMessageFactory().createMessage("HB");
		msg.setSubject(subject.c_str());

		for (int i = 0; i < iterations; ++i)
		{
			conn->publish(msg);

			Message* rcvMsg = conn->receive(10000);

			if (rcvMsg)
			{
				GMSEC_VERBOSE << "Received Message:\n" << rcvMsg->toXML();

				Message::destroy(rcvMsg);
			}
			else
			{
				m_test.require("Failed to receive message", false);
			}
		}

		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		m_test.require(e.what(), false);
	}

	try
	{
		const char* const logfile = "unit.log";

		std::ifstream infile(logfile, std::ios::in);
		std::string   line;
		int           lineCount = 0;
		
		while (infile && std::getline(infile, line))
		{
			GMSEC_VERBOSE << "Read entry from logfile: " << line.c_str();

			++lineCount;

			// We only care to analyze the last entry
			if (lineCount < iterations)
				continue;

			std::vector<std::string> elements = split(line, ',');
			GMSEC_VERBOSE << "Expecting four elements: " << elements.size();
			m_test.check("element count", elements.size() == 4);

			GMSEC_VERBOSE << "Expecting equal subjects: " << elements.at(0).c_str() << " and " << subject.c_str();
			m_test.check("subject check", elements.at(0).compare(subject) == 0);

			double msCheck = std::strtod(elements.at(1).c_str(), NULL);
			GMSEC_VERBOSE << "Millisecond check, expecting more than 0, less than 1000: " << msCheck;
			m_test.check("millisecond check", msCheck < 1000);
			m_test.check("millisecond check", msCheck >= 0);

			std::string pubTime     = elements.at(2);
			std::string receiveTime = elements.at(3);

			double pubTimeT     = TimeUtil::convertTimeString_s(pubTime.c_str());
			double receiveTimeT = TimeUtil::convertTimeString_s(receiveTime.c_str());
			double delta        = receiveTimeT - pubTimeT;

			GMSEC_INFO << "\n"
				<< "msCheck      = " << msCheck << "\n"
				<< "pubTime      = " << pubTime.c_str() << "\n"
				<< "receiveTime  = " << receiveTime.c_str() << "\n"
				<< "pubTimeT     = " << pubTimeT << "\n"
				<< "receiveTimeT = " << receiveTimeT << "\n"
				<< "delta        = " << delta;

			//Check to make sure delta timestamp is within a millisecond of stated delay (msCheck)
			bool okay = fabs( msCheck - round(delta * 1000) ) <= 1.0;
			m_test.check("timestamp check failed", okay);
		}

		infile.close();

		m_test.check("linecount error", lineCount == iterations);

		if (lineCount != iterations)
		{
			GMSEC_WARNING << "Found " << lineCount << " entries; expecting " << iterations;
			GMSEC_WARNING << "See log file: " << logfile;
		}
		else
		{
			std::remove(logfile);
		}
	}
	catch(const std::exception& e)
	{
		GMSEC_ERROR << "Found exception in testing log file: " << e.what();
		m_test.check("exception thrown", false);
	}
}
