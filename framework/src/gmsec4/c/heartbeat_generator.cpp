/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file heartbeat_generator.cpp
 *
 *  @brief This file contains a class for starting a heartbeat message generator.
 */


#include <gmsec4/c/heartbeat_generator.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/HeartbeatGenerator.h>


using namespace gmsec::api;
using namespace gmsec::api::util;


GMSEC_HeartbeatGenerator CALL_TYPE heartbeatGeneratorCreate(const GMSEC_Config config,
                                                            const char* hbMsgSubject,
                                                            GMSEC_U16 hbPubRate,
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
			hbgen = reinterpret_cast<GMSEC_HeartbeatGenerator>(new HeartbeatGenerator(*cfg, hbMsgSubject, hbPubRate));
		}
		catch (const Exception& e)
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


GMSEC_HeartbeatGenerator CALL_TYPE heartbeatGeneratorCreateWithFields(const GMSEC_Config config,
                                                                      const char* hbMsgSubject,
                                                                      GMSEC_U16 hbPubRate,
                                                                      GMSEC_Field fields[],
                                                                      size_t numFields,
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
		DataList<Field*> dl_fields;

		for (size_t i = 0; i < numFields; ++i)
		{
			dl_fields.push_back(reinterpret_cast<Field*>(fields[i]));
		}

		try
		{
			hbgen = reinterpret_cast<GMSEC_HeartbeatGenerator>(new HeartbeatGenerator(*cfg, hbMsgSubject, hbPubRate, dl_fields));
		}
		catch (const Exception& e)
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
	GMSEC_BOOL hbgenStatus = GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		hbgenStatus = (h->start() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hbgenStatus;
}


GMSEC_BOOL CALL_TYPE heartbeatGeneratorStop(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
{
	GMSEC_BOOL hbgenStatus = GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		hbgenStatus = (h->stop() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hbgenStatus;
}


GMSEC_BOOL CALL_TYPE heartbeatGeneratorIsRunning(GMSEC_HeartbeatGenerator hbgen, GMSEC_Status status)
{
	GMSEC_BOOL isRunning = GMSEC_FALSE;
	Status     result;

	HeartbeatGenerator* h = reinterpret_cast<HeartbeatGenerator*>(hbgen);

	if (!h)
	{
		result = Status(HEARTBEAT_GENERATOR_ERROR, UNINITIALIZED_OBJECT, "HeartbeatGenerator handle is NULL");
	}
	else
	{
		isRunning = (h->isRunning() ? GMSEC_TRUE : GMSEC_FALSE);
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
	GMSEC_BOOL overwritten = GMSEC_FALSE;
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
			overwritten = (h->setField(*f) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (const Exception& e)
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
