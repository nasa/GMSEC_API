/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4/internal/mist/message/InternalProductFileMessage.h>

#include <gmsec4/mist/ProductFileIterator.h>
#include <gmsec4/mist/Specification.h>

#include <gmsec4/Config.h>

#include <gmsec4/util/Log.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message::internal;


ProductFileMessage::ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version)
	: Message(new InternalProductFileMessage(subject,
	                                         responseStatus, 
	                                         InternalMistMessage::buildSchemaID(PUBLISH, "PROD", productType, version).c_str(),
	                                         InternalMistMessage::buildSpecification(version)))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::ProductFileMessage instead.";
}


ProductFileMessage::ProductFileMessage(const char* subject, const Config& config, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version)
	: Message(new InternalProductFileMessage(subject,
	                                         responseStatus, 
	                                         InternalMistMessage::buildSchemaID(PUBLISH, "PROD", productType, version).c_str(),
	                                         config,
	                                         InternalMistMessage::buildSpecification(version)))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::ProductFileMessage instead.";
}


ProductFileMessage::ProductFileMessage(const ProductFileMessage& other)
	: Message(new InternalProductFileMessage(dynamic_cast<InternalProductFileMessage&>(other.getInternal())))
{
}


ProductFileMessage::ProductFileMessage(const char* data)
	: Message(new InternalProductFileMessage(data))
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::ProductFileMessage instead.";
}


ProductFileMessage& ProductFileMessage::operator=(const ProductFileMessage& other)
{
	GMSEC_WARNING << "This class has been deprecated; use gmsec::api::mist::message::ProductFileMessage instead.";

	if (this != &other)
	{
		this->registerInternal(new InternalProductFileMessage(dynamic_cast<InternalProductFileMessage&>(other.getInternal())));
	}

	return *this;
}


ProductFileMessage::~ProductFileMessage()
{
}


void ProductFileMessage::addProductFile(const ProductFile& productFile)
{
	dynamic_cast<InternalProductFileMessage&>(getInternal()).addProductFile(productFile);
}


const ProductFile& ProductFileMessage::getProductFile(size_t index) const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getProductFile(index);
}


size_t ProductFileMessage::getNumProductFiles() const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getNumProductFiles();
}


ResponseStatus::Response ProductFileMessage::getResponseStatus() const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getResponseStatus();
}


const char* ProductFileMessage::getProductType() const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getProductType();
}


const char* ProductFileMessage::getProductSubtype() const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getProductSubtype();
}


gmsec::api::mist::ProductFileIterator& ProductFileMessage::getProductFileIterator() const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getProductFileIterator();
}


ProductFileMessage ProductFileMessage::convertMessage(const Message& message)
{
	GMSEC_DISABLE_DEPRECATED_WARNINGS
	return ProductFileMessage(message.toXML());
	GMSEC_ENABLE_DEPRECATED_WARNINGS
}
