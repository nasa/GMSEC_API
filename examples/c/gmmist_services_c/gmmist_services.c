/*
* Copyright 2007-2016 United States Government as represented by the
* Administrator of The National Aeronautics and Space Administration.
* No copyright is claimed in the United States under Title 17, U.S. Code.
* All Rights Reserved.
*/



/**
* @file gmmist_services.cpp
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

	add_to_config_from_file(&this->config);

	this->connMgr = connectionManagerCreate(this->config, this->status);

	connectionManagerInitialize(this->connMgr, this->status);

	return GMSEC_TRUE;
}

GMSEC_BOOL gm_Run(gm_t* this)
{
	const int TIMEOUT = 60000;

	GMSEC_Field definedFields[4];
	GMSEC_Field logDefinedFields[2];
	GMSEC_Field hbDefinedFields[3];

	GMSEC_Field fieldLogSeverity = i16FieldCreate("SEVERITY", 1, this->status);
	
	GMSEC_Field compStatus = i16FieldCreate("COMPONENT-STATUS", 4, this->status);

	// SetStandardFields()
	GMSEC_INFO("Setting fields which will be applied globally to message from this ConnectionManager");
	GMSEC_INFO("These fields are usually certain values from the GMSEC Header which apply broadly");
	
	definedFields[0] = f32FieldCreate("HEADER-VERSION", 2010.0, this->status);
	definedFields[1] = stringFieldCreate("MISSION-ID", "GMSEC", this->status);
	definedFields[2] = stringFieldCreate("FACILITY", "GMSEC Lab", this->status);
	definedFields[3] = stringFieldCreate("COMPONENT", "gmmist", this->status);

	connectionManagerSetStandardFields(this->connMgr, definedFields, 4, this->status);

	fieldDestroy(&definedFields[0]);
	fieldDestroy(&definedFields[1]);
	fieldDestroy(&definedFields[2]);
	fieldDestroy(&definedFields[3]);
	
	GMSEC_INFO("No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!");
	GMSEC_INFO("Adding general log fields");

	logDefinedFields[0] = stringFieldCreate("SUBCLASS", "TAC", this->status);
	logDefinedFields[1] = stringFieldCreate("OCCURRENCE-TYPE", "NORM", this->status);

	connectionManagerSetLoggingDefaults(this->connMgr, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields, 2, this->status);

	fieldDestroy(&logDefinedFields[0]);
	fieldDestroy(&logDefinedFields[1]);

	GMSEC_INFO("Sending log message regarding imminent start of heartbeat service");
	connectionManagerPublishLog(this->connMgr, "About to start heartbeat service", fieldLogSeverity, this->status);

	GMSEC_INFO("No need to add MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE for a heartbeat, we do that for you!");
	GMSEC_INFO("Adding general heartbeat fields");

	hbDefinedFields[0] = i16FieldCreate("PUB-RATE", 1, this->status);
	hbDefinedFields[1] = i16FieldCreate("COUNTER", 0, this->status);
	hbDefinedFields[2] = stringFieldCreate("MSG-ID", "My heartbeat identifier", this->status);

	GMSEC_INFO("Starting heartbeat service");
	connectionManagerStartHeartbeatService(this->connMgr, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.HB", hbDefinedFields, 3, this->status);
	GMSEC_INFO("Heartbeat service has been started. Publish rate is defined by PUB-RATE, defaulting to 30 seconds if not defined.");
	GMSEC_INFO("Component status will be reported as nominal unless otherwise instructed. The counter will automatically increment.");

	fieldDestroy(&hbDefinedFields[0]);
	fieldDestroy(&hbDefinedFields[1]);
	fieldDestroy(&hbDefinedFields[2]);

	connectionManagerPublishLog(this->connMgr, "Heartbest service has been started!", fieldLogSeverity, this->status);

	GMSEC_INFO("Time to start up the resource service. This will automatically publish resource information");
	connectionManagerStartResourceMessageService(this->connMgr, "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE.RSRC.EXAMPLE2", 1, 1, 1, this->status);

	GMSEC_INFO("Waiting for 60 seconds to allow heartbeats and resources to publish.");
	GMSEC_INFO("The user may wish to use gmsub to observe message traffic.");
	example_millisleep(TIMEOUT);

	GMSEC_INFO("Changing the example component status to RED");
	connectionManagerChangeComponentStatus(this->connMgr, compStatus, this->status);

	GMSEC_INFO("Wait another 60 seconds to observe the change");
	example_millisleep(TIMEOUT);

	connectionManagerPublishLog(this->connMgr, "End of demonstration, cleaning up ConnectionManager", fieldLogSeverity, this->status);
	GMSEC_INFO("End of demonstration, cleaning up ConnectionManager");

	connectionManagerStopHeartbeatService(this->connMgr, this->status);
	GMSEC_INFO("Stopping heartbeat service. This will happen automatically when the ConnectionManager cleans up resources, should the user not perform so manually.");

	connectionManagerStopResourceMessageService(this->connMgr, this->status);

	fieldDestroy(&fieldLogSeverity);
	fieldDestroy(&compStatus);

	return GMSEC_TRUE;
}


void gm_Cleanup(gm_t* this)
{
	//Destroy the Config
	configDestroy(&this->config);

	//Destroy ConnectionManager
	connectionManagerCleanup(this->connMgr, this->status);

	connectionManagerDestroy(&this->connMgr);

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
