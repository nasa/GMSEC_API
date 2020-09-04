/*
* Copyright 2007-2016 United States Government as represented by the
* Administrator of The National Aeronautics and Space Administration.
* No copyright is claimed in the United States under Title 17, U.S. Code.
* All Rights Reserved.
*/



/**
* @file gmmist_validation.cpp
*
* A C example demonstration of GMSEC the ConnectionManager class services
* for logging and heartbeats, as well as general message validation.
*
*/

#include "../example.h"


typedef struct
{
	GMSEC_Config		config;
	GMSEC_ConnectionMgr connMgr;
	GMSEC_Status		status;
	GMSEC_Message		message;
	GMSEC_Field			field;
} gm_t;


void gm_printUsage()
{
	printf("\nusage: gmmist_services_c connectiontype=<middleware> "
	       "[ <parameter>=<value> ]\n"
	       "\n\tNote that the parameter 'connectiontype' is required. "
	       "\n\tThe rest of other parameters are optional.\n"
	       "\n\tserver=<server name> "
	       "(required if middleware is not bolt/MB locally)"
	       "\n\tsubject=<subject name>"
	       "\n\titerations=<iterations>"
	       "\n\tinterval_ms=<interval (milliseconds)>"
	       "\n\tloglevel=<log level>"
	       "\n\tcfgfile=<config_filepath.xml>"
	       "\n\nFor more information, see API User's Guide\n");
}


GMSEC_BOOL gm_Initialize(gm_t* this, int argc, char* argv[])
{
	this->status = statusCreate();
	this->config = configCreateWithArgs(argc, argv);

	//Initialize the configuration
	example_initialize(this->config);

	add_to_config_from_file(&(this->config));

	this->connMgr = connectionManagerCreate(this->config, this->status);
	connectionManagerInitialize(this->connMgr, this->status);

	this->message = NULL;
	this->field = NULL;

	return GMSEC_TRUE;
}

