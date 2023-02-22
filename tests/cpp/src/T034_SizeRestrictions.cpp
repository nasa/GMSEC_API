#include "TestCase.h"

#include <gmsec5/internal/Encoder.h>

#include <vector>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


int test_SizeRestrictions(Test& test)
{
    test.setDescription("Size Restrictions");

	/* largest blob of data (1GB) */
	const GMSEC_U32 GMSEC_BIN_LIMIT = 0x40000000;

	/* maximum length of GMSEC_STRING (16MB) */
	const GMSEC_U32 GMSEC_STRING_LIMIT = 0x01000000;

	try
	{
		//o Create message with oversized binary field
		{
			Message msg;
			msg.setSubject( test.getSubject().c_str() );

			std::vector<unsigned char> blob(static_cast<size_t>(GMSEC_BIN_LIMIT) + 1);

			msg.addField("HUGE-BINARY-FIELD", blob.data(), blob.size());

			char*          data = 0;
			GMSEC_U64      dataSize;
			MessageEncoder encoder;

			//o Should not be able to encode message containing oversized binary field
			try
			{
				encoder.encode(msg, dataSize, data);
				test.check("Expected encoder error for oversized binary field", false);
			}
			catch (const GmsecException& e)
			{
				test.check(e.what(), true);
			}
		}

		//o Create message with oversized string field
		{
			Message msg;
			msg.setSubject( test.getSubject().c_str() );

			std::vector<char> str(static_cast<size_t>(GMSEC_STRING_LIMIT) + 1, 'A');

			msg.addField("HUGE-STRING-FIELD", static_cast<const char*>(str.data()));

			char*          data = 0;
			GMSEC_U64      dataSize;
			MessageEncoder encoder;

			//o Should not be able to encode message containing oversized string field
			try
			{
				encoder.encode(msg, dataSize, data);
				test.check("Expected encoder error for oversized string field", false);
			}
			catch (const GmsecException& e)
			{
				test.check(e.what(), true);
			}
		}

		//o Create oversized message (largest allowable is 2GB)
		{
			Message msg;
			msg.setSubject( test.getSubject().c_str() );

			std::vector<unsigned char> blob1(static_cast<size_t>(1));
			std::vector<unsigned char> blob2(static_cast<size_t>(GMSEC_BIN_LIMIT));

			msg.addField("BINARY-FIELD-1", blob1.data(), blob1.size());  // 1 byte
			msg.addField("BINARY-FIELD-2", blob2.data(), blob2.size());  // + 1GB
			msg.addField("BINARY-FIELD-3", blob2.data(), blob2.size());  // + 1GB (note, no harm reusing the same binary data)


			char*          data = 0;
			GMSEC_U64      dataSize;
			MessageEncoder encoder;

			//o Should not be able to encode oversized message
			try
			{
				encoder.encode(msg, dataSize, data);
				test.check("Expected encoder error for oversized message", false);
			}
			catch (const GmsecException& e)
			{
				test.check(e.what(), true);
			}
		}
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
	catch (const std::bad_alloc& e)
	{
		test.require(e.what(), false);
	}

    return 0;
}

TEST_DRIVER(test_SizeRestrictions)
