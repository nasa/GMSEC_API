/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file config_file_iterator.cpp
 *
 * @brief This file contains functions for accessing elements of a configuration file (ConfigFile) object.
 */


#include <gmsec4/c/config_file_iterator.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFileIterator.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Message.h>

using namespace gmsec::api;


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextConfig(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_FALSE;
	Status     result;

	const ConfigFileIterator* cfi = reinterpret_cast<const ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextConfig() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextMessage(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_FALSE;
	Status     result;

	const ConfigFileIterator* cfi = reinterpret_cast<const ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextMessage() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextSubscription(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_FALSE;
	Status     result;

	const ConfigFileIterator* cfi = reinterpret_cast<const ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextSubscription() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextCustomElement(const GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_FALSE;
	Status     result;

	const ConfigFileIterator* cfi = reinterpret_cast<const ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextCustomElement() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_ConfigEntry CALL_TYPE configFileIteratorNextConfig(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_ConfigEntry entry = {0, 0};
	Status            result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		try
		{
			const ConfigFile::ConfigEntry& nativeEntry = cfi->nextConfig();

			Config& cfg = const_cast<Config&>(nativeEntry.getConfig());

			entry.name   = nativeEntry.getName();
			entry.config = reinterpret_cast<GMSEC_Config>(&cfg);
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

	return entry;
}


GMSEC_MessageEntry CALL_TYPE configFileIteratorNextMessage(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_MessageEntry entry = {0, 0};
	Status             result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		try
		{
			const ConfigFile::MessageEntry& nativeEntry = cfi->nextMessage();

			Message& msg = const_cast<Message&>(nativeEntry.getMessage());

			entry.name    = nativeEntry.getName();
			entry.message = reinterpret_cast<GMSEC_Message>(&msg);
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

	return entry;
}


GMSEC_SubscriptionEntry CALL_TYPE configFileIteratorNextSubscription(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_SubscriptionEntry entry = {0, 0};
	Status                  result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		try
		{
			const ConfigFile::SubscriptionEntry& nativeEntry = cfi->nextSubscription();

			entry.name    = nativeEntry.getName();
			entry.subject = nativeEntry.getPattern();
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

	return entry;
}


const char* CALL_TYPE configFileIteratorNextCustomElement(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	const char* custom = NULL;
	Status      result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		try
		{
			custom = cfi->nextCustomElement();
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

	return custom;
}


void CALL_TYPE configFileIteratorReset(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	Status result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		cfi->reset();
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextSubscriptionEntry(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_FALSE;
	Status     result;

	const ConfigFileIterator* cfi = reinterpret_cast<const ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextSubscription() ? GMSEC_TRUE : GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_SubscriptionEntry_Handle CALL_TYPE configFileIteratorNextSubscriptionEntry(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_SubscriptionEntry_Handle entry = NULL;
	Status                         result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		try
		{
			ConfigFile::SubscriptionEntry& nativeEntry =
				const_cast<ConfigFile::SubscriptionEntry&>(cfi->nextSubscription());

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
