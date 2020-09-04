/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mnemonic_message.cpp
 *
 *  @brief This file contains functions for the management of a MIST MnemonicMessage object.
 *  The MnemonicMessage is a specialization, or extension, of the standard GMSEC Message.
 */


#include <gmsec4/c/mist/mnemonic_message.h>

#include <gmsec4/mist/MnemonicMessage.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;


GMSEC_Message CALL_TYPE mnemonicMessageCreate(const char* subject, unsigned int version, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(subject, version));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE mnemonicMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (config)
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(subject, *cfg, version));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE mnemonicMessageCreateUsingData(const char* data, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(data));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE mnemonicMessageCreateCopy(const GMSEC_Message other, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	const Message*         tmpMsg   = reinterpret_cast<const Message*>(other);
	const MnemonicMessage* otherMsg = dynamic_cast<const MnemonicMessage*>(tmpMsg);

	if (otherMsg)
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(*otherMsg));
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MnemonicMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


void CALL_TYPE mnemonicMessageAddMnemonic(GMSEC_Message msg, const GMSEC_Mnemonic mnemonic, GMSEC_Status status)
{
	Status result;

	Message*         tmpMsg  = reinterpret_cast<Message*>(msg);
	MnemonicMessage* mnemMsg = dynamic_cast<MnemonicMessage*>(tmpMsg);
	const Mnemonic*  mnem    = reinterpret_cast<const Mnemonic*>(mnemonic);

	if (!mnemMsg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MnemonicMessage");
	}
	else if (!mnem)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Mnemonic handle is NULL");
	}
	else
	{
		mnemMsg->addMnemonic(*mnem);
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}
}


size_t CALL_TYPE mnemonicMessageGetNumMnemonics(const GMSEC_Message msg, GMSEC_Status status)
{
	size_t count = 0;
	Status result;

	const Message*         tmpMsg  = reinterpret_cast<const Message*>(msg);
	const MnemonicMessage* mnemMsg = dynamic_cast<const MnemonicMessage*>(tmpMsg);

	if (mnemMsg)
	{
		count = mnemMsg->getNumMnemonics();
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MnemonicMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return count;
}


const GMSEC_Mnemonic CALL_TYPE mnemonicMessageGetMnemonic(const GMSEC_Message msg, size_t index, GMSEC_Status status)
{
	GMSEC_Mnemonic mnemonic = NULL;
	Status         result;

	const Message*         tmpMsg  = reinterpret_cast<const Message*>(msg);
	const MnemonicMessage* mnemMsg = dynamic_cast<const MnemonicMessage*>(tmpMsg);

	if (mnemMsg)
	{
		try
		{
			Mnemonic& tmp = const_cast<Mnemonic&>(mnemMsg->getMnemonic(index));

			mnemonic = reinterpret_cast<GMSEC_Mnemonic>(&tmp);
		}
		catch (Exception& e)
		{
			result = Status(e);
		}
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL or is not a MnemonicMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return mnemonic;
}


GMSEC_Message CALL_TYPE mnemonicMessageConvert(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Message mnemonicMsg = NULL;
	Status        result;

	const Message* tmpMsg = reinterpret_cast<const Message*>(msg);

	if (tmpMsg)
	{
		MnemonicMessage tmpMnemonicMsg = MnemonicMessage::convertMessage(*tmpMsg);

		mnemonicMsg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(tmpMnemonicMsg));
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Message handle is NULL");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return mnemonicMsg;
}