GMSEC_BOOL gm_Run(gm_t* this)
{
	GMSEC_Field definedFields[4];
	GMSEC_Field logDefinedFields[2];

	GMSEC_ProductFile myFile;
	
	GMSEC_Field sampleFields[5];
	GMSEC_MnemonicSample mnemonicSamples[4];
	GMSEC_Mnemonic mnemonic;

	GMSEC_Device device1;
	GMSEC_Device device2;
	GMSEC_DeviceParam param1;
	GMSEC_DeviceParam param2;

	GMSEC_INFO("Setting fields which will be applied globally to message from this ConnectionManager");
	GMSEC_INFO("These fields are usually certain values from the GMSEC Header which apply broadly");

	//setting standard fields
	definedFields[0] = f32FieldCreate("HEADER-VERSION", 2010.0, this->status);
	definedFields[1] = stringFieldCreate("MISSION-ID", "GMSEC", this->status);
	definedFields[2] = stringFieldCreate("FACILITY", "GMSEC Lab", this->status);
	definedFields[3] = stringFieldCreate("COMPONENT", "gmmist-demo", this->status);

	connectionManagerSetStandardFields(this->connMgr, definedFields, 4, this->status);

	fieldDestroy(&definedFields[0]);
	fieldDestroy(&definedFields[1]);
	fieldDestroy(&definedFields[2]);
	fieldDestroy(&definedFields[3]);

	//creating log message
	logDefinedFields[0] = stringFieldCreate("SUBCLASS", "TAC", this->status);
	logDefinedFields[1] = stringFieldCreate("OCCURRENCE-TYPE", "NORM", this->status);

	GMSEC_INFO("No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!");
	GMSEC_INFO("Adding general log fields");

	this->message = connectionManagerCreateLogMessage(this->connMgr, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields, 2, this->status);

	messageAddI16Field(this->message, "SEVERITY", 1, this->status);
	messageAddStringField(this->message, "MSG-TEXT", "Self-published log message", this->status);

	//publishing a valid log message
	GMSEC_INFO("Attempting to manually publish a valid log message...");
	connectionManagerPublish(this->connMgr, this->message, this->status);

	GMSEC_INFO("Demonstrating the fast log functionality...");
	GMSEC_INFO("First we set the defaults for all log messages called with 'publishLog'");
	connectionManagerSetLoggingDefaults(this->connMgr, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields, 2, this->status);
	this->field = i16FieldCreate("SEVERITY", 1, this->status);
	GMSEC_INFO("...Then all we have to do is call 'publishLog' with a log string and a severity");
	connectionManagerPublishLog(this->connMgr, "Self-published log message", this->field, this->status);
	connectionManagerPublishLog(this->connMgr, "Another log, with one function call!", this->field, this->status);

	fieldDestroy(&this->field);
	fieldDestroy(&logDefinedFields[0]);
	fieldDestroy(&logDefinedFields[1]);

	//attempting to publish an invalid log message
	messageAddStringField(this->message,"SEVERITY", "SEVERITY Field in a LOG should be an I16", this->status);

	GMSEC_INFO("Attempting to manually publish an invalid log message...");
	connectionManagerPublish(this->connMgr, this->message, this->status);
	if (statusIsError(this->status))
	{
		GMSEC_INFO("Invalid log message was NOT published; reason: %s", statusGet(this->status));
	}
	else
	{
		GMSEC_WARNING("Ignored improper severity and published anyway");
	}

	messageDestroy(&this->message);

	//creating a product message
	GMSEC_INFO("Creating PROD message...");
	this->message = messageCreate("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", GMSEC_PUBLISH, this->status);

	GMSEC_INFO("Setting fields related to PROD message header...");
	connectionManagerAddStandardFields(this->connMgr, this->message, this->status);
	messageAddStringField(this->message, "MESSAGE-TYPE", "MSG", this->status);
	messageAddStringField(this->message, "MESSAGE-SUBTYPE", "PROD", this->status);
	messageAddF32Field(this->message, "CONTENT-VERSION", (GMSEC_F32) 2013.0, this->status);
	messageAddI16Field(this->message, "RESPONSE-STATUS", 6, this->status);
	messageAddStringField(this->message, "PROD-TYPE", "AUTO", this->status);
	messageAddStringField(this->message, "PROD-SUBTYPE", "DM", this->status);

	//publishing a valid product message
	GMSEC_INFO("Publishing PROD message...");
	connectionManagerPublish(this->connMgr, this->message, this->status);

	//altering the pruduct message to make it invalid
	messageAddF32Field(this->message, "PROD-SUBTYPE", 45, this->status);

	//attempting to publish the invalid product message
	GMSEC_INFO("Attempting to publish invalid PROD message...");
	connectionManagerPublish(this->connMgr, this->message, this->status);
	if (statusIsError(this->status))
	{
		GMSEC_INFO("Invalid product message was NOT published; reason: %s", statusGet(this->status));
	}
	else
	{
		GMSEC_WARNING("Ignored improper severity and published anyway");
	}

	GMSEC_INFO("But wait, publishing a product is even easier!");

	messageDestroy(&this->message);

	this->message = productFileMessageCreate("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM",
		GMSEC_FINAL_MESSAGE, "AUTO", "DM", C_GMSEC_ISD_CURRENT, this->status);

	myFile = productFileCreate("My Product", "My Product Description", "Version", "TXT", 
		"A path to somewhere", this->status);

	productFileMessageAddProductFile(this->message, myFile, this->status);

	productFileDestroy(&myFile);

	connectionManagerAddStandardFields(this->connMgr, this->message, this->status);

	GMSEC_INFO("Attempting to publish PROD message with ProductFileMessage");
	connectionManagerPublish(this->connMgr, this->message, this->status);
	if (statusIsError(this->status))
	{
		GMSEC_INFO("Invalid product message was NOT published; reason: %s", statusGet(this->status));
	}

	GMSEC_INFO("Mnemonic Messages are also easier!");

	sampleFields[0] = i32FieldCreate("Timestamp 1", 15, this->status);
	sampleFields[1] = f32FieldCreate("Timestamp 1", 15.0, this->status);
	sampleFields[2] = i32FieldCreate("Timestamp 2", 16, this->status);
	sampleFields[3] = i32FieldCreate("Timestamp 3", 17, this->status);
	sampleFields[4] = i32FieldCreate("Timestamp 4", 18, this->status);

	mnemonicSamples[0] = mnemonicSampleCreate("Timestamp 1", sampleFields[0], this->status);
	mnemonicSampleSetEUValue(mnemonicSamples[0], sampleFields[1], this->status);
	mnemonicSampleGetFlags(mnemonicSamples[0], this->status);
	mnemonicSampleSetLimit(mnemonicSamples[0], GMSEC_RED_HIGH, this->status);
	//implicitly set limit enable/disable with setting of limit
	mnemonicSampleSetQuality(mnemonicSamples[0], GMSEC_TRUE, this->status);
	mnemonicSampleSetStalenessStatus(mnemonicSamples[0], GMSEC_FALSE, this->status);
	mnemonicSampleSetTextValue(mnemonicSamples[0], "15", this->status);
	mnemonicSamples[1] = mnemonicSampleCreate("Timestamp 2", sampleFields[2], this->status);
	mnemonicSamples[2] = mnemonicSampleCreate("Timestamp 3", sampleFields[3], this->status);
	mnemonicSamples[3] = mnemonicSampleCreate("Timestamp 4", sampleFields[4], this->status);

	fieldDestroy(&sampleFields[0]);
	fieldDestroy(&sampleFields[1]);
	fieldDestroy(&sampleFields[2]);
	fieldDestroy(&sampleFields[3]);
	fieldDestroy(&sampleFields[4]);

	mnemonic = mnemonicCreate("MyMnemonic", mnemonicSamples, 4, this->status);

	mnemonicSampleDestroy(&mnemonicSamples[0]);
	mnemonicSampleDestroy(&mnemonicSamples[1]);
	mnemonicSampleDestroy(&mnemonicSamples[2]);
	mnemonicSampleDestroy(&mnemonicSamples[3]);

	this->field = i16FieldCreate("My status", 5, this->status);

	mnemonicSetStatus(mnemonic, this->field, this->status);
	fieldDestroy(&this->field);

	mnemonicSetUnits(mnemonic, "MW", this->status);

	messageDestroy(&this->message);

	this->message = mnemonicMessageCreate("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.MVAL.EXAMPLE-PROG", C_GMSEC_ISD_CURRENT, this->status);
	mnemonicMessageAddMnemonic(this->message, mnemonic, this->status);
	mnemonicDestroy(&mnemonic);
	connectionManagerAddStandardFields(this->connMgr, this->message, this->status);

	if (C_GMSEC_ISD_CURRENT == C_GMSEC_ISD_2014_00)
	{
		//MSG-ID was required in GMSEC messages before the 2016 specification.
		this->field = stringFieldCreate("MSG-ID", "MYMSG", this->status);
		messageAddField(this->message, this->field, this->status);
		fieldDestroy(&this->field);
	}

	GMSEC_INFO("MnemonicMessage formats my GMSEC data automatically!");
	GMSEC_INFO(messageToXML(this->message, this->status));
	connectionManagerPublish(this->connMgr, this->message, this->status);

	messageDestroy(&this->message);

	GMSEC_INFO("Device Messages have their own subclass, as well");

	device1 = deviceCreate("Device 1", GMSEC_RED, this->status);
	device2 = deviceCreate("Device 2", GMSEC_NOMINAL_GREEN, this->status);

	this->field = i32FieldCreate("Param1Val", 79, this->status);
	param1 = deviceParamCreate("DEV Param 1", "Param 1 Timestamp", this->field, this->status);
	fieldDestroy(&this->field);

	deviceAddParam(device1, param1, this->status);
	deviceParamDestroy(&param1);

	this->field = i32FieldCreate("Param1Val", 79, this->status);
	param1 = deviceParamCreate("DEV Param 1", "Param 1 Timestamp", this->field, this->status);
	fieldDestroy(&this->field);

	this->field = stringFieldCreate("Param2Val", "79", this->status);
	param2 = deviceParamCreate("DEV Param 2", "Param 2 Timestamp", this->field, this->status);
	fieldDestroy(&this->field);

	deviceAddParam(device2, param1, this->status);
	deviceAddParam(device2, param2, this->status);

	deviceParamDestroy(&param1);
	deviceParamDestroy(&param2);

	this->message = deviceMessageCreate("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE-PROG.DEV.EXAMPLE-PROG-2",
		C_GMSEC_ISD_CURRENT, this->status);
	deviceMessageAddDevice(this->message, device1, this->status);
	deviceMessageAddDevice(this->message, device2, this->status);

	deviceDestroy(&device1);
	deviceDestroy(&device2);

	GMSEC_INFO("DeviceMessage formats my GMSEC data automatically!");
	GMSEC_INFO(messageToXML(this->message, this->status));

	connectionManagerAddStandardFields(this->connMgr, this->message, this->status);
	connectionManagerPublish(this->connMgr, this->message, this->status);

	return GMSEC_TRUE;
}


void gm_Cleanup(gm_t* this)
{
	//Destroy the Config
	configDestroy(&this->config);

	//Destroy ConnectionManager
	connectionManagerCleanup(this->connMgr, this->status);

	connectionManagerDestroy(&this->connMgr);

	//Destroy the Message
	messageDestroy(&this->message);

	//Destroy the Status
	statusDestroy(&this->status);

	GMSEC_INFO("Demo completed!");
}


int main(int argc, char* argv[])
{
	gm_t gm;

	if (gm_Initialize(&gm, argc, argv) == GMSEC_FALSE)
	{
		fprintf(stderr, "\nFailure of initializing the example");
		return -1;
	}

	if (example_option_invalid(gm.config, argc) == GMSEC_TRUE)
	{
		gm_printUsage();
		return -1;
	}

	gm_Run(&gm);
	gm_Cleanup(&gm);
 
	return 0;
}
