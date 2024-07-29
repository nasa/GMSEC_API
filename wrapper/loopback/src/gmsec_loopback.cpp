/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file gmsec_loopback.cpp
 * @brief This file provides a template for implementing a middleware wrapper.
 */

#include "gmsec_loopback.h"

#include "LoopbackConnection.h"

#include <gmsec5/util/Log.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;

using namespace gmsec_loopback;


GMSEC_LOOPBACK_API void createConnection(const Config* config, ConnectionInterface** connIf, Status* status)
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
            *connIf = new LoopbackConnection(*config);
        }
        else
        {
            status->set(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, "No Config object supplied");
        }
    }
    catch (const std::exception& e)
    {
        status->set(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, e.what());
    }

}
