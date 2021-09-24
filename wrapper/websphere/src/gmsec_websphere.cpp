/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec_websphere.cpp
 *
 *  @author Matt Handy
 *  @date March 3, 2009
 *
 *  @brief This file contains the definitions and methods for dynaminc runtime loading of this dll.
 */

#include "gmsec_websphere.h"

#include "WebSConnection.h"

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::internal;

using namespace gmsec_websphere;


GMSEC_WEBS_API void createConnection(const Config* config, ConnectionInterface** connIf , Status* status)
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
			*connIf = new WebSConnection(*config);
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
