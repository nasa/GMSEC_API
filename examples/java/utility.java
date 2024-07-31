/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file utility.java
 *
 * @brief Defines common utility functions for Java example programs.
 */

import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.LogLevel;

public class utility
{
	public static void initializeLogLevel(Config config)
	{
		LogLevel level = Log.levelFromString( config.getValue("loglevel", "info") );
		Log.setReportingLevel(level);
	}
}
