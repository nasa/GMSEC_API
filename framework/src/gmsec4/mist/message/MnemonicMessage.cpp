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


#include <gmsec4/mist/message/MnemonicMessage.h>

#include <gmsec4/internal/mist/message/InternalMnemonicMessage.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicIterator.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::message::internal;


MnemonicMessage::MnemonicMessage(const char* subject, const char* schemaID, const Specification& spec)
	: MistMessage(new InternalMnemonicMessage(subject, schemaID, spec))
{
}


MnemonicMessage::MnemonicMessage(const char* subject, const char* schemaID, const gmsec::api::Config& config, const Specification& spec)
	: MistMessage(new InternalMnemonicMessage(subject, schemaID, config, spec))
{
}


MnemonicMessage::MnemonicMessage(const MnemonicMessage& other)
	: MistMessage(new InternalMnemonicMessage(dynamic_cast<const InternalMnemonicMessage&>(other.getInternal())))
{
}


MnemonicMessage::MnemonicMessage(const char* data)
	: MistMessage(new InternalMnemonicMessage(data))
{
}


MnemonicMessage::MnemonicMessage(const Specification& spec, const char* data)
	: MistMessage(new InternalMnemonicMessage(spec, data))
{
}


MnemonicMessage& MnemonicMessage::operator=(const MnemonicMessage& other)
{
	if (this != &other)
	{
		this->registerInternal(new InternalMnemonicMessage(dynamic_cast<const InternalMnemonicMessage&>(other.getInternal())));
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


MnemonicIterator& MnemonicMessage::getMnemonicIterator() const
{
	return dynamic_cast<InternalMnemonicMessage&>(getInternal()).getMnemonicIterator();
}


MnemonicMessage MnemonicMessage::convertMessage(const Message& message)
{
	GMSEC_DISABLE_DEPRECATED_WARNINGS
	return MnemonicMessage(message.toXML());
	GMSEC_ENABLE_DEPRECATED_WARNINGS
}
