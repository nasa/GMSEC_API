/*
 * Copyright 2007-2016 United States Government as represented by the
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

#include <gmsec4/internal/mist/InternalMnemonicMessage.h>

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/mist/MnemonicIterator.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


MnemonicMessage::MnemonicMessage(const char* subject, unsigned int version)
	: Message(subject, Message::PUBLISH),
	  m_internal(new InternalMnemonicMessage(subject, version))
{
	registerChild(m_internal);
}


MnemonicMessage::MnemonicMessage(const char* subject, const Config& config, unsigned int version)
	: Message(subject, Message::PUBLISH, config),
	  m_internal(new InternalMnemonicMessage(subject, config, version))
{
	registerChild(m_internal);
}


MnemonicMessage::MnemonicMessage(const MnemonicMessage& other)
	: Message(other),
	  m_internal(new InternalMnemonicMessage(*(other.m_internal)))
{
	registerChild(m_internal);
}


MnemonicMessage::MnemonicMessage(const char* data)
	: Message(data),
	  m_internal(new InternalMnemonicMessage(data))
{
	registerChild(m_internal);
}


MnemonicMessage& MnemonicMessage::operator=(const MnemonicMessage& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new internal::InternalMnemonicMessage(*(other.m_internal));

		registerChild(m_internal);
	}

	return *this;
}


MnemonicMessage::~MnemonicMessage()
{
	// Message parent class deletes m_internal, providing it's been registered.
}


void MnemonicMessage::addMnemonic(const Mnemonic& mnemonic)
{
	m_internal->addMnemonic(mnemonic);
}


const Mnemonic& MnemonicMessage::getMnemonic(size_t index) const
{
	return m_internal->getMnemonic(index);
}


size_t MnemonicMessage::getNumMnemonics() const
{
	return m_internal->getNumMnemonics();
}


MnemonicIterator& MnemonicMessage::getMnemonicIterator() const
{
	return m_internal->getMnemonicIterator();
}


MnemonicMessage MnemonicMessage::convertMessage(const Message& message)
{
	MnemonicMessage mnemonic_message(message.toXML());

	return mnemonic_message;
}
