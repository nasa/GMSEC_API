/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* @file gmsec_amqp.h
 *  This file provides a template for implementing a middleware wrapper.
 */

#include "gmsec_amqp.h"

#include "AMQPConnection.h"

#include <gmsec5/util/Log.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;

using namespace gmsec_amqp;


GMSEC_AMQP_API void createConnection(const Config* config, ConnectionInterface** connIf, Status* status)
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
            *connIf = new AMQPConnection(*config);
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
