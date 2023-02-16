/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_from_file.cpp
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
 * See config.xml for a similar example.
 */

#include "../utility.h"

#include <gmsec5_cpp.h>

using namespace gmsec::api5;


int main(int argc, char** argv)
{
	Config config(argc, argv);

	// Initialize log level for output
	initializeLogLevel(config);

	try
	{
		// Load/create different configurations from a file.
		Config config1 = Config::getFromFile( "config.xml", "Bolt" );
		Config config2 = Config::getFromFile( "config.xml", "ActiveMQ" );
		Config config3 = Config::getFromFile( "config.xml", NULL);

		GMSEC_INFO << "Config 1 is:\n" << config1.toXML();
		GMSEC_INFO << "Config 2 is:\n" << config2.toXML();
		GMSEC_INFO << "Config 3 is:\n" << config3.toXML();
	}
	catch (const GmsecException& e)
	{
		GMSEC_ERROR << "GmsecException: " << e.what();
	}
}
