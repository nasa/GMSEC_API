/*
 * Copyright 2007-2015 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * All Rights Reserved.
 */

#include "gmsec_zeromq.h"

#include "ZMQConnection.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;

using namespace gmsec_zeromq;


GMSEC_ZEROMQ_API void createConnection(const Config* config, ConnectionInterface** connIf, Status* status)
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
			*connIf = new ZMQConnection(*config);
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
