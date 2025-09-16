/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file heartbeat_generator.cpp
 *
 *  @brief This file contains a class for starting a heartbeat message generator.
 */


#include <gmsec5/c/heartbeat_generator.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/util/List.h>

#include <gmsec5/Config.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/HeartbeatGenerator.h>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


GMSEC_HeartbeatGenerator CALL_TYPE heartbeatGeneratorCreate(GMSEC_Config config,
                                                            GMSEC_U16    hbPubRate,
                                                            GMSEC_Status status)
{
	GMSEC_HeartbeatGenerator hbgen = NULL;

	Config* cfg = reinterpret_cast<Config*>(config);
	Status  result;

	if (!cfg)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			hbgen = reinterpret_cast<GMSEC_HeartbeatGenerator>(new HeartbeatGenerator(*cfg, hbPubRate));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hbgen;
}


GMSEC_HeartbeatGenerator CALL_TYPE heartbeatGeneratorCreateWithFields(GMSEC_Config config,
                                                                      GMSEC_U16    hbPubRate,
                                                                      GMSEC_Field  fields[],
                                                                      size_t       numFields,
                                                                      GMSEC_Status status)
{
	GMSEC_HeartbeatGenerator hbgen = NULL;

	Config* cfg = reinterpret_cast<Config*>(config);
	Status  result;

	if (!cfg)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else if (!fields || numFields == 0)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Array of fields is NULL or field count is zero");
	}
	else
	{
		List<Field*> dl_fields;

		for (size_t i = 0; i < numFields; ++i)
		{
			dl_fields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			hbgen = reinterpret_cast<GMSEC_HeartbeatGenerator>(new HeartbeatGenerator(*cfg, hbPubRate, dl_fields));
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hbgen;
}


void CALL_TYPE heartbeatGeneratorDestroy(GMSEC_HeartbeatGenerator* hbgen)
{
	if (hbgen && *hbgen)
	{
		HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(*hbgen);
		delete h;
		*hbgen = NULL;
	}
}


GMSEC_BOOL CALL_TYPE heartbeatGeneratorStart(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
{
	GMSEC_BOOL hbgenStatus = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		try
		{
			hbgenStatus = (h->start() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hbgenStatus;
}


GMSEC_BOOL CALL_TYPE heartbeatGeneratorStop(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
{
	GMSEC_BOOL hbgenStatus = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		hbgenStatus = (h->stop() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hbgenStatus;
}


GMSEC_BOOL CALL_TYPE heartbeatGeneratorIsRunning(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
{
	GMSEC_BOOL isRunning = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		isRunning = (h->isRunning() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return isRunning;
}


void CALL_TYPE heartbeatGeneratorChangePublishRate(GMSEC_HeartbeatGenerator hbgen, GMSEC_U16 pubRate, GMSEC_Status status)
{
	Status result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		h->changePublishRate(pubRate);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE heartbeatGeneratorSetField(GMSEC_HeartbeatGenerator hbgen, GMSEC_Field field, GMSEC_Status status)
{
	GMSEC_BOOL overwritten = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);
	Field*              f = reinterpret_cast<Field*>(field);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else if (!f)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		try
		{
			overwritten = (h->setField(*f) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return overwritten;
}
