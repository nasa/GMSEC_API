/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config.cpp
 *
 * @brief This file contains function for handling configurations for use in the creation of
 * connections and messages.
 */

#include <gmsec5/c/config.h>

#include <gmsec5_defs.h>
#include <gmsec5/Config.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>


using namespace gmsec::api5;


static void CALL_TYPE setStatus(GMSEC_Status status, GMSEC_I32 clazz, GMSEC_I32 code, const char* text, GMSEC_I32 custom)
{
	Status* s = reinterpret_cast<Status*>(status);

	if (s)
	{
		*s = Status(clazz, code, text, custom);
	}
}


static bool CALL_TYPE isGoodConfig(GMSEC_Config cfg, GMSEC_Status status)
{
	bool    isGood = true;
	Config* config = reinterpret_cast<Config*>(cfg);

	if (config)
	{
		Status* s = reinterpret_cast<Status*>(status);

		if (s)
		{
			s->reset();
		}
	}
	else
	{
		isGood = false;
		setStatus(status, CONFIG_ERROR, UNINITIALIZED_OBJECT, "Config object is NULL", 0);
	}

	return isGood;
}


GMSEC_Config CALL_TYPE configCreate(void)
{
	return reinterpret_cast<GMSEC_Config>(new Config());
}


GMSEC_Config CALL_TYPE configCreateWithArgs(int argc, char* argv[])
{
	return reinterpret_cast<GMSEC_Config>(new Config(argc, argv));
}


GMSEC_Config CALL_TYPE configCreateUsingData(const char* data, DataType type, GMSEC_Status status)
{
	GMSEC_Config config = NULL;
	Status       result;

	try
	{
		config = reinterpret_cast<GMSEC_Config>(new Config(data, type));
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*((Status*) status) = result;
	}

	return config;
}


GMSEC_Config CALL_TYPE configCreateCopy(GMSEC_Config other, GMSEC_Status status)
{
	if (isGoodConfig(other, status))
	{
		Config* cfgOther = reinterpret_cast<Config*>(other);

		return reinterpret_cast<GMSEC_Config>(new Config(*cfgOther));
	}

	return NULL;
}


void CALL_TYPE configDestroy(GMSEC_Config* cfg)
{
	if (cfg && *cfg)
	{
		Config* config = reinterpret_cast<Config*>(*cfg);
		delete config;
		*cfg = NULL;
	}
}


void CALL_TYPE configAddValue(GMSEC_Config cfg, const char *name, const char *value, GMSEC_Status status)
{
	if (isGoodConfig(cfg, status))
	{
		try
		{
			reinterpret_cast<Config*>(cfg)->addValue(name, value);
		}
		catch (const GmsecException& e)
		{
			setStatus(status, e.getErrorClass(), e.getErrorCode(), e.getErrorMessage(), e.getCustomCode());
		}
	}
}


