/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_from_file.c
 *
 * @brief This example program provides a basic demonstration of creating
 * a Config object using the XML contents of a file that represent a
 * connection (or message) configuration.
 *
 * A configuration file should have contents using a format similar to:
 *
 * <CONFIG NAME="Bolt">
 *     <PARAMETER NAME="mw-id">bolt</PARAMETER>
 *     <PARAMETER NAME="server">my-server</PARAMETER>
 * </CONFIG>
 *
 * <CONFIG NAME="ActiveMQ">
 *     <PARAMETER NAME="mw-id">activemq39</PARAMETER>
 *     <PARAMETER NAME="server">tcp://my-server:61616</PARAMETER>
 * </CONFIG>
 *
 * See config.xml for a simple example.
 */

#include "../utility.h"

#include <gmsec5_c.h>


int main(int argc, char** argv)
{
	// Initialize log level for output
	initializeLogLevel(NULL);

	// Load/create Config from a file.
	GMSEC_Status status = statusCreate();
	GMSEC_Config config = NULL;

	/* Load Bolt configuration */
	config = configGetFromFile( "config.xml", "Bolt", status );

	if (statusHasError(status) == GMSEC_FALSE)
	{
		GMSEC_INFO("Config 1 is:\n%s", configToXML(config, NULL));

		configDestroy(&config);
	}
	else
	{
		GMSEC_ERROR("Error: %s", statusGet(status));
	}

	/* Load ActiveMQ configuration */
	config = configGetFromFile( "config.xml", "ActiveMQ", status );

	if (statusHasError(status) == GMSEC_FALSE)
	{
		GMSEC_INFO("Config 2 is:\n%s", configToXML(config, NULL));

		configDestroy(&config);
	}
	else
	{
		GMSEC_ERROR("Error: %s", statusGet(status));
	}

	/* Load first configuration that appears in the file */
	config = configGetFromFile( "config.xml", NULL, status );

	if (statusHasError(status) == GMSEC_FALSE)
	{
		GMSEC_INFO("Config 3 is:\n%s", configToXML(config, NULL));

		configDestroy(&config);
	}
	else
	{
		GMSEC_ERROR("Error: %s", statusGet(status));
	}

	statusDestroy(&status);

	return 0;
}
