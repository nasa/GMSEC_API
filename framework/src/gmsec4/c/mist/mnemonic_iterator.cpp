/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file mnemonic_iterator.cpp
 *
 *  @brief Defines the functions which support the iteration over the Mnemonic objects stored in a MnemonicMessage.
 */


#include <gmsec4/c/mist/mnemonic_iterator.h>

#include <gmsec4/mist/MnemonicIterator.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_BOOL CALL_TYPE mnemonicIteratorHasNext(GMSEC_MnemonicIterator iter, GMSEC_Status status)
{
	MnemonicIterator* mnemIter = reinterpret_cast<MnemonicIterator*>(iter);
	GMSEC_BOOL        hasNext = GMSEC_FALSE;
	Status            result;

	if (mnemIter)
	{
		hasNext = (mnemIter->hasNext() ? GMSEC_TRUE : GMSEC_FALSE);
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "MnemonicIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return hasNext;
}


const GMSEC_Mnemonic CALL_TYPE mnemonicIteratorNext(GMSEC_MnemonicIterator iter, GMSEC_Status status)
{
	MnemonicIterator* mnemIter = reinterpret_cast<MnemonicIterator*>(iter);
	GMSEC_Mnemonic    mnemonic = NULL;
	Status            result;

	if (mnemIter)
	{
		try
		{
			const Mnemonic& mnem = mnemIter->next();

			mnemonic = (GMSEC_Mnemonic) &mnem;
		}
		catch (const Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "MnemonicIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return mnemonic;
}


void CALL_TYPE mnemonicIteratorReset(GMSEC_MnemonicIterator iter, GMSEC_Status status)
{
	MnemonicIterator* mnemIter = reinterpret_cast<MnemonicIterator*>(iter);
	Status            result;

	if (mnemIter)
	{
		mnemIter->reset();
	}
	else
	{
		result = Status(ITERATOR_ERROR, UNINITIALIZED_OBJECT, "MnemonicIterator handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}
