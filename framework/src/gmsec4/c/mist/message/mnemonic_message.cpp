/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mnemonic_message.cpp
 *
 *  @brief This file contains functions for the management of a MIST MnemonicMessage object.
 */


#include <gmsec4/c/mist/message/mnemonic_message.h>

#include <gmsec4/mist/message/MnemonicMessage.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Log.h>

#include <sstream>
#include <string>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;


GMSEC_Message CALL_TYPE mnemonicMessageCreateWithSpec(const char* subject,
	                                                  const char* schemaID,
	                                                  GMSEC_Specification spec,
	                                                  GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(subject, schemaID, *s));
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


GMSEC_Message CALL_TYPE mnemonicMessageCreateWithConfigAndSpec(const char* subject,
	                                                           const char* schemaID,
	                                                           GMSEC_Config config,
	                                                           GMSEC_Specification spec,
	                                                           GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Config*        cfg = reinterpret_cast<Config*>(config);
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	if (!cfg)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Config handle is NULL");
	}
	else if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(subject, schemaID, *cfg, *s));
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


GMSEC_Message CALL_TYPE mnemonicMessageCreateUsingData(const char* data, GMSEC_Status status)
{
	GMSEC_Message msg = NULL;
	Status        result;

	GMSEC_DISABLE_DEPRECATED_WARNINGS
	try
	{
		msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(data));
	}
	catch (Exception& e)
	{
		result = Status(e);
	}
	GMSEC_ENABLE_DEPRECATED_WARNINGS

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return msg;
}


GMSEC_Message CALL_TYPE mnemonicMessageCreateUsingSpecAndData(GMSEC_Specification spec, const char* data, GMSEC_Status status)
{
	GMSEC_Message  msg = NULL;
	Specification* s   = reinterpret_cast<Specification*>(spec);
	Status         result;

	if (!s)
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "Specification handle is NULL");
	}
	else
	{
		try
		{
			msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(*s, data));
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
			GMSEC_DISABLE_DEPRECATED_WARNINGS

			Mnemonic& tmp = const_cast<Mnemonic&>(mnemMsg->getMnemonic(index));

			GMSEC_ENABLE_DEPRECATED_WARNINGS

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


GMSEC_MnemonicIterator CALL_TYPE mnemonicMessageGetIterator(GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_MnemonicIterator iter = NULL;
	Status                 result;

	const Message*         tmpMsg  = reinterpret_cast<const Message*>(msg);
	const MnemonicMessage* mnemMsg = dynamic_cast<const MnemonicMessage*>(tmpMsg);

	if (mnemMsg)
	{
		MnemonicIterator& mnemIter = mnemMsg->getMnemonicIterator();

		iter = &mnemIter;
	}
	else
	{
		result = Status(MIST_ERROR, UNINITIALIZED_OBJECT, "MnemonicMessage handle is NULL or is not a MnemonicMessage");
	}

	if (status)
	{
		*(reinterpret_cast<Status*>(status)) = result;
	}

	return iter;
}


GMSEC_Message CALL_TYPE mnemonicMessageConvert(const GMSEC_Message msg, GMSEC_Status status)
{
	GMSEC_Message mnemonicMsg = NULL;
	Status        result;

	const Message* tmpMsg = reinterpret_cast<const Message*>(msg);

	if (tmpMsg)
	{
		GMSEC_DISABLE_DEPRECATED_WARNINGS
		MnemonicMessage tmpMnemonicMsg = MnemonicMessage::convertMessage(*tmpMsg);
		GMSEC_ENABLE_DEPRECATED_WARNINGS

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


static Specification buildSpecification(unsigned int version)
{
	std::ostringstream oss;
	oss << version;

	Config config;
	config.addValue("gmsec-specification-version", oss.str().c_str());

	return Specification(config);
}


static std::string buildSchemaID(const Specification& spec)
{
	std::ostringstream oss;
	oss << spec.getVersion();

	std::string schemaID = oss.str();

	schemaID.insert(4, ".");
	schemaID.append(".GMSEC.MSG.MVAL");

	return schemaID;
}


//
// DEPRECATED!
//
GMSEC_Message CALL_TYPE mnemonicMessageCreate(const char* subject, unsigned int version, GMSEC_Status status)
{
	GMSEC_WARNING << "This function has been deprecated; use the following function instead:\n"
	                 "mnemonicMessageCreateWithSpec(const char* subject, const char* schemaID, GMSEC_Specification spec, GMSEC_Status status)";

	GMSEC_Message msg = NULL;
	Status        result;

	try
	{
		Specification spec     = buildSpecification(version);
		std::string   schemaID = buildSchemaID(spec);

		msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(subject, schemaID.c_str(), spec));
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


//
// DEPRECATED!
//
GMSEC_Message CALL_TYPE mnemonicMessageCreateWithConfig(const char* subject, const GMSEC_Config config, unsigned int version, GMSEC_Status status)
{
	GMSEC_WARNING << "This function has been deprecated; use the following function instead:\n"
	                 "mnemonicMessageCreateWithConfigAndSpec(const char* subject, const char* schemaID, GMSEC_Config config, GMSEC_Specification spec, GMSEC_Status status)";

	GMSEC_Message msg = NULL;
	Status        result;

	const Config* cfg = reinterpret_cast<const Config*>(config);

	if (config)
	{
		try
		{
			Specification spec     = buildSpecification(version);
			std::string   schemaID = buildSchemaID(spec);

			msg = reinterpret_cast<GMSEC_Message>(new MnemonicMessage(subject, schemaID.c_str(), *cfg, spec));
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
