/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file InternalProductFileMessage.h
 *
 * @brief This file contains the interface to InternalProductFileMessage.
 */


#ifndef GMSEC_API_MESSAGE_INTERNAL_PRODUCT_FILE_MESSAGE_H
#define GMSEC_API_MESSAGE_INTERNAL_PRODUCT_FILE_MESSAGE_H

#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/internal/mist/message/InternalMistMessage.h>

#include <gmsec4/mist/Specification.h>

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/ProductFileIterator.h>

#include <vector>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace message
{
namespace internal
{

class GMSEC_API InternalProductFileMessage : public InternalMistMessage
{
public:
	InternalProductFileMessage(const char* subject, 
							   ResponseStatus::Response responseStatus, 
							   const char* schemaID,
							   const Specification& spec);

	InternalProductFileMessage(const char* subject, 
							   ResponseStatus::Response responseStatus, 
							   const char* schemaID,
							   const gmsec::api::Config& config, 
							   const Specification& spec);


	InternalProductFileMessage(const InternalProductFileMessage& other);


	InternalProductFileMessage(const char* data);


	InternalProductFileMessage(const Specification& spec, const char* data);


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

	void init(ResponseStatus::Response responseStatus, const char* schemaID);

	std::vector<ProductFile>              m_list;
	std::vector<ProductFile>::iterator    m_listIter;
	gmsec::api::mist::ProductFileIterator m_productFileIterator;
};

} // namespace internal
} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
