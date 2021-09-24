/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MistMessage.h
 *
 * @brief This file contains the public interface to generic MIST Messages.
 */

#ifndef GMSEC_API_MIST_MESSAGE_H
#define GMSEC_API_MIST_MESSAGE_H

#include <gmsec4/Message.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/Deprecated.h>

namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace mist
{
	// Forward declaration(s)
	class Specification;

namespace message
{
	// Forward declaration(s)
	namespace internal
	{
		class InternalMistMessage;
	}


/**
 * @class MistMessage
 *
 * @brief This Message object shares all of the functionality of standard messages,
 * but can also be auto-populated based on how the message schema identifies itself within
 * the given specification.
 *
 * @sa Message @n
 *     Config @n
 *	   Specification @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API MistMessage : public Message
{
public:
   /**
	* @fn MistMessage(const char* subject, const char* schemaID, const Specification& spec)
	*
	* @brief default constructor - Initializes the message instance with a template determined by ID and spec.
	*
	* @param subject - the subject string for the message.
	* @param schemaID - the string used to identify the message schema in C2MS or other message specification
	* (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	* 
	* @param spec - A reference to the specification this message's schema will adhere to.
	*
	* @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	*/
	MistMessage(const char* subject, const char* schemaID, const Specification& spec);


   /**
	* @fn MistMessage(const char* subject, const char* schemaID, const Config& config, const Specification& spec)
	*
	* @brief default constructor - Initializes the message instance with a template determined by ID, spec, and config.
	*
	* @param subject - the subject string for the message.
	* @param schemaID - the string used to identify the message schema in C2MS or other message specification
	* (e.g. 2019.00.C2MS.MSG.HB or MSG.HB).
	* @param config - A configuration to associate with the message.
	* @param spec - A reference to the specification this message's schema will adhere to.
	*
	* @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	*/
	MistMessage(const char* subject, const char* schemaID, const Config& config, const Specification& spec);


   /**
	* @fn MistMessage(const MistMessage& other)
	*
	* @brief Copy constructor - initializes the message instance using the other given MistMessage
	*
	* @param other - The message to be copied
	*
	* @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	*/
	MistMessage(const MistMessage& other);


	/**
	 * @fn MistMessage(const Message& msg)
	 *
	 * @brief Special constructor that constructs a MistMessage using a simple Message.
	 *
	 * @param msg - The simple Message to reference while building the MistMessage
	 *
	 * @throw An exception is thrown if the given Message does not house sufficient information to construct a MistMessage.
	 */
	MistMessage(const Message& msg);


	/**
	 * @fn MistMessage(const Message& msg, const Config& specConfig)
	 *
	 * @brief Special constructor that builds a MistMessage using a simple Message.
	 *
	 * @param msg - The simple Message to reference while building the MistMessage
	 * @param specConfig - The specification configuration to apply when constructing the MistMessage
	 *
	 * @throw An exception is thrown if the given Message does not house sufficient information to construct a MistMessage.
	 */
	MistMessage(const Message& msg, const Config& specConfig);


   /**
	* @fn MistMessage(const char* data)
	*
	* @brief Initializes a MistMessage using an XML or JSON string
	*
	* @param data - XML or JSON string used to initialize the message
	*
	* @throw An exception is thrown if the given data does not represent a valid XML or JSON statement.
	*
	* @note This method has been deprecated. Use MistMessage(const Specification&, const char*) instead.
	*/
	GMSEC_DEPRECATED MistMessage(const char* data);


   /**
	* @fn MistMessage(const Specification& spec, const char* data)
	*
	* @brief Initializes a MistMessage using the given XML or JSON data representation of the message, and ensures
	* the message adheres to the provided specification.
	*
	* @param spec - A reference to the specification this message's schema will adhere to.
	* @param data - XML or JSON string used to initialize the message
	*
	* @throw An exception is thrown if the given data does not represent a valid XML or JSON statement.
	*/
	MistMessage(const Specification& spec, const char* data);


   /**
    * @fn ~MistMessage()
	*
	* @brief Destructor - cleans up the message instance
	*/
	virtual ~MistMessage();


	/**
	 * @fn void setStandardFields(const gmsec::api::util::DataList<Field*>& standardFields)
	 *
	 * @brief Sets the internal list of fields which are to be automatically placed in all
	 * MistMessage objects that are created.  Internal copies of the Fields are made, and
	 * thus ownership of the fields that are provided are not retained by MistMessage.
	 *
	 * @param standardFields - A list of fields to be copied to the internal set of fields,
	 * which will in turn be included with all MistMessage objects that are created.
	 *
	 * @sa clearStandardFields()
	 */
	static void CALL_TYPE setStandardFields(const gmsec::api::util::DataList<Field*>& standardFields);


	/**
	 * @fn void clearStandardFields()
	 *
	 * @brief Destroys the lists of standard fields that are included with MistMessage objects.
	 *
	 * @sa setStandardFields()
	 */
	static void clearStandardFields();


	/**
	 * @fn getSchemaID()
	 *
	 * @brief Returns a string that identifies the schema that the Message is based off of.
	 */
	const char* CALL_TYPE getSchemaID() const;


   /**
    * @fn setValue(const char* fieldName, const char* value)
	*
	* @brief Sets the value for a field indicated by user.  The value's type is automatically
	* determined by the function depending on the message schema being used.  If the field 
	* does not have a required type, a new StringField will be created with the specified name 
	* and value.
	* 
	* @param fieldName - name of the field to be modified/created
	* @param value - the value of the field.  The value's type is automatically determined
	* based on the message schema.
	*
	* @throws An exception is thrown if the field name is NULL or contains an empty string.
	* @throws An exception is thrown if the input value's type cannot be converted into the type required
	* by the field, or if the string is too big when converting to char.
	*/
	void CALL_TYPE setValue(const char* fieldName, const char* value);


   /**
    * @fn setValue(const char* fieldName, GMSEC_I64 value)
	*
	* @brief Sets the value for a field indicated by user.  The value's type is automatically
	* determined by the function depending on the message schema being used.  If the field 
	* does not have a required type, a new I64Field will be created with the specified name 
	* and value.
	* 
	* @param fieldName - name of the field to be modified/created
	* @param value - the value of the field.  The value's type is automatically determined
	* based on the message schema.
	*
	* @throws An exception is thrown if the field name is NULL or contains an empty string.
	* @throws An exception is thrown if the input value's type cannot be converted into the type required
	* by the field, or if the string is too big when converting to char.
	*/
	void CALL_TYPE setValue(const char* fieldName, GMSEC_I64 value);


   /**
    * @fn setValue(const char* fieldName, GMSEC_F64 value)
	*
	* @brief Sets the value for a field indicated by user.  The value's type is automatically
	* determined by the function depending on the message schema being used.  If the field 
	* does not have a required type, a new F64Field will be created with the specified name 
	* and value.
	* 
	* @param fieldName - name of the field to be modified/created
	* @param value - the value of the field.  The value's type is automatically determined
	* based on the message schema.
	*
	* @throws An exception is thrown if the field name is NULL or contains an empty string.
	* @throws An exception is thrown if the input value's type cannot be converted into the type required
	* by the field, or if the string is too big when converting to char.
	*/
	void CALL_TYPE setValue(const char* fieldName, GMSEC_F64 value);


protected:
	MistMessage(gmsec::api::mist::message::internal::InternalMistMessage* internalMsg);


private:
	// Defined, but not implemented
	MistMessage& operator=(const MistMessage&);
};

} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
