#include "TestCase.h"

#include <sstream>

using namespace gmsec::api5;


/*
o Create a message with no configuration specified.
o Create a message with configuration requesting TREE map storage with size limit of 25.
o Create a message with configuration requesting HASH map storage.
o Use message with no configuration specified, then set configuration to use HASH map storage.
o Attempt to create a message using illegal store-type configuration value.
o Attempt to create a message using illegal store-size configuration value.
*/

void test_StorageTypes(Test& test)
{
	try
	{
		const std::string subject = test.getSubject();
		Config msgConfig;

		//o Create a message with no configuration specified.
		GMSEC_INFO << "Create a message using no preset configuration and with subject " << subject.c_str();
		Message msg1;
		msg1.setSubject(subject.c_str());
		test.check("Passed", true);


		//o Create a message with configuration requesting TREE map storage with size limit of 25.
		GMSEC_INFO << "Create a message using preset config requesting TREE storage with size limit of 25, and with subject "
		           << subject.c_str();
		msgConfig.addValue("gmsec-msgfld-store-type", "tree");
		msgConfig.addValue("gmsec-msgfld-store-size", "25");
		Message msg2;
		msg2.setSubject(subject.c_str());
		msg2.setConfig(msgConfig);
		test.check("Passed", true);


		//o Create a message with configuration requesting HASH map storage.
		GMSEC_INFO << "Create a message using preset config requesting HASH storage, and with subject " << subject.c_str();
		msgConfig.addValue("gmsec-msgfld-store-type", "hash");
		Message msg3;
		msg3.setSubject(subject.c_str());
		msg3.setConfig(msgConfig);
		test.check("Passed", true);


		//o Use message with no configuration specified, then set configuration to use HASH map storage.
		GMSEC_INFO << "Use initial message with no preset configuration, and with subject "
		           << subject.c_str() << "; set config to use HASH storage.";
		Message msg4;
		msg4.setSubject(subject.c_str());
		msg4.setConfig(msgConfig);
		test.check("Passed", true);


		//o Attempt to create a message using illegal store-type configuration value.
		GMSEC_INFO << "Attempt to create a message using illegal configuration option for store type";
		msgConfig.addValue("gmsec-msgfld-store-type", "foo");
		try
		{
			Message msg5;
			msg5.setSubject(subject.c_str());
			msg5.setConfig(msgConfig);
			test.check("Message with illegal setting for gmsec-msgfld-store-type", false);
		}
		catch (...)
		{
			test.check("Illegal value not allowed", true);
		}


		//o Attempt to create a message using illegal store-type configuration value.
		GMSEC_INFO << "Attempt to create a message using illegal configuration option for store size";
		msgConfig.addValue("gmsec-msgfld-store-type", "tree");
		msgConfig.addValue("gmsec-msgfld-store-size", "10abc");
		try
		{
			Message msg6;
			msg6.setSubject(subject.c_str());
			msg6.setConfig(msgConfig);
			test.check("Message with illegal setting for gmsec-msgfld-store-size", false);
		}
		catch (...)
		{
			test.check("Illegal value not allowed", true);
		}


		//o Triggering hashmap
		msgConfig.addValue("gmsec-msgfld-store-type", "tree");
		msgConfig.addValue("gmsec-msgfld-store-size", "2");
		Message msg7;
		msg7.setSubject(subject.c_str());
		msg7.setConfig(msgConfig);
		test.check("Passed", true);

		msg7.addField("FIELD-1", "fee");
		msg7.addField("FIELD-2", "fi");
		msg7.addField("FIELD-3", "fo");     // MsgFieldMap should rollover to using a HashMap.
		msg7.addField("FIELD-4", "fum");
		msg7.addField("FIELD-4", "what?");  // replace value for FIELD-4
		(void) msg7.toXML();
		test.check("Passed", true);

		Message msg8(msg7);

		msg8.getField("FIELD-4");
		msg8.clearField("FIELD-1");
		msg8.getField("FIELD-1");
		msg8.clearFields();
		test.check("Passed", true);

		// Using while-loop, call hasNext() and next() on msg7.  When loop is done, call next().
		MessageFieldIterator& iter = msg7.getFieldIterator();
		while (iter.hasNext()){
			iter.next();
		}

		try
		{
			(void) iter.next();
			test.check("Should not be allowed to iterate further", false);
		}
		catch (const GmsecException& e)
		{
			test.check("Expected error class of ITERATOR_ERROR", e.getErrorClass() == ITERATOR_ERROR);
			test.check("Expected error code of ITER_INVALID_NEXT", e.getErrorCode() == ITER_INVALID_NEXT);
		}
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


void test_DR5171(Test& test)
{
	Config msgConfig1;
	Config msgConfig2("gmsec-sort-msg-fields=true", DataType::KEY_VALUE_DATA);
	Config msgConfig3("gmsec-sort-msg-fields=false", DataType::KEY_VALUE_DATA);
	Config msgConfig4("gmsec-sort-msg-fields=foobar", DataType::KEY_VALUE_DATA);

	//o Create message
	Message msg;
	msg.setSubject( test.getSubject().c_str() );

	//o Add large number of fields, thus forcing Message to rely on hashmap for field storage
	for (GMSEC_I32 i = 0; i < 60; ++i)
	{
		std::ostringstream oss;
		oss << "FIELD-" << (i < 10 ? "0" : "") << i;

		msg.addField(oss.str().c_str(), i);
	}

	msg.setConfig(msgConfig1);
	GMSEC_VERBOSE << "Default display of fields in Message:\n" << msg.toXML();

	msg.setConfig(msgConfig2);
	GMSEC_VERBOSE << "Sorted display of fields in Message:\n" << msg.toXML();
	test.check("Expected message config to indicate sorted fields", msg.getConfig().getBooleanValue("gmsec-sort-msg-fields") == true);

	msg.setConfig(msgConfig3);
	GMSEC_VERBOSE << "Unsorted display of fields in Message:\n" << msg.toXML();
	test.check("Expected message config to indicate unsorted fields", msg.getConfig().getBooleanValue("gmsec-sort-msg-fields") == false);

	// Off-nominal
	try {
		msg.setConfig(msgConfig4);
		test.check("Expected exception because of bad gmsec-sort-msg-fields value", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), true);
	}
}


int test_MsgOptimization(Test& test)
{
	test.setDescription("Message optimization");

	test_StorageTypes(test);
	test_DR5171(test);

	return 0;
}

TEST_DRIVER(test_MsgOptimization)