GMSEC_BOOL CALL_TYPE configClearValue(GMSEC_Config cfg, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL cleared = GMSEC_BOOL::GMSEC_FALSE;

	if (isGoodConfig(cfg, status))
	{
		try
		{
			cleared = (reinterpret_cast<Config*>(cfg)->clearValue(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			setStatus(status, e.getErrorClass(), e.getErrorCode(), e.getErrorMessage(), e.getCustomCode());
		}
	}

	return cleared;
}


const char* CALL_TYPE configGetValue(GMSEC_Config cfg, const char* name, GMSEC_Status status)
{
	const char* value = NULL;

	if (isGoodConfig(cfg, status))
	{
		value = reinterpret_cast<Config*>(cfg)->getValue(name);
	}

	return value;
}


const char* CALL_TYPE configGetValueOrDefault(GMSEC_Config cfg, const char* name, const char* defaultValue, GMSEC_Status status)
{
	const char* value = NULL;

	if (isGoodConfig(cfg, status))
	{
		value = reinterpret_cast<Config*>(cfg)->getValue(name,defaultValue);
	}

	return value;
}


GMSEC_BOOL CALL_TYPE configGetBooleanValue(GMSEC_Config cfg, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL value = GMSEC_BOOL::GMSEC_FALSE;

	if (isGoodConfig(cfg, status))
	{
		try
		{
			value = (reinterpret_cast<Config*>(cfg)->getBooleanValue(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
		}
		catch (const GmsecException& e)
		{
			setStatus(status, e.getErrorClass(), e.getErrorCode(), e.getErrorMessage(), e.getCustomCode());
		}
	}

	return value;
}


GMSEC_BOOL CALL_TYPE configGetBooleanValueOrDefault(GMSEC_Config cfg, const char* name, GMSEC_BOOL defaultValue, GMSEC_Status status)
{
	GMSEC_BOOL value = GMSEC_BOOL::GMSEC_FALSE;

	if (isGoodConfig(cfg, status))
	{
		value = (reinterpret_cast<Config*>(cfg)->getBooleanValue(name, defaultValue == GMSEC_BOOL::GMSEC_TRUE) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	return value;
}


int CALL_TYPE configGetIntegerValue(GMSEC_Config cfg, const char* name, GMSEC_Status status)
{
	int value = 0;

	if (isGoodConfig(cfg, status))
	{
		try
		{
			value = reinterpret_cast<Config*>(cfg)->getIntegerValue(name);
		}
		catch (const GmsecException& e)
		{
			setStatus(status, e.getErrorClass(), e.getErrorCode(), e.getErrorMessage(), e.getCustomCode());
		}
	}

	return value;
}


int CALL_TYPE configGetIntegerValueOrDefault(GMSEC_Config cfg, const char* name, int defaultValue, GMSEC_Status status)
{
	int value = 0;

	if (isGoodConfig(cfg, status))
	{
		value = reinterpret_cast<Config*>(cfg)->getIntegerValue(name, defaultValue);
	}

	return value;
}


double CALL_TYPE configGetDoubleValue(GMSEC_Config cfg, const char* name, GMSEC_Status status)
{
	double value = 0;

	if (isGoodConfig(cfg, status))
	{
		try
		{
			value = reinterpret_cast<Config*>(cfg)->getDoubleValue(name);
		}
		catch (const GmsecException& e)
		{
			setStatus(status, e.getErrorClass(), e.getErrorCode(), e.getErrorMessage(), e.getCustomCode());
		}
	}

	return value;
}


double CALL_TYPE configGetDoubleValueOrDefault(GMSEC_Config cfg, const char* name, double defaultValue, GMSEC_Status status)
{
	double value = 0;

	if (isGoodConfig(cfg, status))
	{
		value = reinterpret_cast<Config*>(cfg)->getDoubleValue(name, defaultValue);
	}

	return value;
}


void CALL_TYPE configClear(GMSEC_Config cfg, GMSEC_Status status)
{
	if (isGoodConfig(cfg, status))
	{
		reinterpret_cast<Config*>(cfg)->clear();
	}
}


void CALL_TYPE configGetFirst(GMSEC_Config cfg, const char** name, const char** value, GMSEC_Status status)
{
	if (!name)
	{
			setStatus(status, CONFIG_ERROR, UNINITIALIZED_OBJECT, "The name parameter cannot be NULL", 0);
	}
	else if (!value)
	{
			setStatus(status, CONFIG_ERROR, UNINITIALIZED_OBJECT, "The value parameter cannot be NULL", 0);
	}
	else if (isGoodConfig(cfg, status))
	{
		bool result = reinterpret_cast<Config*>(cfg)->getFirst(*name, *value);

		if (!result)
		{
			setStatus(status, CONFIG_ERROR, CONFIG_END_REACHED, "A Config entry is not available", 0);
		}
	}
	else
	{
		*name  = NULL;
		*value = NULL;
	}
}


void CALL_TYPE configGetNext(GMSEC_Config cfg, const char** name, const char** value, GMSEC_Status status)
{
	if (!name)
	{
			setStatus(status, CONFIG_ERROR, UNINITIALIZED_OBJECT, "The name parameter cannot be NULL", 0);
	}
	else if (!value)
	{
			setStatus(status, CONFIG_ERROR, UNINITIALIZED_OBJECT, "The value parameter cannot be NULL", 0);
	}
	else if (isGoodConfig(cfg, status))
	{
		bool result = reinterpret_cast<Config*>(cfg)->getNext(*name, *value);

		if (!result)
		{
			setStatus(status, CONFIG_ERROR, CONFIG_END_REACHED, "A Config entry is not available", 0);
		}
	}
	else
	{
		*name  = NULL;
		*value = NULL;
	}
}


void CALL_TYPE configMerge(GMSEC_Config config, GMSEC_Config other, GMSEC_BOOL overwriteExisting, GMSEC_Status status)
{
	if (isGoodConfig(config, status) && isGoodConfig(other, status))
	{
		Config* cfg      = reinterpret_cast<Config*>(config);
		Config* otherCfg = reinterpret_cast<Config*>(other);

		cfg->merge(*otherCfg, (overwriteExisting == GMSEC_BOOL::GMSEC_TRUE));
	}
}


const char* CALL_TYPE configToXML(GMSEC_Config cfg, GMSEC_Status status)
{
	const char* xml = NULL;

	if (isGoodConfig(cfg, status))
	{
		xml = reinterpret_cast<Config*>(cfg)->toXML();
	}

	return xml;
}


void CALL_TYPE configFromXML(GMSEC_Config cfg, const char* xml, GMSEC_Status status)
{
	if (isGoodConfig(cfg, status))
	{
		reinterpret_cast<Config*>(cfg)->fromXML(xml);
	}
}


GMSEC_Config CALL_TYPE configGetFromFile(const char* filename, const char* configName, GMSEC_Status status)
{
	Status       result;
	GMSEC_Config c_cfg = NULL;

	try
	{
		c_cfg = reinterpret_cast<GMSEC_Config>(new Config( Config::getFromFile(filename, configName) ));
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return c_cfg;
}


const char* CALL_TYPE configToJSON(GMSEC_Config cfg, GMSEC_Status status)
{
	const char* json = NULL;

	if (isGoodConfig(cfg, status))
	{
		json = reinterpret_cast<Config*>(cfg)->toJSON();
	}

	return json;
}
