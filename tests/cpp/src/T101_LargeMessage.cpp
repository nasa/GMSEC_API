#include "TestCase.h"

#include <memory>
#include <sstream>
#include <vector>

using namespace gmsec::api5;


int test_LargeMessage(Test& test)
{
	test.setDescription("Large Message");

	std::string subject = test.getSubject();
	size_t      size    = 5 * 1024 * 1024;	// 5MB

	//o make size accomodation for Kafka middleware which does not
	//  recommend the use of large messages
	if (std::string("kafka") == test.getConfig().getValue("mw-id", ""))
	{
		size = 976 * 1024;  // slightly less than 1MB
	}

	//o Create message and add large field
	std::vector<unsigned char> data(size);
	for (size_t i = 0; i < size; ++i)
	{
		data[i] = i % 128;
	}

	Message msg;
	msg.setSubject(subject.c_str());
	msg.addField("LARGE-FIELD", data.data(), data.size());

	std::unique_ptr<Connection> conn;

	try
	{
		//o Create connection
		conn.reset(new Connection(test.getConfig()));

		//o Connect
		conn->connect();

		//o Subscribe
		conn->subscribe(subject.c_str());

		//o Publish large message
		GMSEC_INFO << "Publishing message...";
		conn->publish(msg);

		//o Receive large message
		Message* rcvMsg = conn->receive(10000);
		test.check("Large message was not received", rcvMsg != NULL);

		if (rcvMsg)
		{
			GMSEC_INFO << "Received message...";

			const BinaryField* binField = dynamic_cast<const BinaryField*>(rcvMsg->getField("LARGE-FIELD"));
			test.check("Did not get LARGE-FIELD from received message", binField != NULL);

			//o Verify data
			GMSEC_INFO << "Verifying data...";
			if (binField)
			{
				const unsigned char* bufIn  = binField->getValue();
				const size_t         sizeIn = binField->getLength();

				test.check("Size of data sent does not equal size received", data.size() == sizeIn);
				for (size_t i = 0; i < size; ++i)
				{
					const bool sameData = (data[i] == bufIn[i]);

					if (!sameData)
					{
						GMSEC_INFO << "mismatch at offset " << i << ": " << (int)data[i] << " != " << (int)bufIn[i];
						test.check("wrong value returned", sameData);
						break;
					}
				}
			}

			//o Release received message
			Message::destroy(rcvMsg);
		}

		//o Clean up
		conn->disconnect();
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}

	return 0;
}

TEST_DRIVER(test_LargeMessage)
