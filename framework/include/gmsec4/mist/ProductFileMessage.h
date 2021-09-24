/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ProductFileMessage.h
 *
 * @brief This file contains the public interface to ProductFile messages.
 */


#ifndef GMSEC_API_PRODUCT_FILE_MESSAGE_H
#define GMSEC_API_PRODUCT_FILE_MESSAGE_H

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/Message.h>

#include <gmsec4/util/Deprecated.h>


namespace gmsec
{

namespace api
{
	class Config;

namespace mist
{
	class ProductFileIterator;

namespace message
{
	namespace internal
	{
		class InternalProductFileMessage;
	}
}

/**
 * @class ProductFileMessage
 *
 * @brief The Message object contains a GMSEC MSG PROD message.
 * The following message schema IDs and their templates are supported: @n
 * 2014.00.GMSEC.MSG.PROD.AAA @n
 * 2014.00.GMSEC.MSG.PROD.AUTO @n
 * 2014.00.GMSEC.MSG.PROD.FD @n
 * 2014.00.GMSEC.MSG.PROD.MAS @n
 * 2014.00.GMSEC.MSG.PROD.PAS @n
 * 2014.00.GMSEC.MSG.PROD.SC @n
 * 2014.00.GMSEC.MSG.PROD.TAC @n
 * 2016.00.GMSEC.MSG.PROD.AAA @n
 * 2016.00.GMSEC.MSG.PROD.AUTO @n
 * 2016.00.GMSEC.MSG.PROD.FD @n
 * 2016.00.GMSEC.MSG.PROD.MAS @n
 * 2016.00.GMSEC.MSG.PROD.PAS @n
 * 2016.00.GMSEC.MSG.PROD.SC @n
 * 2016.00.GMSEC.MSG.PROD.TAC @n
 *
 * @note gmsec::api::mist::ProductFileMessage has been deprecated.  It is recommended to use gmsec::api::mist::message::ProductFileMessage
 *
 * @sa Message @n
 *     Config @n
 *     Connection @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API ProductFileMessage : public Message
{
public:
	/**
	 * @fn ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version)
	 *
	 * @brief Default constructor - Initializes the message instance
	 *
	 * @param subject - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to indert into a MSG PROD message.
	 * @param productType - PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype - PROD-SUBTYPE field to indert into a MSG PROD message.
	 * @param version - The version of the GMSEC message specification to be used.
	 */
	GMSEC_DEPRECATED ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version);


	/**
	 * @fn ProductFileMessage(const char* subject, const Config& config, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version)
	 *
	 * @brief constructor - Initializes the message instance and associates a Configuration object
	 *
	 * @param subject - The subject string for the message.
	 * @param config  - A configuration to associate with the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param productType - PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype - PROD-SUBTYPE field to indert into a MSG PROD message.
	 * @param version - The version of the GMSEC message specification to be used.
	 */
	GMSEC_DEPRECATED ProductFileMessage(const char* subject, const Config& config, ResponseStatus::Response responseStatus, const char* productType, const char* productSubtype, unsigned int version);


	/**
	 * @fn ProductFileMessage(const ProductFileMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given ProductFileMessage
	 *
	 * @param other - the other ProductFileMessage object to copy.
	 */
	GMSEC_DEPRECATED ProductFileMessage(const ProductFileMessage& other);


	/**
	 * @fn ProductFileMessage(const char* data)
	 *
	 * @brief Initializes a ProductFileMessage using an XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize ProductFileMessage.
	 */
	GMSEC_DEPRECATED ProductFileMessage(const char* data);


	/**
	 * @fn ~ProductFileMessage()
	 *
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~ProductFileMessage();


	/**
	 * @fn ProductFileMessage& operator=(const ProductFileMessage& other)
	 *
	 * @brief Overloaded assignment operator method for making a copy of other ProductFileMessage object
	 *
	 * @param other - the other ProductFileMessage object to copy.
	 *
	 * @return A copy of the other ProductFileMessage object that was given.
	 */
	ProductFileMessage& CALL_TYPE operator=(const ProductFileMessage& other);


	/**
	 * @fn void addProductFile(const ProductFile& productFile)
	 *
	 * @brief Add a product file to the message.
	 */
	void CALL_TYPE addProductFile(const ProductFile& productFile);


	/**
	 * @fn const ProductFile getProductFile(size_t index) const
	 *
	 * @brief Get ProductFile numbered at index.
	 *
	 * @return ProductFile at specified index inside of ProductFileMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the
	 *        range of ProductFiles in this message.
	 */
	const ProductFile& CALL_TYPE getProductFile(size_t index) const;


	/**
	 * @fn size_t getNumProductFiles() const
	 *
	 * @brief Get the number of ProductFiles in this message.
	 *
	 * @return Return the number of ProductFiles in the message.
	 */
	size_t CALL_TYPE getNumProductFiles() const;


	/**
	 * @fn ResponseStatus::Response getResponseStatus() const
	 *
	 * @brief Get the response status of this ProductFile message.
	 */
	ResponseStatus::Response CALL_TYPE getResponseStatus() const;


	/**
	 * @fn const char* getProductType() const
	 *
	 * @brief Get the PROD-TYPE of this ProductFile message.
	 */
	const char* CALL_TYPE getProductType() const;


	/**
	 * @fn const char* getProductSubtype() const
	 *
	 * @brief Get the PROD-SUBTYPE of this ProductFile message.
	 */
	const char* CALL_TYPE getProductSubtype() const;


	/**
	 * @fn ProductFileIterator& getProductFileIterator() const
	 *
	 * @desc Method that allows the callee to get the ProductFileIterator associated with the %ProductFileMessage.
	 * This iterator will allow for applications to iterate over the ProductFile objects stored within the %ProductFileMessage.
	 * The iterator is reset each time getProductFileIterator() is called.  The iterator itself is destroyed when the
	 * %ProductFileMessage object is destroyed.
	 *
	 * @note Only one ProductFileIterator object is associated with a %ProductFileMessage object; multiple calls to
	 * getProductFileIterator() will return a reference to the same ProductFileIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a ProductFileIterator object.
	 */
	gmsec::api::mist::ProductFileIterator& CALL_TYPE getProductFileIterator() const;


	/**
	 * @fn ProductFileMessage convertMessage(const Message& message)
	 *
	 * @brief Constructs and returns a MnemonicMessage from an an ordinary message.
	 */
	GMSEC_DEPRECATED static ProductFileMessage CALL_TYPE convertMessage(const Message& message);


private:
	// Defined so as to preserve binary compatibility with API 4.1
	void* filler;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
