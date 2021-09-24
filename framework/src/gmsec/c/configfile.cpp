
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file configfile.cpp
 *
 *  This file contains the C-wrapper for functions in the ConfigFile object.
 *
*/

#include <gmsec/c/configfile.h>

#include <gmsec/Config.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

void CALL_TYPE gmsec_CreateConfigFile(GMSEC_CONFIGFILE_OBJECT *cfgfile, const char *filepath, GMSEC_STATUS_OBJECT status)
{
	if (filepath == NULL)
	{
		*cfgfile = (GMSEC_CONFIGFILE_OBJECT)new ConfigFile();
	}
	else
	{
		*cfgfile = (GMSEC_CONFIGFILE_OBJECT)new ConfigFile(filepath);
	}
	if (NULL != status)
	{
		((Status *)status)->ReSet();
	}
}

void CALL_TYPE gmsec_DestroyConfigFile(GMSEC_CONFIGFILE_OBJECT *cfgfile, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == *cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::DestroyConfigFile : " << result.Get(); 
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)*cfgfile;
		delete tmpCF;
		*cfgfile = NULL;
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_LoadConfigFile(GMSEC_CONFIGFILE_OBJECT cfgfile, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::LoadConfigFile : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->Load();
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_LoadConfigFilePath(GMSEC_CONFIGFILE_OBJECT cfgfile, const char* filePath, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
		LOG_WARNING << "configfile_c::LoadConfigFile : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->Load(filePath);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}


void CALL_TYPE gmsec_ConfigFileSave(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *filePath, GMSEC_BOOL gbCompact, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileSavePath : " << result.Get();
	}
	else
	{
		bool compact = (gbCompact == GMSEC_TRUE);
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->Save(filePath, compact);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ToXMLConfigFile(GMSEC_CONFIGFILE_OBJECT cfgfile, const char **xml, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ToXMLConfigFile : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->ToXML(*xml);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_FromXMLConfigFile(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *xml, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::FromXMLConfigFile : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->FromXML(xml);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileIsLoaded(GMSEC_CONFIGFILE_OBJECT cfgfile, GMSEC_BOOL loaded)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
		LOG_WARNING << "configfile_c::ConfigFileIsLoaded : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		loaded = (tmpCF->IsLoaded() ? GMSEC_TRUE : GMSEC_FALSE);

		//Note:  The value assigned to 'loaded' will not be returned to the caller.  The
		//       reason is that the parameter is passed by value.
	}
}

void CALL_TYPE gmsec_LookupConfigFileConfig(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_CONFIG_OBJECT cfg, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::LookupConfigFileConfig : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		if (NULL == cfg)
		{
			result.Set(GMSEC_STATUS_CONFIG_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Config handle is NULL");
                        LOG_WARNING << "configfile_c::LookupConfigFileConfig : " << result.Get();
		}
		else
		{
			Config *tmp = (Config *)cfg;
			result = tmpCF->LookupConfig(name,tmp);
		}
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_LookupConfigFileMessage(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_MESSAGE_OBJECT msg, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::LookupConfigFileMessage : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		if (NULL == msg)
		{
			result.Set(GMSEC_STATUS_MESSAGE_ERROR,
			           GMSEC_OTHER_ERROR,
			           "Message handle is NULL");
                        LOG_WARNING << "configfile_c::LookupConfigFileMessage : " << result.Get();
		}
		else
		{
			Message *tmp = (Message *)msg;
			result = tmpCF->LookupMessage(name,tmp);
		}
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_LookupConfigFileSubscription(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, const char **pattern, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::LookupConfigFileSubscription : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->LookupSubscription(name,*pattern);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileAddSubscription(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, const char *subscription, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileAddSubscription : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->AddSubscription(name, subscription);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileAddConfig(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_CONFIG_OBJECT config, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileAddConfig : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		Config *cfg = (Config *) config;
		result = tmpCF->AddConfig(name, *cfg);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileAddMessage(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_MESSAGE_OBJECT message, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileAddMessage : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		Message *msg = (Message *) message;
		result = tmpCF->AddMessage(name, *msg);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileAddCustomXML(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *xml, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileAddCustomXML : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->AddCustomXML(xml);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileRemoveSubscription(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileRemoveSubscription : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->RemoveSubscription(name);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileRemoveConfig(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileRemoveConfig : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->RemoveConfig(name);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileRemoveMessage(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *name, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileRemoveMessage : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->RemoveMessage(name);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

void CALL_TYPE gmsec_ConfigFileRemoveCustomXML(GMSEC_CONFIGFILE_OBJECT cfgfile, const char *xml, GMSEC_STATUS_OBJECT status)
{
	Status result;
	if (NULL == cfgfile)
	{
		result.Set(GMSEC_STATUS_CONFIG_ERROR,
		           GMSEC_OTHER_ERROR,
		           "ConfigFile handle is NULL");
                LOG_WARNING << "configfile_c::ConfigFileRemoveCustomXML : " << result.Get();
	}
	else
	{
		ConfigFile *tmpCF = (ConfigFile *)cfgfile;
		result = tmpCF->RemoveCustomXML(xml);
	}
	if (NULL != status)
	{
		*((Status *)status) = result;
	}
}

