/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mnemonic_sample.cpp
 *
 *  @brief This file contains functions for the management of a MIST DeviceParam object.
 */


#include <gmsec4/c/mist/mnemonic_sample.h>

#include <gmsec4/mist/MnemonicSample.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_MnemonicSample CALL_TYPE mnemonicSampleCreate(const char* timestamp, const GMSEC_Field rawValue, GMSEC_Status status)
{
	GMSEC_MnemonicSample sample = NULL;
	Status               result;

	const Field* raw = reinterpret_cast<const Field*>(rawValue);

	if (raw)
	{
		try
		{
			sample = reinterpret_cast<GMSEC_MnemonicSample>(new MnemonicSample(timestamp, *raw));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Raw Field handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sample;
}


GMSEC_MnemonicSample CALL_TYPE mnemonicSampleCreateCopy(const GMSEC_MnemonicSample other, GMSEC_Status status)
{
	GMSEC_MnemonicSample sample = NULL;
	Status               result;

	const MnemonicSample* otherSample = reinterpret_cast<const MnemonicSample*>(other);

	if (otherSample)
	{
		try
		{
			sample = reinterpret_cast<GMSEC_MnemonicSample>(new MnemonicSample(*otherSample));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sample;
}


void CALL_TYPE mnemonicSampleDestroy(GMSEC_MnemonicSample* sample)
{
	if (sample && *sample)
	{
		MnemonicSample* tmp = reinterpret_cast<MnemonicSample*>(*sample);

		delete tmp;

		*sample = NULL;
	}
}


const char* CALL_TYPE mnemonicSampleGetTimestamp(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	const char* timestamp = NULL;
	Status      result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		timestamp = mnemSample->getTimestamp();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return timestamp;
}


const GMSEC_Field CALL_TYPE mnemonicSampleGetRawValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_Field rawValue = NULL;
	Status      result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		Field& tmp = const_cast<Field&>(mnemSample->getRawValue());

		rawValue = reinterpret_cast<GMSEC_Field>(&tmp);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return rawValue;
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasEUValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasValue = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasValue = (mnemSample->hasEUValue() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasValue;
}


const GMSEC_Field CALL_TYPE mnemonicSampleGetEUValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_Field value = NULL;
	Status      result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			Field& tmp = const_cast<Field&>(mnemSample->getEUValue());

			value = reinterpret_cast<GMSEC_Field>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE mnemonicSampleSetEUValue(GMSEC_MnemonicSample sample, const GMSEC_Field value, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);
	const Field*    field      = reinterpret_cast<const Field*>(value);

	if (!mnemSample)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}
	else if (!field)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		mnemSample->setEUValue(*field);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasTextValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasValue = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasValue = (mnemSample->hasTextValue() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasValue;
}


const char* CALL_TYPE mnemonicSampleGetTextValue(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	const char* value = NULL;
	Status      result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			value = mnemSample->getTextValue();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE mnemonicSampleSetTextValue(GMSEC_MnemonicSample sample, const char* text, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			mnemSample->setTextValue(text);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasFlags(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasFlags = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasFlags = (mnemSample->hasFlags() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasFlags;
}


GMSEC_I32 CALL_TYPE mnemonicSampleGetFlags(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_I32 value = 0;
	Status    result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			value = mnemSample->getFlags();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE mnemonicSampleSetFlags(GMSEC_MnemonicSample sample, GMSEC_I32 flags, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);

	if (mnemSample)
	{
		mnemSample->setFlags(flags);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasLimitEnableDisable(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasLimit = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasLimit = (mnemSample->hasLimitEnableDisable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasLimit;
}


GMSEC_BOOL CALL_TYPE mnemonicSampleGetLimitEnableDisable(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL value = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			value = (mnemSample->getLimitEnableDisable() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE mnemonicSampleSetLimitEnableDisable(GMSEC_MnemonicSample sample, GMSEC_BOOL value, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);

	if (mnemSample)
	{
		mnemSample->setLimitEnableDisable(value == GMSEC_TRUE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasLimit(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasLimit = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasLimit = (mnemSample->hasLimit() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasLimit;
}


GMSEC_LimitFlag CALL_TYPE mnemonicSampleGetLimit(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_LimitFlag flag = GMSEC_UNSPECIFIED;
	Status          result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			int tmp = mnemSample->getLimit();

			flag = static_cast<GMSEC_LimitFlag>(tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return flag;
}


void CALL_TYPE mnemonicSampleSetLimit(GMSEC_MnemonicSample sample, GMSEC_LimitFlag limit, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);

	if (mnemSample)
	{
		int tmp = limit;

		mnemSample->setLimit(static_cast<MnemonicSample::LimitFlag>(tmp));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasStalenessStatus(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasStaleness = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasStaleness = (mnemSample->hasStalenessStatus() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasStaleness;
}


GMSEC_BOOL CALL_TYPE mnemonicSampleGetStalenessStatus(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL value = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			value = (mnemSample->getStalenessStatus() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE mnemonicSampleSetStalenessStatus(GMSEC_MnemonicSample sample, GMSEC_BOOL staleness, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);

	if (mnemSample)
	{
		mnemSample->setStalenessStatus(staleness == GMSEC_TRUE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicSampleHasQuality(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL hasQuality = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		hasQuality = (mnemSample->hasQuality() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasQuality;
}


GMSEC_BOOL CALL_TYPE mnemonicSampleGetQuality(const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	GMSEC_BOOL value = GMSEC_FALSE;
	Status     result;

	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (mnemSample)
	{
		try
		{
			value = (mnemSample->getQuality() ? GMSEC_TRUE : GMSEC_FALSE);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return value;
}


void CALL_TYPE mnemonicSampleSetQuality(GMSEC_MnemonicSample sample, GMSEC_BOOL quality, GMSEC_Status status)
{
	Status result;

	MnemonicSample* mnemSample = reinterpret_cast<MnemonicSample*>(sample);

	if (mnemSample)
	{
		mnemSample->setQuality(quality == GMSEC_TRUE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}
