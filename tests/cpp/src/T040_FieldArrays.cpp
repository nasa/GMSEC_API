#include "TestCase.h"

#include <cctype>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


static void runTest(Test& test, unsigned int version, unsigned int schemaLevel)
{
	Config config = test.getConfig();   // make a copy
	
	config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

	std::stringstream converter;
	converter << version;
	config.addValue("GMSEC-SPECIFICATION-VERSION", converter.str().c_str());

	converter.clear();
	converter.str("");
	converter << schemaLevel;
	config.addValue("GMSEC-SCHEMA-LEVEL", converter.str().c_str());

	MessageFactory factory(config);
	Status         status;

	Message msg = factory.createMessage("MSG.DEV");
	msg.setFieldValue("DOMAIN1", "1");
	msg.setFieldValue("DOMAIN2", "2");
	msg.setFieldValue("MISSION-ID", "TEST");
	msg.setFieldValue("CONSTELLATION-ID", "TEST");
	msg.setFieldValue("SAT-ID-PHYSICAL", "TEST");
	msg.setFieldValue("SAT-ID-LOGICAL", "TEST");
	msg.setFieldValue("FACILITY", "TEST");
	msg.setFieldValue("COMPONENT", "TEST");
	msg.setFieldValue("NUM-OF-DEVICES", "0");

	status = msg.isCompliant();

	if (status.hasError())
	{
		test.check("Message with missing required fields in array and array size of 0 should have passed validation", false);
		GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
	}
	else
	{
		test.check("Passed!", true);
	}

	msg.setFieldValue("NUM-OF-DEVICES", "1");
	msg.setFieldValue("DEVICE.1.STATUS", "1");

	status = msg.isCompliant();

	if (status.hasError())
	{
		test.check("Passed!", true);
	}
	else
	{
		test.check("Message with missing required fields should not have passed validation", false);
		GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
	}

	msg.setFieldValue("NAME", "1");

	status = msg.isCompliant();

	if (status.hasError())
	{
		std::string err = status.get();
		test.check("DEVICE.1.NAME not identified as missing", err.find("DEVICE.1.NAME") != std::string::npos);
	}
	else
	{
		test.check("Required field in array with wrong name was validated", false);
		GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
	}

	msg.clearField("NAME");
	msg.setFieldValue("DEVICE.1.NAME", "1");

	status = msg.isCompliant();

	if (status.hasError())
	{
		test.require("Message with all correct required fields should have passed validation", false);
		GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
	}
	else
	{
		test.check("Passed!", true);
	}

	msg.addField("DEVICE.1.PARAM.1.NAME", "1");

	status = msg.isCompliant();

	if (status.hasError())
	{
		std::string err = status.get();
		test.check("DEVICE.1.PARAM.1.NAME error message is missing", err.find("DEVICE.1.PARAM.1.NAME") != std::string::npos);
		test.check("DEVICE.1.PARAM.1.NAME error message is repeated", err.find("DEVICE.1.PARAM.1.NAME") == err.rfind("DEVICE.1.PARAM.1.NAME"));
	}
	else
	{
		test.check("Message with user-defined field was validated??", false);
	}

	msg.clearField("DEVICE.1.PARAM.1.NAME");
	msg.clearField("DEVICE.1.NAME");
	msg.setFieldValue("NUM-OF-DEVICES", "2");

	status = msg.isCompliant();

	if (status.hasError())
	{
		std::string err = status.get();
		test.check("overview error message is missing", err.find("Message Validation Failed.") != std::string::npos);
		test.check("overview error message is repeated", err.find("Message Validation Failed.") == err.rfind("Message Validation Failed."));
	}
	else
	{
		test.check("Message with missing required fields was validated!!", false);
	}
}


int test_FieldArrays(Test& test)
{
	test.setDescription("Field Arrays");

	runTest(test, GMSEC_MSG_SPEC_2019_00, 0);

	return 0;
}

TEST_DRIVER(test_FieldArrays)
