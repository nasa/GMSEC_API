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


#include <gmsec4/mist/message/ProductFileMessage.h>

#include <gmsec4/internal/mist/message/InternalProductFileMessage.h>

#include <gmsec4/mist/ProductFileIterator.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::message::internal;


ProductFileMessage::ProductFileMessage(const char* subject, 
									   ResponseStatus::Response responseStatus, 
									   Message::MessageKind kind,
									   const char* productType, 
									   const char* productSubtype, 
									   const Specification& spec)
	: MistMessage(new InternalProductFileMessage(subject,
	                                             responseStatus,
	                                             InternalMistMessage::buildSchemaID(kind, "PROD", (spec.getVersion() >= GMSEC_ISD_2018_00 ? NULL : productType), spec.getVersion()).c_str(),
	                                             spec))
{
	if (spec.getVersion() >= GMSEC_ISD_2018_00)
	{
		this->addField("PROD-TYPE", productType);
		this->addField("PROD-SUBTYPE", productSubtype);
	}
}


ProductFileMessage::ProductFileMessage(const char* subject, 
									   ResponseStatus::Response responseStatus, 
									   Message::MessageKind kind,
									   const char* productType, 
									   const char* productSubtype, 
									   const Config& config, 
									   const Specification& spec)
	: MistMessage(new InternalProductFileMessage(subject,
	                                             responseStatus,
	                                             InternalMistMessage::buildSchemaID(kind, "PROD", (spec.getVersion() >= GMSEC_ISD_2018_00 ? NULL : productType), spec.getVersion()).c_str(),
	                                             config,
	                                             spec))
{
	if (spec.getVersion() >= GMSEC_ISD_2018_00)
	{
		this->addField("PROD-TYPE", productType);
		this->addField("PROD-SUBTYPE", productSubtype);
	}
}


ProductFileMessage::ProductFileMessage(const char* subject, 
									   ResponseStatus::Response responseStatus, 
									   const char* schemaID, 
									   const Specification &spec)
	: MistMessage(new InternalProductFileMessage(subject, responseStatus, schemaID, spec))
{
}


ProductFileMessage::ProductFileMessage(const char* subject, 
									   ResponseStatus::Response responseStatus, 
									   const char* schemaID, 
									   const Config& config, 
									   const Specification &spec)
	: MistMessage(new InternalProductFileMessage(subject, responseStatus, schemaID, config, spec))
{
}


ProductFileMessage::ProductFileMessage(const ProductFileMessage& other)
	: MistMessage(new InternalProductFileMessage(dynamic_cast<const InternalProductFileMessage&>(other.getInternal())))
{
}


ProductFileMessage::ProductFileMessage(const char* data)
	: MistMessage(new InternalProductFileMessage(data))
{
}


ProductFileMessage::ProductFileMessage(const Specification& spec, const char* data)
	: MistMessage(new InternalProductFileMessage(spec, data))
{
}


ProductFileMessage& ProductFileMessage::operator=(const ProductFileMessage& other)
{
	if (this != &other)
	{
		this->registerInternal(new InternalProductFileMessage(dynamic_cast<const InternalProductFileMessage&>(other.getInternal())));
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


ProductFileIterator& ProductFileMessage::getProductFileIterator() const
{
	return dynamic_cast<InternalProductFileMessage&>(getInternal()).getProductFileIterator();
}


ProductFileMessage ProductFileMessage::convertMessage(const Message& message)
{
	GMSEC_DISABLE_DEPRECATED_WARNINGS
	return ProductFileMessage(message.toXML());
	GMSEC_ENABLE_DEPRECATED_WARNINGS
}
