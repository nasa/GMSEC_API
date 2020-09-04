/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalProductFileMessage.h
 *
 * @brief This file contains the interface to InternalProductFileMessage.
 */


#ifndef GMSEC_API_INTERNAL_PRODUCT_FILE_MESSAGE_H
#define GMSEC_API_INTERNAL_PRODUCT_FILE_MESSAGE_H

#include <gmsec4/mist/ConnectionManager.h>
#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ProductFileIterator.h>
#include <gmsec4/mist/ProductFileMessage.h>

#include <gmsec4/internal/InternalMessage.h>

#include <vector>


namespace gmsec
{

namespace api
{

namespace mist
{

namespace internal
{

class GMSEC_API InternalProductFileMessage : public gmsec::api::internal::InternalMessage
{
public:
	InternalProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* prodType, const char* prodSubtype, unsigned int version);


	InternalProductFileMessage(const char* subject, const Config& config, ResponseStatus::Response responseStatus, const char* prodType, const char* prodSubtype, unsigned int version);


	InternalProductFileMessage(const InternalProductFileMessage& other);


	InternalProductFileMessage(const char* data);


	virtual ~InternalProductFileMessage();


	InternalProductFileMessage& CALL_TYPE operator=(const InternalProductFileMessage& other);


	void CALL_TYPE addProductFile(const ProductFile& productFile);


	const ProductFile& CALL_TYPE getProductFile(size_t index) const;


	size_t CALL_TYPE getNumProductFiles() const;


	ResponseStatus::Response CALL_TYPE getResponseStatus() const;


	const char* CALL_TYPE getProductType() const;


	const char* CALL_TYPE getProductSubtype() const;


	ProductFileIterator& getProductFileIterator();


	void resetProductFileIterator();


	bool hasNextProductFile() const;


	const ProductFile& nextProductFile();


private:
	ProductFile extractMessageProductFile(size_t index);

	void init(ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version);

	std::vector<ProductFile>           m_list;
	std::vector<ProductFile>::iterator m_listIter;
	ProductFileIterator                m_productFileIterator;

	ResponseStatus::Response           m_responseStatus;
	std::string                        m_productType;
	std::string                        m_productSubtype;
	unsigned int					   m_specVersion;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
