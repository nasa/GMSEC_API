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


#ifndef GMSEC_API_MESSAGE_PRODUCT_FILE_MESSAGE_H
#define GMSEC_API_MESSAGE_PRODUCT_FILE_MESSAGE_H

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/mist/ProductFile.h>
#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/Deprecated.h>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Config;

namespace mist
{
	class ProductFileIterator;
	class Specification;

namespace message
{
	// Forward declaration(s)
	namespace internal
	{
		class InternalProductFileMessage;
	}


/**
 * @class ProductFileMessage
 *
 * @brief A MistMessage object that is capable of storing ProductFile objects
 *
 * @sa Message @n
 *	   MistMessage @n
 *     Config @n
 *	   Specification @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API ProductFileMessage : public MistMessage
{
public:
	/**
	 * @fn ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, Message::MessageKind kind, const char* productType, const char* productSubtype, const Specification& spec)
	 *
	 * @brief constructor - Initializes the message instance and automatically builds a schema ID based on params given.
	 *
	 * @param subject - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param kind - the kind of message to instantiate.
	 * @param productType - PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype - PROD-SUBTYPE field to insert into a MSG PROD message.
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * 
	 * @throw An exception is thrown if Specification fails to load the template directory or if a valid schemaID cannot be created from the given params.
	 */
	ProductFileMessage(const char* subject, 
					   ResponseStatus::Response responseStatus, 
					   Message::MessageKind kind,
					   const char* productType, 
					   const char* productSubtype, 
					   const Specification& spec);


	/**
	 * @fn ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, Message::MessageKind kind, const char* productType, const char* productSubtype, const Config& config, const Specification& spec)
	 *
	 * @brief constructor - Initializes the message instance and associates a Configuration object and automatically builds a schema ID based on params given.
	 *
	 * @param subject - The subject string for the message.
	 * @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	 * @param kind - the kind of message to instantiate.
	 * @param productType - PROD-TYPE field to insert into a MSG PROD message.
	 * @param productSubtype - PROD-SUBTYPE field to insert into a MSG PROD message.
	 * @param config  - A configuration to associate with the message.
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 *
	 * @throw An exception is thrown if Specification fails to load the template directory or if a valid schemaID cannot be created from the given params.
	 */
	ProductFileMessage(const char* subject, 
					   ResponseStatus::Response responseStatus, 
					   Message::MessageKind kind,
					   const char* productType, 
					   const char* productSubtype, 
					   const gmsec::api::Config& config, 
					   const Specification& spec);

   /**
    * @fn ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* schemaID, const Specification& spec)
	* 
	* @brief constructor - Initializes the message with a given schema ID
	*
	* @param subject - The subject string for the message.
	* @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	* @param schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	* format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.PROD)
	* You may also use the shorthand notation of messageKind.messageType.messageSubType (e.g. MSG.PROD)
	* @param spec - A reference to the specification this message's schema will adhere to.
	* 
	* @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
    */
	ProductFileMessage(const char* subject, 
				   ResponseStatus::Response responseStatus, 
				   const char* schemaID,
				   const Specification& spec);

   /**
    * @fn ProductFileMessage(const char* subject, ResponseStatus::Response responseStatus, const char* schemaID, const gmsec::api::Config& config, const Specification& spec)
	* 
	* @brief constructor - Initializes the message with a given schema ID
	*
	* @param subject - The subject string for the message.
	* @param responseStatus - RESPONSE-STATUS field to insert into a MSG PROD message.
	* @param schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	* format of: major.minor.schemaLevelName.messageKind.messageType.messageSubtype (e.g. 2019.00.C2MS.MSG.PROD)
	* You may also use the shorthand notation of messageKind.messageType.messageSubType (e.g. MSG.PROD)
	* @param config  - A configuration to associate with the message.
	* @param spec - A reference to the specification this message's schema will adhere to.
	*
	* @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
    */
	ProductFileMessage(const char* subject, 
				   ResponseStatus::Response responseStatus, 
				   const char* schemaID,
				   const gmsec::api::Config& config, 
				   const Specification& spec);

	/**
	 * @fn ProductFileMessage(const ProductFileMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given ProductFileMessage
	 *
	 * @param other - the other ProductFileMessage object to copy.
	 */
	ProductFileMessage(const ProductFileMessage& other);


	/**
	 * @fn ProductFileMessage(const char* data)
	 *
	 * @brief Initializes a ProductFileMessage using an XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize ProductFileMessage.
	 */
	GMSEC_DEPRECATED ProductFileMessage(const char* data);


	/**
	 * @fn ProductFileMessage(const Specification& spec, const char* data)
	 *
	 * @brief Initializes a Product File Message using the given XML or JSON data representation of the message,
	 * and ensures the message adheres to the provided specification.
	 *
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * @param data - XML or JSON string used to initialize the message
	 *
	 * @throw An Exception is thrown if the given data does not represent a valid XML or JSON statement.
	 * @throw An Exception is thrown if the given data does not represent a Product File Message.
	 */
	ProductFileMessage(const Specification& spec, const char* data);


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
	 * @brief Get ProductFile numbered at index. Note that the index which will be
	 * retrieved does not correspond to the GMSEC ISD and starts from 0 instead of 1.
	 * For example, getProductFile(0) would return the ProductFile corresponding to
	 * FILE.1.
	 *
	 * @return ProductFile at specified index inside of ProductFileMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the
	 *        range of ProductFiles in this message.
	 *
	 * @note This function has been deprecated, use ProductFileIterator instead.
	 */
	GMSEC_DEPRECATED const ProductFile& CALL_TYPE getProductFile(size_t index) const;


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
	ProductFileIterator& CALL_TYPE getProductFileIterator() const;


	/**
	 * @fn ProductFileMessage convertMessage(const Message& message)
	 *
	 * @brief Constructs and returns a MnemonicMessage from an an ordinary message.
	 */
	static ProductFileMessage CALL_TYPE convertMessage(const Message& message);
};

} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
