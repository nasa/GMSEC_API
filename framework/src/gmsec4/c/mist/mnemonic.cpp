/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mnemonic.cpp
 *
 *  @brief This file contains functions for the management of a MIST Mnemonic object.
 */


#include <gmsec4/c/mist/mnemonic.h>

#include <gmsec4/mist/Mnemonic.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::util;


GMSEC_Mnemonic CALL_TYPE mnemonicCreate(const char* name, const GMSEC_MnemonicSample samples[], size_t numSamples, GMSEC_Status status)
{
	GMSEC_Mnemonic mnemonic = NULL;
	Status         result;

	DataList<MnemonicSample*> sampleList;

	for (size_t i = 0; i < numSamples; ++i)
	{
		sampleList.push_back(reinterpret_cast<MnemonicSample*>(samples[i]));
	}

	try
	{
		mnemonic = reinterpret_cast<GMSEC_Mnemonic>(new Mnemonic(name, sampleList));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return mnemonic;
}


GMSEC_Mnemonic CALL_TYPE mnemonicCreateWithNoSamples(const char* name, GMSEC_Status status)
{
    return mnemonicCreate(name, 0, 0, status);
}


GMSEC_Mnemonic CALL_TYPE mnemonicCreateCopy(const GMSEC_Mnemonic other, GMSEC_Status status)
{
	GMSEC_Mnemonic mnemonic = NULL;
	Status         result;

	const Mnemonic* otherMnemonic = reinterpret_cast<const Mnemonic*>(other);

	if (otherMnemonic)
	{
		mnemonic = reinterpret_cast<GMSEC_Mnemonic>(new Mnemonic(*otherMnemonic));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return mnemonic;
}


void CALL_TYPE mnemonicDestroy(GMSEC_Mnemonic* mnemonic)
{
	if (mnemonic && *mnemonic)
	{
		Mnemonic* tmp = reinterpret_cast<Mnemonic*>(*mnemonic);

		delete tmp;

		*mnemonic = NULL;
	}
}


const char* CALL_TYPE mnemonicGetName(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	const char* name = NULL;
	Status      result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		name = mnem->getName();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return name;
}


GMSEC_BOOL CALL_TYPE mnemonicStatusAvailable(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		available = (mnem->statusAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const GMSEC_Field CALL_TYPE mnemonicGetStatus(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	GMSEC_Field statusField = NULL;
	Status      result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		try
		{
			Field& tmp = const_cast<Field&>(mnem->getStatus());

			statusField = reinterpret_cast<GMSEC_Field>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return statusField;
}


void CALL_TYPE mnemonicSetStatus(GMSEC_Mnemonic mnemonic, const GMSEC_Field statusField, GMSEC_Status status)
{
	Status result;

	Mnemonic*    mnem  = reinterpret_cast<Mnemonic*>(mnemonic);
	const Field* field = reinterpret_cast<const Field*>(statusField);

	if (!mnem)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}
	else if (!field)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Field handle is NULL");
	}
	else
	{
		mnem->setStatus(*field);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


GMSEC_BOOL CALL_TYPE mnemonicUnitsAvailable(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	GMSEC_BOOL available = GMSEC_FALSE;
	Status     result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		available = (mnem->unitsAvailable() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return available;
}


const char* CALL_TYPE mnemonicGetUnits(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	const char* units = NULL;
	Status      result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		try
		{
			units = mnem->getUnits();
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return units;
}


void CALL_TYPE mnemonicSetUnits(GMSEC_Mnemonic mnemonic, const char* units, GMSEC_Status status)
{
	Status result;

	Mnemonic* mnem  = reinterpret_cast<Mnemonic*>(mnemonic);

	if (mnem)
	{
		try
		{
			mnem->setUnits(units);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


void CALL_TYPE mnemonicAddSample(GMSEC_Mnemonic mnemonic, const GMSEC_MnemonicSample sample, GMSEC_Status status)
{
	Status result;

	Mnemonic*             mnem       = reinterpret_cast<Mnemonic*>(mnemonic);
	const MnemonicSample* mnemSample = reinterpret_cast<const MnemonicSample*>(sample);

	if (!mnem)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}
	else if (!mnemSample)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicSample handle is NULL");
	}
	else
	{
		mnem->addSample(*mnemSample);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


size_t CALL_TYPE mnemonicGetSampleCount(const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	size_t count = 0;
	Status result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		count = mnem->getSampleCount();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return count;
}


const GMSEC_MnemonicSample CALL_TYPE mnemonicGetSample(const GMSEC_Mnemonic mnemonic, size_t index, GMSEC_Status status)
{
	GMSEC_MnemonicSample sample = NULL;
	Status               result;

	const Mnemonic* mnem = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (mnem)
	{
		try
		{
			MnemonicSample& tmp = const_cast<MnemonicSample&>(mnem->getSample(index));

			sample = reinterpret_cast<GMSEC_MnemonicSample>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return sample;
}
