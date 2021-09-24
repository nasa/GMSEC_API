/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicMessage.cpp
 *
 * @brief This file contains the implementation of Mnemonic messages.
 */


#include <gmsec4/mist/MnemonicMessage.h>

#include <gmsec4/internal/mist/message/InternalMnemonicMessage.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicIterator.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


MnemonicMessage::MnemonicMessage(const char* subject, unsigned int version)
	: Message(new InternalMnemonicMessage(subject,
	                                      InternalMistMessage::buildSchemaID(PUBLISH, "MVAL", NULL, version).c_str(),
	                                      InternalMistMessage::buildSpecification(version)))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::MnemonicMessage instead.";
}


MnemonicMessage::MnemonicMessage(const char* subject, const Config& config, unsigned int version)
	: Message(new InternalMnemonicMessage(subject,
	                                      InternalMistMessage::buildSchemaID(PUBLISH, "MVAL", NULL, version).c_str(),
	                                      config,
	                                      InternalMistMessage::buildSpecification(version)))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::MnemonicMessage instead.";
}


MnemonicMessage::MnemonicMessage(const MnemonicMessage& other)
	: Message(new InternalMnemonicMessage(dynamic_cast<InternalMnemonicMessage&>(other.getInternal())))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::MnemonicMessage instead.";
}


MnemonicMessage::MnemonicMessage(const char* data)
	: Message(new InternalMnemonicMessage(data))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::MnemonicMessage instead.";
}


MnemonicMessage& MnemonicMessage::operator=(const MnemonicMessage& other)
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::MnemonicMessage instead.";

	if (this != &other)
	{
		this->registerInternal(new InternalMnemonicMessage(dynamic_cast<InternalMnemonicMessage&>(other.getInternal())));
	}

	return *this;
}


MnemonicMessage::~MnemonicMessage()
{
}


void MnemonicMessage::addMnemonic(const Mnemonic& mnemonic)
{
	dynamic_cast<InternalMnemonicMessage&>(getInternal()).addMnemonic(mnemonic);
}


const Mnemonic& MnemonicMessage::getMnemonic(size_t index) const
{
	return dynamic_cast<InternalMnemonicMessage&>(getInternal()).getMnemonic(index);
}


size_t MnemonicMessage::getNumMnemonics() const
{
	return dynamic_cast<InternalMnemonicMessage&>(getInternal()).getNumMnemonics();
}


gmsec::api::mist::MnemonicIterator& MnemonicMessage::getMnemonicIterator() const
{
	return dynamic_cast<InternalMnemonicMessage&>(getInternal()).getMnemonicIterator();
}


MnemonicMessage MnemonicMessage::convertMessage(const Message& message)
{
	GMSEC_DISABLE_DEPRECATED_WARNINGS
	return MnemonicMessage(message.toXML());
	GMSEC_ENABLE_DEPRECATED_WARNINGS
}
