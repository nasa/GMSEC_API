/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


// Definitions for dynamic loading of OpenDDS.
#include "gmsec_opendds.h"

#include "OpenDDSConnection.h"

#include <gmsec4/util/Log.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;

using namespace gmsec_opendds;


// Note: This is not thread-safe by design. Something external must ensure that multiple threads do not call CreateConnection simultaneously...
GMSEC_OPENDDS_API void createConnection(const Config* config, ConnectionInterface** connIf, Status* status)
{
	*connIf = NULL;

	if (status == NULL)
	{
		GMSEC_ERROR << "Status is NULL";
		return;
	}

	try
	{
		if (config)
		{
			*connIf = new OpenDDSConnection(*config);
		}
		else
		{
			status->set(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "No Config object supplied");
		}
	}
	catch (std::exception& e)
	{
		status->set(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, e.what());
	}
}
