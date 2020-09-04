/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileMessage.cpp
 *
 * @brief This file contains the implementation for ProductFile messages.
 */


#include <gmsec4/mist/ProductFileMessage.h>

#include <gmsec4/mist/ProductFileIterator.h>

#include <gmsec4/internal/mist/InternalProductFileMessage.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


ProductFileMessage::ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version)
	: Message(subject, Message::PUBLISH),
	  m_internal(new InternalProductFileMessage(subject, responseStatus, productType, productSubtype, version))
{
	registerChild(m_internal);
}


ProductFileMessage::ProductFileMessage(const char* subject, const Config& config, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version)
	: Message(subject, Message::PUBLISH, config),
	  m_internal(new InternalProductFileMessage(subject, config, responseStatus, productType, productSubtype, version))
{
	registerChild(m_internal);
}


ProductFileMessage::ProductFileMessage(const ProductFileMessage& other)
	: Message(other),
	  m_internal(new InternalProductFileMessage(*(other.m_internal)))
{
	registerChild(m_internal);
}


ProductFileMessage::ProductFileMessage(const char* data)
	: Message(data),
	  m_internal(new InternalProductFileMessage(data))
{
	registerChild(m_internal);
}


ProductFileMessage& ProductFileMessage::operator=(const ProductFileMessage& other)
{
	if (this != &other)
	{
		delete m_internal;

		m_internal = new internal::InternalProductFileMessage(*(other.m_internal));

		registerChild(m_internal);
	}

	return *this;
}


ProductFileMessage::~ProductFileMessage()
{
	// Message parent class deletes m_internal, providing it's been registered.
}


void ProductFileMessage::addProductFile(const ProductFile& productFile)
{
	m_internal->addProductFile(productFile);
}


const ProductFile& ProductFileMessage::getProductFile(size_t index) const
{
	return m_internal->getProductFile(index);
}


size_t ProductFileMessage::getNumProductFiles() const
{
	return m_internal->getNumProductFiles();
}


ResponseStatus::Response ProductFileMessage::getResponseStatus() const
{
	return m_internal->getResponseStatus();
}


const char* ProductFileMessage::getProductType() const
{
	return m_internal->getProductType();
}


const char* ProductFileMessage::getProductSubtype() const
{
	return m_internal->getProductSubtype();
}


ProductFileIterator& ProductFileMessage::getProductFileIterator() const
{
	return m_internal->getProductFileIterator();
}


ProductFileMessage ProductFileMessage::convertMessage(const Message& message)
{
	ProductFileMessage product_file_message(message.toXML());

	return product_file_message;

}
