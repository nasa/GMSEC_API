/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_file.cpp
 *
 * @brief This file contains functions for interfacing with a ConfigFile object.
 */


#include <gmsec5/c/config_file.h>

#include <gmsec5/Config.h>
#include <gmsec5/ConfigFile.h>
#include <gmsec5/ConfigFileIterator.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>

using namespace gmsec::api5;


GMSEC_ConfigFile CALL_TYPE configFileCreate(GMSEC_Status status)
{
	Status           result;
	GMSEC_ConfigFile cfgFile = NULL;

	try
	{
		cfgFile = reinterpret_cast<GMSEC_ConfigFile>(new ConfigFile());
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return cfgFile;
}


GMSEC_ConfigFile CALL_TYPE configFileCreateUsingConfig(GMSEC_Config config, GMSEC_Status status)
{
	Status           result;
	GMSEC_ConfigFile cfgFile = NULL;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (!cfg)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else
	{
		try
		{
			cfgFile = reinterpret_cast<GMSEC_ConfigFile>(new ConfigFile(*cfg));
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

	return cfgFile;
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
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE configFileSave(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
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
			cf->save();
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
}


void CALL_TYPE configFileSaveToFile(GMSEC_ConfigFile cfgFile, const char* filePath, GMSEC_BOOL compact, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else if (!filePath)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "File path is NULL");
	}
	else
	{
		try
		{
			cf->save(filePath, compact == GMSEC_BOOL::GMSEC_TRUE);
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
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


const char* CALL_TYPE configFileToXML(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
{
	const char* xml = NULL;
	Status      result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		xml = cf->toXML();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return xml;
}


GMSEC_BOOL CALL_TYPE configFileIsLoaded(GMSEC_ConfigFile cfgFile, GMSEC_Status status)
{
	GMSEC_BOOL loaded = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		loaded = (cf->isLoaded() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return loaded;
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
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_Config CALL_TYPE configFileLookupConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_Config config = NULL;
	Status       result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

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
		catch (const GmsecException& e)
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


GMSEC_BOOL CALL_TYPE configFileRemoveConfig(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		removed = (cf->removeConfig(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
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
		catch (const GmsecException& e)
		{
			result = Status(e);
		}
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_Message CALL_TYPE configFileLookupMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

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
		catch (const GmsecException& e)
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


GMSEC_BOOL CALL_TYPE configFileRemoveMessage(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		removed = (cf->removeMessage(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return removed;
}


void CALL_TYPE configFileAddSubscriptionEntry(GMSEC_ConfigFile cfgFile, GMSEC_SubscriptionEntry entry, GMSEC_Status status)
{
	Status result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);
	ConfigFile::SubscriptionEntry* cfe = reinterpret_cast<ConfigFile::SubscriptionEntry*>(entry);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else if (!cfe)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		cf->addSubscriptionEntry(*cfe);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_SubscriptionEntry CALL_TYPE configFileLookupSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_SubscriptionEntry entry = NULL;
	Status                  result;

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

			entry = reinterpret_cast<GMSEC_SubscriptionEntry>( new ConfigFile::SubscriptionEntry(nativeEntry) );
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

	return entry;
}


GMSEC_BOOL CALL_TYPE configFileRemoveSubscriptionEntry(GMSEC_ConfigFile cfgFile, const char* name, GMSEC_Status status)
{
	GMSEC_BOOL removed = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFile* cf = reinterpret_cast<ConfigFile*>(cfgFile);

	if (!cf)
	{
		result = Status(CONFIGFILE_ERROR, UNINITIALIZED_OBJECT, "ConfigFile handle is NULL");
	}
	else
	{
		removed = (cf->removeSubscriptionEntry(name) ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
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



GMSEC_SubscriptionEntry CALL_TYPE subscriptionEntryCreate(const char* name, const char* pattern, GMSEC_Status status)
{
	GMSEC_SubscriptionEntry entry = NULL;
	Status                  result;

	try
	{
		entry = reinterpret_cast<GMSEC_SubscriptionEntry>(new ConfigFile::SubscriptionEntry(name, pattern));
	}
	catch (const GmsecException& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return entry;
}


void CALL_TYPE subscriptionEntryDestroy(GMSEC_SubscriptionEntry* entry)
{
	if (entry && *entry)
	{
		ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(*entry);
		delete se;
		*entry = NULL;
	}
}


const char* CALL_TYPE subscriptionEntryGetName(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
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
		name = se->getName();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


const char* CALL_TYPE subscriptionEntryGetPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
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
		pattern = se->getPattern();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return pattern;
}


void CALL_TYPE subscriptionEntryAddExcludedPattern(GMSEC_SubscriptionEntry subEntry, const char* pattern, GMSEC_Status status)
{
	Status result;

	ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(subEntry);

	if (!se)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		try
		{
			se->addExcludedPattern(pattern);
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
}


GMSEC_BOOL CALL_TYPE subscriptionEntryHasNextExcludedPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFile::SubscriptionEntry* se = reinterpret_cast<ConfigFile::SubscriptionEntry*>(subEntry);

	if (!se)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "SubscriptionEntry handle is NULL");
	}
	else
	{
		flag = (se->hasNextExcludedPattern() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


const char* CALL_TYPE subscriptionEntryNextExcludedPattern(GMSEC_SubscriptionEntry subEntry, GMSEC_Status status)
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
		catch (const GmsecException& e)
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
