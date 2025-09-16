/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.h
 *
 * @brief This file contains the public interface for a Message.
 */

#ifndef GMSEC_API5_MESSAGE_H
#define GMSEC_API5_MESSAGE_H

#include <gmsec5/MessageFieldIterator.h>

#include <gmsec5/Fields.h>
#include <gmsec5/Specification.h>
#include <gmsec5/Status.h>

#include <gmsec5/c/message.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>

#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api5
{
	class Config;
	class MessageValidator;

namespace internal
{
	class InternalMessage;
	class MessageBuddy;
}

/**
 * @class Message
 *
 * @brief This Message object is auto-populated based on how the message
 * schema identifies itself within the given specification.
 *
 * @sa Config
 * @sa Field
 * @sa MessageFieldIterator
 */
class GMSEC_API Message
{
public:
	/**
	 * @enum Kind
	 *
	 * @brief Allowable type of messages that can be created.
	 *
	 * @note The message kind is auto-deduced using the MESSAGE-TYPE field;
	 * if the field is not present in the message, or does not contain a
	 * value of MSG, REQ, or RESP, then the message kind will default to
	 * PUBLISH.
	 */
	enum class Kind
	{
		PUBLISH = 1, ///< For messages that will be published
		REQUEST = 2, ///< For request messages
		REPLY   = 3  ///< For reply messages
	};


	/**
	 * @enum ResponseStatus
	 *
	 * @brief Possible values for the a GMSEC Message's Response to a request.
	 */
	enum class ResponseStatus
	{
		ACKNOWLEDGEMENT = 1,        ///< Basic acknowledgement
		WORKING_KEEP_ALIVE = 2,     ///< Still working on operation
		SUCCESSFUL_COMPLETION = 3,  ///< Operation successfully completed
		FAILED_COMPLETION = 4,      ///< Operation failed to complete
		INVALID_REQUEST = 5,        ///< The request for an operation was invalid
		FINAL_MESSAGE = 6           ///< This message is the last in a set of responses
	};


	/**
	 * @fn Message()
	 *
	 * @brief Constructor to create a message containing no fields.
	 *
	 * @sa addField()
	 * @sa addFields()
	 */
	Message();


	/**
	 * @fn Message(const Message& other)
	 *
	 * @brief Copy constructor.
	 *
	 * @param other - The message to be copied.
	 */
	Message(const Message& other);


	/**
	 * @fn ~Message()
	 *
	 * @brief Destructor.
	 */
	virtual ~Message();


	/**
	 * @fn Message& operator=(const Message& other)
	 *
	 * @brief Assignment operator for copying a %Message object.
	 *
	 * @param other - The %Message object to copy.
	 */
	Message& CALL_TYPE operator=(const Message& other);


	/**
	 * @fn void acknowledge() const
	 *
	 * @brief Acknowledges the processing of the message that has been received
	 * from the GMSEC Bus.
	 *
	 * @note Some middleware can retain a delivered message in a queue until said time
	 * the message has been acknowledged as being processed. Should the message not be
	 * acknowledged, the middleware can deliver the message to another subscriber client.
	 *
	 * @see Connection::receive()
	 * @see Connection::request()
	 */
	void CALL_TYPE acknowledge() const;


	/**
	 * @fn static void destroy(Message*& msg)
	 *
	 * @brief Destroys a Message object.
	 * This method should NOT be used to destroy a Message object received within a callback.
	 *
	 * @param msg - The %Message object to be destroyed.
	 *
	 * @see Connection::receive()
	 * @see Connection::request()
	 */
	static void CALL_TYPE destroy(Message*& msg);


	/**
	 * @fn getSchemaID()
	 *
	 * @brief Returns a string that identifies the schema from which the Message is based.
	 */
	const char* CALL_TYPE getSchemaID() const;


	/**
	 * @fn unsigned int getVersion() const
	 *
	 * @brief Returns the specification version number (if known) that was used to create the message.
	 */
	unsigned int CALL_TYPE getVersion() const;


	/**
	 * @fn Specification::SchemaLevel getSchemaLevel() const
	 *
	 * @brief Returns the specification schema level (if known) that was used to create the message.
	 */
	Specification::SchemaLevel CALL_TYPE getSchemaLevel() const;


	/**
	 * @fn Status isCompliant() const
	 *
	 * @brief The contents of the message are compared to the template to ensure the
	 * message is in compliance with the message specification.
	 *
	 * @return A Status object to indicate whether the message is valid. If invalid,
	 * a list of errors will be provided of issues found within the message will be
	 * reported.
	 *
	 * @sa Status
	 */
	Status CALL_TYPE isCompliant() const;


	/**
	 * @fn void registerMessageValidator(MessageValidator* validator)
	 *
	 * @brief Registers the given message validator that is used when the
	 * message is checked for compliance.
	 *
	 * @param validator - A custom message validation object.
	 *
	 * @throw A GmsecException is thrown if the validator object is NULL.
	 *
	 * @sa MessageValidator
	 * @sa MessageFactory::registerMessageValidator()
	 */
	void CALL_TYPE registerMessageValidator(MessageValidator* validator);


	/* @cond For C API support ONLY! */
	void CALL_TYPE registerMessageValidator(GMSEC_MessageValidator* validator);
	/* @endcond */


	/**
	 * @fn bool setFieldValue(const char* fieldName, const char* value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required
	 * by the field, or if the string is too big when converting to char.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, const char* value);


	/** * @fn bool setFieldValue(const char* fieldName, const char* value, bool convert) 
	* 
	* @brief Adds/replaces a field with the given name and value to the message. If 
	* a field template for the schema in use is available for the named field, then 
	* the value's type will be coerced to match that which is described in the field 
	* template. 
	* 
	* @note This method is not very efficient; if the type of the field is known, 
	* consider calling addField() instead. 
	* 
	* @param fieldName - name of the field to be modified/created 
	* @param value - the value of the field * @param convert - <usage>
	* @param convert - the boolean value to indicate wheather or not to covert a string
	*                  containing a hex number to a binary value. 
	*                  For case convert = true, the valid value of parameter 'value'  is
	*                  that all of its charaters to be in the range  of ['0'-'9'], 
    *                  ['a'-'f'], or ['A'-'F'].
	* 
	* @returns Returns true if the field was replaced; false otherwise. 
	* 
	* @throw A GmsecException is thrown if the field name is NULL or contains an empty string. 
	* @throw A GmsecException is thrown if the input value's type cannot be converted into the type required 
	* by the field, or if the string is too big when converting to char. 
	*/
	bool CALL_TYPE setFieldValue(const char* fieldName, const char* value, bool convert);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_I16 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_I16 value);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_U16 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_U16 value);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_I32 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_I32 value);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_U32 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_U32 value);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_I64 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_I64 value);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_U64 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_U64 value);


	/**
	 * @fn bool setFieldValue(const char* fieldName, GMSEC_F64 value)
	 *
	 * @brief Adds/replaces a field with the given name and value to the message. If
	 * a field template for the schema in use is available for the named field, then
	 * the value's type will be coerced to match that which is described in the field
	 * template.
	 *
	 * @note This method is not very efficient; if the type of the field is known,
	 * consider calling addField() instead.
	 * 
	 * @param fieldName - name of the field to be modified/created
	 * @param value     - the value of the field
	 *
	 * @returns Returns true if the field was replaced; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the field name is NULL or contains an empty string.
	 * @throw A GmsecException is thrown if the input value's type cannot be converted into the type required.
	 */
	bool CALL_TYPE setFieldValue(const char* fieldName, GMSEC_F64 value);


	/**
	 * @fn void setConfig(const Config& config)
	 *
	 * @brief This function sets configuration items to the message.
	 *
	 * @note If a configuration item included within the given config is already associated
	 * with the message, then the message's configuration item will be replaced with the
	 * new item.
	 *
	 * @param config - Config that contains configuration values to add to the Message.
	 *
	 * @note Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 */
	void CALL_TYPE setConfig(const Config& config);


	/**
	 * @fn Config& getConfig()
	 *
	 * @brief Returns the configuration associcated with the message.
	 *
	 * @return A reference to the Config object associated with the Message.
	 */
	Config& CALL_TYPE getConfig();


	/**
	 * @fn const Config& getConfig() const
	 *
	 * @brief Returns the configuration associcated with the message.
	 *
	 * @return A reference to the Config object associated with the Message.
	 */
	const Config& CALL_TYPE getConfig() const;


	/**
	 * @fn void setSubject(const char* subject) const
	 *
	 * @brief Allows for the setting/changing of the message subject.
	 *
	 * @param subject - the subject/topic to associate with the message.
	 *
	 * @throw A GmsecException is thrown if the subject is NULL or contains an empty-string.
	 */
	void CALL_TYPE setSubject(const char* subject);


	/**
	 * @fn const char* getSubject() const
	 *
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @return The subject assigned to the Message.
	 */
	const char* CALL_TYPE getSubject() const;

	/**
	* @fn void setSubjectElement(const char* name, const char* value)
	*
	* @brief Allows for the setting of individual subject elements. The name of the elements are defined
	* by the message's corresponding message template. This value will be overridden by automatic subject
	* generation if the subject element is defined by an existing field in the message, or if the subject
	* was manually defined with setSubject.
	*
	* @param name - the name of the subject element
	* @param value - the value of the subject element.  An empty or null value will be seen as FILL in the subject line.
	*
	* @throw A GmsecException is thrown if the name is NULL, an empty string, or does not match a subject element
	* name defined the message template, or if the message does not have a corresponding message template.
	*
	*
	*/
	void CALL_TYPE setSubjectElement(const char* name, const char* value);


	/**
	 * @fn setKind(Message::Kind kind)
	 *
	 * @brief This method sets the message kind.
	 *
	 * @param kind - the message kind
	 *
	 * @note The API can auto-deduce the message kind, but there may be cases where
	 * this method may be useful. For most common Use Cases, this method should not
	 * need to be called.
	 */
	void CALL_TYPE setKind(Message::Kind kind);


	/**
	 * @fn Message::Kind getKind() const
	 *
	 * @brief This method returns the message kind.
	 *
	 * @return The Kind assigned to the Message.
	 */
	Message::Kind CALL_TYPE getKind() const;


	/**
	 * @fn bool addField(const Field& field)
	 *
	 * @brief This function will add a new field to this message. A new field will be added to the message
	 * that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	 * The client program must also clean up that field when no longer needed.
	 *
	 * Example:
	 * @code
	 * bool replaced = msg.addField(I32Field("MyField", 1234));
	 * if (replaced) {
	 *	   //field was replaced
	 * }
	 * else {
	 *     //new field added
	 * }
	 * @endcode
	 *
	 * @param field - field object to copy info from
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 */
	bool CALL_TYPE addField(const Field& field);


	/**
	 * @fn bool addField(const char* name, const GMSEC_U8* blob, size_t len)
	 *
	 * @brief Adds a BinaryField to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char* name, const GMSEC_U8* blob, size_t len);


	/**
	 * @fn bool addField(const char* name, bool value)
	 *
	 * @brief Adds a BooleanField to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char* name, bool value);


	/**
	 * @fn bool addField(const char* name, GMSEC_CHAR value)
	 *
	 * @brief Adds a CharField to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char* name, GMSEC_CHAR value);


	/**
	 * @fn bool addField(const char* name, GMSEC_F32 value)
	 *
	 * @brief Adds an F32Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_F32 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_F64 value)
	 *
	 * @brief Adds an F64Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_F64 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_I8 value)
	 *
	 * @brief Adds an I8Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_I8 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_I16 value)
	 *
	 * @brief Adds an I16Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_I16 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_I32 value)
	 *
	 * @brief Adds I32Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_I32 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_I64 value)
	 *
	 * @brief Adds I64Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_I64 value);


	/**
	 * @fn bool addField(const char* name, const char* value)
	 *
	 * @brief Adds StringField to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, const char* value);


	/**
	 * @fn bool addField(const char* name, GMSEC_U8 value)
	 *
	 * @brief Adds a U8Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_U8 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_U16 value)
	 *
	 * @brief Adds a U16Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_U16 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_U32 value)
	 *
	 * @brief Adds a U32Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_U32 value);


	/**
	 * @fn bool addField(const char* name, GMSEC_U64 value)
	 *
	 * @brief Adds a U64Field to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throw A GmsecException is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_U64 value);


	/**
	 * @fn bool addFields(const gmsec::api5::util::List<Field*> fields)
	 *
	 * @brief Adds each Field in the given list to the Message.
	 *
	 * @return Returns true if any existing field in the Message has been replaced; false otherwise.
	 *
	 */
	bool CALL_TYPE addFields(const gmsec::api5::util::List<Field*>& fields);


	/**
	 * @fn void clearFields()
	 *
	 * @brief Removes all fields form the message.
	 */
	void CALL_TYPE clearFields();


	/**
	 * @fn bool clearField(const char* name)
	 *
	 * @brief This function will remove a field from this message by name.
	 *
	 * @param name - name of field to remove
	 *
	 * @return Returns true if the field was removed; false otherwise.
	 */
	bool CALL_TYPE clearField(const char* name);


	/**
	 * @fn bool hasField(const char* name) const
	 *
	 * @brief Reports whether the field, identified by name, is present in the Message.
	 *
	 * @param name - the name of field to find
	 *
	 * @return True if the field is found; false otherwise.
	 */
	bool CALL_TYPE hasField(const char* name) const;


	/**
	 * @fn const char* getStringValue(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into string representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a string value.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a string value.
	 */
	const char* CALL_TYPE getStringValue(const char* fieldName) const;


	/**
	 * @fn bool getBooleanValue(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into boolean representation.
	 *
	 * @note Any non-zero value will be interpreted as true, otherwise as false.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a boolean value.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a boolean.
	 */
	bool CALL_TYPE getBooleanValue(const char* fieldName) const;


	/**
	 * @fn GMSEC_I16 getI16Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into a signed 16-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a GMSEC_I16.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a signed 16-bit integer.
	 */
	GMSEC_I16 CALL_TYPE getI16Value(const char* fieldName) const;


	/**
	 * @fn GMSEC_I32 getI32Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into a signed 32-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a GMSEC_I32.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	GMSEC_I32 CALL_TYPE getI32Value(const char* fieldName) const;


	/**
	 * @fn GMSEC_I64 getI64Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into a signed 64-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a GMSEC_I64.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	GMSEC_I64 CALL_TYPE getI64Value(const char* fieldName) const;


	/**
	 * @fn GMSEC_U16 getU16Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into an unsigned 16-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an unsigned 16-bit integer value.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an unsigned 16-bit integer.
	 */
	GMSEC_U16 CALL_TYPE getU16Value(const char* fieldName) const;


	/**
	 * @fn GMSEC_U32 getU32Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into an unsigned 32-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an unsigned 32-bit integer value.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an unsigned 32-bit integer.
	 */
	GMSEC_U32 CALL_TYPE getU32Value(const char* fieldName) const;


	/**
	 * @fn GMSEC_U64 getU64Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into an unsigned 64-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an unsigned 64-bit integer value.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an unsigned 64-bit integer.
	 */
	GMSEC_U64 CALL_TYPE getU64Value(const char* fieldName) const;


	/**
	 * @fn GMSEC_F64 getF64Value(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a floating-point value.
	 *
	 * @throw A GmsecException is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a floating-point value.
	 */
	GMSEC_F64 CALL_TYPE getF64Value(const char* fieldName) const;


	/**
	 * @fn const Field* getField(const char* name) const
	 *
	 * @brief This function will return the named Field object contained within the Message object,
	 * if it exists.  Otherwise a NULL pointer is returned.
	 *
	 * Example:
	 * @code
	 * // Note that the dynamic_cast<> shown below is used to demon-
	 * // strate a safe manner in which to convert a base Field object.
	 * // into a more specialized type.
	 * //
	 * // For applications that follow a deterministic model, a C-style
	 * // cast would suffice, and prove to be more efficient.
	 * //
	 * const BooleanField* field =
	 *     dynamic_cast<const BooleanField*>(msg->getField("BOOL_FIELD"));
	 *
	 * if (field)
	 * {
	 *     bool value = field->getValue();
	 * }
	 * else
	 * {
	 *	    //handle error
	 * }
	 *
	 * @endcode
	 *
	 * @param name - name of field to get
	 *
	 * @return If found, a pointer to the field; otherwise NULL.
	 */
	const Field* CALL_TYPE getField(const char* name) const;


	/**
	 * @fn Field::Type getFieldType(const char* name) const
	 *
	 * @brief Returns the Field Type associated with the named field.
	 *
	 * @param name - name of field to get the type for
	 *
	 * @return An enumerated Type value as defined in Field.
	 *
	 * @throw A GmsecException is thrown if the named field cannot be found.
	 */
	Field::Type CALL_TYPE getFieldType(const char* name) const;


	/**
	 * @fn BinaryField& getBinaryField(const char* name) const
	 *
	 * @brief Returns a reference to the BinaryField associated with the named field.
	 *
	 * @return A refences to a BinaryField object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT a BinaryField or cannot be found.
	 */
	const BinaryField& CALL_TYPE getBinaryField(const char* name) const;


	/**
	 * @fn BooleanField& getBooleanField(const char* name) const
	 *
	 * @brief Returns a reference to the BooleanField associated with the named field.
	 *
	 * @return A reference to a BooleanField object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT a BooleanField or cannot be found.
	 */
	const BooleanField& CALL_TYPE getBooleanField(const char* name) const;


	/**
	 * @fn CharField& getCharField(const char* name) const
	 *
	 * @brief Returns a reference to the CharField associated with the named field.
	 *
	 * @return A reference to a CharField object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT a CharField or cannot be found.
	 */
	const CharField& CALL_TYPE getCharField(const char* name) const;


	/**
	 * @fn F32Field& getF32Field(const char* name) const
	 *
	 * @brief Returns a reference to the F32Field associated with the named field.
	 *
	 * @return A reference to an F32Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an F32Field or cannot be found.
	 */
	const F32Field& CALL_TYPE getF32Field(const char* name) const;


	/**
	 * @fn F64Field& getF64Field(const char* name) const
	 *
	 * @brief Returns a reference to the F64Field associated with the named field.
	 *
	 * @return A reference to an F64Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an F64Field or cannot be found.
	 */
	const F64Field& CALL_TYPE getF64Field(const char* name) const;


	/**
	 * @fn I8Field& getI8Field(const char* name) const
	 *
	 * @brief Returns a reference to the I8Field associated with the named field.
	 *
	 * @return A reference to an I8Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an I8Field or cannot be found.
	 */
	const I8Field& CALL_TYPE getI8Field(const char* name) const;


	/**
	 * @fn I16Field& getI16Field(const char* name) const
	 *
	 * @brief Returns a reference to the I16Field associated with the named field.
	 *
	 * @return A reference to an I16Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an I16Field or cannot be found.
	 */
	const I16Field& CALL_TYPE getI16Field(const char* name) const;


	/**
	 * @fn I32Field& getI32Field(const char* name) const
	 *
	 * @brief Returns a reference to the I32Field associated with the named field.
	 *
	 * @return A refernce to an I32Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an I32Field or cannot be found.
	 */
	const I32Field& CALL_TYPE getI32Field(const char* name) const;


	/**
	 * @fn I64Field& getI64Field(const char* name) const
	 *
	 * @brief Returns a reference to the I64Field associated with the named field.
	 *
	 * @return A reference to an I64Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an I64Field or cannot be found.
	 */
	const I64Field& CALL_TYPE getI64Field(const char* name) const;


	/**
	 * @fn U8Field& getU8Field(const char* name) const
	 *
	 * @brief Returns a reference to the U8Field associated with the named field.
	 *
	 * @return A reference to an U8Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an U8Field or cannot be found.
	 */
	const U8Field& CALL_TYPE getU8Field(const char* name) const;


	/**
	 * @fn U16Field& getU16Field(const char* name) const
	 *
	 * @brief Returns a reference to the U16Field associated with the named field.
	 *
	 * @return A reference to an U16Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an U16Field or cannot be found.
	 */
	const U16Field& CALL_TYPE getU16Field(const char* name) const;


	/**
	 * @fn U32Field& getU32Field(const char* name) const
	 *
	 * @brief Returns a reference to the U32Field associated with the named field.
	 *
	 * @return A refernce to an U32Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an U32Field or cannot be found.
	 */
	const U32Field& CALL_TYPE getU32Field(const char* name) const;


	/**
	 * @fn U64Field& getU64Field(const char* name) const
	 *
	 * @brief Returns a reference to the U64Field associated with the named field.
	 *
	 * @return A reference to an U64Field object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT an U64Field or cannot be found.
	 */
	const U64Field& CALL_TYPE getU64Field(const char* name) const;


	/**
	 * @fn StringField& getStringField(const char* name) const
	 *
	 * @brief Returns a reference to the StringField associated with the named field.
	 *
	 * @return A reference to a StringField object.
	 *
	 * @throw A GmsecException is thrown if the named field is NOT a StringField or cannot be found.
	 */
	const StringField& CALL_TYPE getStringField(const char* name) const;


	/**
	 * @fn size_t getFieldCount() const
	 *
	 * @brief This function gets the number of fields contained in this message.
	 *
	 * @return The number of fields contained within Message.
	 */
	size_t CALL_TYPE getFieldCount() const;


	/**
	 * @fn void copyFields(Message& toMsg) const
	 *
	 * @brief This function will copy all fields from this message to a new message in the fastest way possible
	 *
	 * @note The fields are NOT copied by reference. As such, the original message and its fields can be
	 * deleted or modified after it has been copied to the new message.
	 *
	 * @param toMsg - the message to copy fields into
	 */
	void CALL_TYPE copyFields(Message& toMsg) const;


	/**
	 * @fn const char* toXML() const
	 *
	 * @brief This function will dump a message to an XML formatted string.
	 *
	 * XML Format example:
	 * @code
	 * <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
	  * 	<FIELD TYPE="CHAR" NAME="CHAR-FIELD">c</FIELD>
	  * 	<FIELD TYPE="BOOL" NAME="BOOL-FIELD">TRUE</FIELD>
	  * 	<FIELD TYPE="I16" NAME="SHORT-FIELD">-123</FIELD>
	  * 	<FIELD TYPE="U16" NAME="USHORT-FIELD">123</FIELD>
	  * 	<FIELD TYPE="I32" NAME="INT-FIELD">-12345678</FIELD>
	  * 	<FIELD TYPE="U32" NAME="UINT-FIELD">123456789</FIELD>
	  * 	<FIELD TYPE="STRING" NAME="STRING-FIELD">This is a test of the Message::toXML function</FIELD>
	  * 	<FIELD TYPE="F32" NAME="LONG-FIELD" BITS="42F6E666">123.45</FIELD>
	  * 	<FIELD TYPE="F64" NAME="DOUBLE-FIELD" BITS="4B94216CA930D75A">1.234e56</FIELD>
	  * 	<FIELD TYPE="BIN" NAME="BIN-FIELD">4a4c4d4e4f5051</FIELD>
	 * </MESSAGE>
	 * @endcode
	 *
	 * @return XML string representation of the Message.
	 */
	const char* CALL_TYPE toXML() const;


	/**
	 * @fn const char* toJSON() const
	 *
	 * @brief This function will dump a message to a JSON formatted string.
	 *
	 * JSON Format example (note, newlines added for readability):
	 * @code
	 * {
	 *    "MESSAGE":{
	 *        "KIND":"PUBLISH",
	 *        "SUBJECT":"GMSEC.MISSION.CONST.SAT.EVT.MSG",
	 *        "FIELD":[
	 *            {
	 *                "NAME":"CHAR-FIELD",
	 *                "TYPE":"CHAR",
	 *                "VALUE":"c"
	 *            },
	 *            {
	 *                "NAME":"BOOL-FIELD",
	 *                "TYPE":"BOOL",
	 *                "VALUE":"TRUE"
	 *            },
	 *            {
	 *                "NAME":"SHORT-FIELD",
	 *                "TYPE":"I16",
	 *                "VALUE":"-123"
	 *            },
	 *            {
	 *                "NAME":"USHORT-SHORT",
	 *                "TYPE":"U16",
	 *                "VALUE":"123"
	 *            },
	 *            {
	 *                "NAME":"INT-FIELD",
	 *                "TYPE":"I32",
	 *                "VALUE":"-12345678"
	 *            },
	 *            {
	 *                "NAME":"UINT-FIELD",
	 *                "TYPE":"U32",
	 *                "VALUE":"123456789"
	 *            },
	 *            {
	 *                "NAME":"STRING-FIELD",
	 *                "TYPE":"STRING",
	 *                "VALUE":"This is a test of the Message::toJSON function"
	 *            },
	 *            {
	 *                "NAME":"LONG-FIELD",
	 *                "TYPE":"F32",
	 *                "VALUE":"123.45"
	 *                "BITS":"42F6E666"
	 *            },
	 *            {
	 *                "NAME":"DOUBLE-FIELD",
	 *                "TYPE":"F64",
	 *                "VALUE":"1.234e56"
	 *                "BITS":"4B94216CA930D75A"
	 *            },
	 *            {
	 *                "NAME":"BIN-FIELD",
	 *                "TYPE":"BIN",
	 *                "VALUE":"4a4c4d4e4f5051"
	 *            }
	 *	    ]
	 *	}
	 * }
	 * @endcode
	 *
	 * @return a JSON formatted string
	 */
	const char* CALL_TYPE toJSON() const;


	/**
	 * @fn size_t getSize() const
	 *
	 * @brief This function will return the physical storage size of the message in bytes.
	 * Please note that additional tracking data fields are added to messages at the
	 * time of publication which will affect the size of the message.
	 *
	 * @return size of message in bytes
	 */
	size_t CALL_TYPE getSize() const;


	/**
	 * @fn MessageFieldIterator& getFieldIterator(MessageFieldIterator::Selector selector = MessageFieldIterator::ALL_FIELDS) const
	 *
	 * @brief Method that allows the callee to get the MessageFieldIterator associated with the %Message.
	 * This iterator will allow for applications to iterate over the Field objects stored within the %Message.
	 * The iterator is reset each time getFieldIterator() is called. The iterator itself is destroyed when the
	 * %Message object is destroyed.
	 *
	 * @note Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object. Each call will reset the iterator.
	 *
	 * @return A reference to a MessageFieldIterator object.
	 */
	MessageFieldIterator& CALL_TYPE getFieldIterator(MessageFieldIterator::Selector selector = MessageFieldIterator::Selector::ALL_FIELDS) const;


private:
	friend class gmsec::api5::internal::MessageBuddy;
	friend class gmsec::api5::internal::InternalMessage;
	friend class gmsec::api5::internal::InternalMessageFieldIterator;

	gmsec::api5::internal::InternalMessage* m_internal;
};

}  // end namespace api5
}  // end namespace gmsec

#endif
