
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file config.cpp
 *
 *  This file contains the C-wrapper for functions in the Config object.
 *
 *
*/

#include <gmsec/c/config.h>

#include <gmsec/Config.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

void CALL_TYPE gmsec_CreateConfig(GMSEC_CONFIG_OBJECT *cfg, GMSEC_STATUS_OBJECT status)
{
	*cfg = (GMSEC_CONFIG_OBJECT)new Config();
	if (NULL != status)
		((Status *)status)->ReSet();
}

void CALL_TYPE gmsec_CreateConfigParams(GMSEC_CONFIG_OBJECT *cfg, int argc, char *argv[], GMSEC_STATUS_OBJECT status)
{
	*cfg = (GMSEC_CONFIG_OBJECT)new Config(argc,argv);
	if (NULL != status)
		((Status *)status)->ReSet();
}

void CALL_TYPE gmsec_DestroyConfig(GMSEC_CONFIG_OBJECT *cfg, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == *cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::DestroyConfig : " << result.Get();
	}
	else
	{
		Config *tmp = (Config *)*cfg;
		delete tmp;
		*cfg = NULL;
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigAddValue(GMSEC_CONFIG_OBJECT cfg, const char *name, const char *value, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigAddValue : " << result.Get();
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->AddValue(name,value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigClearValue(GMSEC_CONFIG_OBJECT cfg, const char *name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigClearValue : " << result.Get();
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->ClearValue(name);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigGetValue(GMSEC_CONFIG_OBJECT cfg, const char *name, const char **value, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigGetValue : " << result.Get();
		*value = NULL;
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->GetValue(name,*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigClear(GMSEC_CONFIG_OBJECT cfg, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigClear : " << result.Get();
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->Clear();
	}
	if (NULL != status)
		*((Status *)status) = result;
}


void CALL_TYPE gmsec_ConfigGetFirst(GMSEC_CONFIG_OBJECT cfg, const char **name, const char **value, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigGetFirst : " << result.Get();
		*value = NULL;
		*name = NULL;
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->GetFirst(*name,*value);
	}
	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigGetNext(GMSEC_CONFIG_OBJECT cfg, const char **name, const char **value, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigGetNext : " << result.Get();
		*value = NULL;
		*name = NULL;
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->GetNext(*name,*value);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigToXML(GMSEC_CONFIG_OBJECT cfg, const char **xml, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigToXML : " << result.Get();
		*xml = NULL;
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->ToXML(*xml);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

void CALL_TYPE gmsec_ConfigFromXML(GMSEC_CONFIG_OBJECT cfg, const char *xml, GMSEC_STATUS_OBJECT status)
{
	Status result;

	if (NULL == cfg)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "Config handle is NULL");
                LOG_WARNING << "config_c::ConfigFromXML : " << result.Get();
	}
	else
	{
		Config *tmp = (Config *)cfg;
		result = tmp->FromXML(xml);
	}

	if (NULL != status)
		*((Status *)status) = result;
}

