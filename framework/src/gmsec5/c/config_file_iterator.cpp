/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file config_file_iterator.cpp
 *
 * @brief This file contains functions for accessing elements of a configuration file (ConfigFile) object.
 */


#include <gmsec5/c/config_file_iterator.h>

#include <gmsec5/Config.h>
#include <gmsec5/ConfigFileIterator.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Message.h>

using namespace gmsec::api5;


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextConfig(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextConfig() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextMessage(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextMessage() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


GMSEC_BOOL CALL_TYPE configFileIteratorHasNextSubscription(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_BOOL flag = GMSEC_BOOL::GMSEC_FALSE;
	Status     result;

	ConfigFileIterator* cfi = reinterpret_cast<ConfigFileIterator*>(iter);

	if (!cfi)
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "ConfigFileIterator handle is NULL");
	}
	else
	{
		flag = (cfi->hasNextSubscription() ? GMSEC_BOOL::GMSEC_TRUE : GMSEC_BOOL::GMSEC_FALSE);
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


GMSEC_SubscriptionEntry CALL_TYPE configFileIteratorNextSubscription(GMSEC_ConfigFileIterator iter, GMSEC_Status status)
{
	GMSEC_SubscriptionEntry entry = NULL;
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
			entry = reinterpret_cast<GMSEC_SubscriptionEntry>(const_cast<ConfigFile::SubscriptionEntry*>(&cfi->nextSubscription()));
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
