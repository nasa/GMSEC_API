#include "TestCase.h"

#include <gmsec5/internal/Encoder.h>
#include <gmsec5/internal/StringUtil.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


bool customFieldSelector(const Field& field)
{
	// Allow only non-header fields
	return !field.isHeader();
}


int test_Encoder(Test& test)
{
	test.setDescription("MessageEncoder/MessageDecoder");

	unsigned char blob[] = { 0x00, 0x02, 0x04, 0x08, 0xFF };
	size_t blobLen = sizeof(blob)/sizeof(unsigned char);

	//o Create message with every conceivable field type
	//o Note: We do not set the subject because it is not encoded/decoded
	Message msg;
	msg.addField("BIN-FIELD", blob, blobLen);
	msg.addField("BOOL-FIELD", true);
	msg.addField("CHAR-FIELD", 'c');
	msg.addField("F32-FIELD", static_cast<GMSEC_F32>(1));
	msg.addField("F64-FIELD", static_cast<GMSEC_F64>(2));
	msg.addField("I16-FIELD", static_cast<GMSEC_I16>(3));
	msg.addField("I32-FIELD", static_cast<GMSEC_I32>(4));
	msg.addField("I64-FIELD", static_cast<GMSEC_I64>(5));
	msg.addField("I8-FIELD", static_cast<GMSEC_I8>(6));
	msg.addField(StringField("STR-FIELD", "FOO-BAR", true));
	msg.addField("U16-FIELD", static_cast<GMSEC_U16>(7));
	msg.addField("U32-FIELD", static_cast<GMSEC_U32>(8));
	msg.addField("U64-FIELD", static_cast<GMSEC_U64>(9));
	msg.addField("U8-FIELD", static_cast<GMSEC_U8>(10));

	MessageEncoder encoder;
	MessageDecoder decoder;

	//o Test encoding/decoding using buffer size and the raw buffer
	{
		GMSEC_INFO << "Test encoding/decoding using buffer size and the raw buffer";

		DataBuffer buffer(512);
		GMSEC_U64  bytes;
		char*      raw = reinterpret_cast<char*>(const_cast<unsigned char*>(buffer.get()));

		encoder.encode(msg, bytes, raw);
		test.check("Unexpected byte count", bytes == 295);

		Message decodedMsg;
		decoder.decode(decodedMsg, bytes, raw);
		test.check("Unexpected decoded message", StringUtil::stringEquals(msg.toXML(), decodedMsg.toXML()));
		test.check("Unexpected number of fields", decodedMsg.getFieldCount() == 14);
	}


	//o Test encoding/decoding using DataBuffer
	{
		GMSEC_INFO << "Test encoding/decoding using DataBuffer";

		DataBuffer buffer;

		encoder.encode(msg, buffer);
		test.check("Unexpected byte count", buffer.size() == 295);

		Message decodedMsg;
		decoder.decode(decodedMsg, buffer);
		test.check("Unexpected decoded message", StringUtil::stringEquals(msg.toXML(), decodedMsg.toXML()));
		test.check("Unexpected number of fields", decodedMsg.getFieldCount() == 14);
	}

	//o Test alternate field selector using a Selector
	{
		GMSEC_INFO << "Test alternate field selector using a Selector";

		// Only encode header fields
		encoder.setSelector(MessageFieldIterator::Selector::HEADER_FIELDS);

		DataBuffer buffer;

		encoder.encode(msg, buffer);
		test.check("Unexpected byte count", buffer.size() == 34);

		Message decodedMsg;
		decoder.decode(decodedMsg, buffer);
		test.check("Unexpected decoded message", StringUtil::stringEquals(msg.toXML(), decodedMsg.toXML()) == false);
		test.check("Missing field", decodedMsg.hasField("STR-FIELD"));
		test.check("Unexpected number of fields", decodedMsg.getFieldCount() == 1);
	}

	//o Test alternate field selector using a custom function
	{
		GMSEC_INFO << "Test alternate field selector using a custom function";

		// Only encode non-header fields
		encoder.setSelector(customFieldSelector);

		DataBuffer buffer;

		encoder.encode(msg, buffer);
		test.check("Unexpected byte count", buffer.size() == 267);

		Message decodedMsg;
		decoder.decode(decodedMsg, buffer);
		test.check("Unexpected decoded message", StringUtil::stringEquals(msg.toXML(), decodedMsg.toXML()) == false);
		test.check("Unexpected field", decodedMsg.hasField("STR-FIELD") == false);
		test.check("Unexpected number of fields", decodedMsg.getFieldCount() == 13);
	}

	return 0;
}

TEST_DRIVER(test_Encoder)
