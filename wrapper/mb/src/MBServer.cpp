/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "MBService.h"

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Config.h>

#include <signal.h>


#define MB_DEFAULT_PORT 9000


using namespace gmsec::api;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


// This is the main function for the Magic Bus server
int main(int argc, char* argv[])
{
	Config config(argc, argv);

	printf("MBServer v" GMSEC_VERSION_NUMBER " ["
	       __DATE__
	       "]\n");

	const char* loglevel = config.getValue("loglevel");
	const char* logfile  = config.getValue("logfile");

	if (!loglevel)
	{
		config.addValue("loglevel", "info");
	}
	if (!logfile)
	{
		config.addValue("logfile", "stderr");
	}

	StringConverter::instance().setMode(StringConverter::NO_CONVERSION);

	int port = MB_DEFAULT_PORT;

#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#endif

	for (int i = 0; i < argc; ++i)
	{
		if ((StringUtil::stringEquals(argv[i], "-help")) || (StringUtil::stringEquals(argv[i], "/?")))
		{
			printf("\n Usage  : ./MBServer [port=n]\n");
			printf("port : Port number for MBServer to use other than default port of %d.\n\n", MB_DEFAULT_PORT);
			return 0;
		}
	}

	const char* portStr = config.getValue("port");

	if (portStr)
	{
		GMSEC_I32 tmp = 0;

		if (StringUtil::stringParseI32(portStr, tmp))
		{
			if (tmp > 0)
			{
				port = tmp;
			}
			else
			{
				printf("Illegal value for port [%s], using default port 9000.\n", portStr);
			}
		}
		else
		{
			printf("Illegal value for port [%s], using default port 9000.\n", portStr);
		}
	}

	// allocate a new MBService and start it
	printf("Starting server on port %d\n", port);
	MBService server(port);

	server.setExitable(config.getBooleanValue("exitable", false));

	server.run();

	return 0;
}
