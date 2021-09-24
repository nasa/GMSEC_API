/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file config_file.cpp
 *
 * @brief This file contains functions for interfacing with a ConfigFile object.
 */


#include <gmsec4/c/config_file.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFile.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

using namespace gmsec::api;


GMSEC_ConfigFile CALL_TYPE configFileCreate(void)
{
	return reinterpret_cast<GMSEC_ConfigFile>(new ConfigFile());
}


void CALL_TYPE configFileDestroy(GMSEC_ConfigFile* cfgFile)
{
	if (cfgFile && *cfgFile)
	{
		ConfigFile* cf = reinterpret_cast<ConfigFile*>(*cfgFile);

		delete cf;

		*cfgFile = NULL;
	}
}


void CALL_TYPE configFileLoad(GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			cf->load(filePath);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileSave(const GMSEC_ConfigFile cfgFile, GMSEC_Status status)
{
	Status result;

	const ConfigFile* cf = reinterpret_cast<const ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			cf->save();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileSaveToFile(const GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_BOOL compact, GMSEC_Status status)
{
	Status result;

	const ConfigFile* cf = reinterpret_cast<const ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			cf->save(filePath, compact == GMSEC_TRUE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileFromXML(GMSEC_ConfigFile cfgFile, const char* xml, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			cf->fromXML(xml);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE configFileToXML(const GMSEC_ConfigFile cfgFile, GMSEC_Status status)
{
	const char* xml = NULL;
	Status      result;

	const ConfigFile* cf = reinterpret_cast<const ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			xml = cf->toXML();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return xml;
}


GMSEC_Config CALL_TYPE configFileLookupConfig(const GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_Config config = NULL;
	Status       result;

	const ConfigFile* cf = reinterpret_cast<const ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			config = reinterpret_cast<GMSEC_Config>(new Config(cf->lookupConfig(name)));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return config;
}


GMSEC_Message CALL_TYPE configFileLookupMessage(const GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	const ConfigFile* cf = reinterpret_cast<const ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new Message(cf->lookupMessage(name)));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


const char* CALL_TYPE configFileLookupSubscription(const GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	const char* subject = NULL;
	Status      result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			const ConfigFile::SubscriptionEntry& entry = cf->lookupSubscriptionEntry(name);

			subject = entry.getPattern();
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

	return subject;
}


void CALL_TYPE configFileAddSubscription(GMSEC_ConfigFile cfgFile, const char* name, const char* subject, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			cf->addSubscription(name, subject);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileAddConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Config config, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf  = reinterpret_cast<ConfigFile*>(cfgFile);
	Config*     cfg = reinterpret_cast<Config*>(config);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else if (!cfg)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			cf->addConfig(name, *cfg);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileAddMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Message msg, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);
	Message*    m  = reinterpret_cast<Message*>(msg);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else if (!m)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}
	else
	{
		try
		{
			cf->addMessage(name, *m);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileAddCustomXML(GMSEC_ConfigFile cfgFile, const char* xml, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			cf->addCustomXML(xml);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE configFileRemoveSubscription(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			removed = (cf->removeSubscription(name) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
}


GMSEC_BOOL CALL_TYPE configFileRemoveConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			removed = (cf->removeConfig(name) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
}


GMSEC_BOOL CALL_TYPE configFileRemoveMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			removed = (cf->removeMessage(name) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
}


GMSEC_BOOL CALL_TYPE configFileRemoveCustomXML(GMSEC_ConfigFile cfgFile, const char* xml, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			removed = (cf->removeCustomXML(xml) ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
}


GMSEC_ConfigFileIterator CALL_TYPE configFileGetIterator(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
{
	GMSEC_ConfigFileIterator cfgIter = NULL;
	Status                   result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		ConfigFileIterator& iter = cf->getIterator();

		cfgIter = reinterpret_cast<GMSEC_ConfigFileIterator>(&iter);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return cfgIter;

}


GMSEC_SubscriptionEntry_Handle CALL_TYPE configFileLookupSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_SubscriptionEntry_Handle entry = NULL;
	Status                         result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		try
		{
			ConfigFile::SubscriptionEntry& nativeEntry =
				const_cast<ConfigFile::SubscriptionEntry&>(cf->lookupSubscriptionEntry(name));

			entry = reinterpret_cast<GMSEC_SubscriptionEntry_Handle>(&nativeEntry);
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

	return entry;
}


const char* CALL_TYPE subscriptionEntryGetName(GMSEC_SubscriptionEntry_Handle subEntry, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(subEntry);

	if (!se)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		try
		{
			name = se->getName();
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

	return name;
}


const char* CALL_TYPE subscriptionEntryGetPattern(GMSEC_SubscriptionEntry_Handle subEntry, GMSEC_Status status)
{
	const char* pattern = NULL;
	Status      result;

	ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(subEntry);

	if (!se)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		try
		{
			pattern = se->getPattern();
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

	return pattern;
}


GMSEC_BOOL CALL_TYPE subscriptionEntryHasNextExcludedPattern(GMSEC_SubscriptionEntry_Handle subEntry, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_FALSE;
	Status     result;

	ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(subEntry);

	if (!se)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		flag = (se->hasNextExcludedPattern() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


const char* CALL_TYPE subscriptionEntryNextExcludedPattern(GMSEC_SubscriptionEntry_Handle subEntry, GMSEC_Status status)
{
	const char* subject = NULL;
	Status      result;

	ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(subEntry);

	if (!se)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		try
		{
			subject = se->nextExcludedPattern();
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

	return subject;
}


void CALL_TYPE subscriptionEntryDestroy(GMSEC_SubscriptionEntry_Handle entry)
{
	if (entry)
	{
		ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(entry);

		delete se;
	}
}
