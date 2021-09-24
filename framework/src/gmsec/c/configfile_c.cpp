
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file configfile_c.cpp
 *
 *  This file contains the C-wrapper for functions in the ConfigFile object.
*/

#include <gmsec/c/configfile_c.h>

#include <gmsec/Config.h>

#include <gmsec/internal/Log.h>


using namespace gmsec::util;
using namespace gmsec;

void CALL_TYPE CreateConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, const char *filepath, GMSEC_STATUS_HANDLE status)
{
	if (filepath == NULL)
	{
		*cfgfile = (GMSEC_CONFIG_HANDLE)new ConfigFile();
	}
	else
	{
		*cfgfile = (GMSEC_CONFIG_HANDLE)new ConfigFile(filepath);
	}
	if (NULL != status)
	{
		((Status *)status)->ReSet();
	}
}

void CALL_TYPE DestroyConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE LoadConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE LoadConfigFilePath(GMSEC_CONFIGFILE_HANDLE cfgfile, const char* filePath, GMSEC_STATUS_HANDLE status)
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


void CALL_TYPE ConfigFileSave(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *filePath, GMSEC_BOOL gbCompact, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ToXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char **xml, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE FromXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status)
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

GMSEC_BOOL CALL_TYPE ConfigFileIsLoaded(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_BOOL loaded)
{
	Status result;
	loaded = GMSEC_FALSE;
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
		loaded = (tmpCF->IsLoaded() == GMSEC_TRUE ? GMSEC_TRUE : GMSEC_FALSE);
	}
	return loaded;
}

void CALL_TYPE LookupConfigFileConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE LookupConfigFileMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE LookupConfigFileSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char **pattern, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileAddSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char *subscription, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileAddConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE config, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileAddMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE message, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileAddCustomXML(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileRemoveSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileRemoveConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileRemoveMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_STATUS_HANDLE status)
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

void CALL_TYPE ConfigFileRemoveCustomXML(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status)
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

