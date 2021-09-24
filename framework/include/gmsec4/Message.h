/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Message.h
 *
 * @brief This file contains the public interface to messages.
 */


#ifndef GMSEC_API_MESSAGE_H
#define GMSEC_API_MESSAGE_H

#include <gmsec4/MessageFieldIterator.h>

#include <gmsec4/Fields.h>

#include <gmsec4/util/DataList.h>
#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <stddef.h>  // for size_t


namespace gmsec
{

namespace api
{
	// Forward declaration(s)
	class Config;
	class Field;
	class MessageFieldIterator;

	namespace mist {
		namespace message {
			namespace internal {
				class InternalMistMessage;
			}
		}
	}

namespace internal
{
	class GMSECadapter;
	class InternalMessage;
	class InternalMessageFieldIterator;
	class MessageBuddy;
}


/**
 * @class Message
 *
 * @brief The Message object is a container for GMSEC Messages.  The methods of this class
 * allow the construction and manipulation of the data in a message.
 *
 * @sa Config @n
 *     Connection @n
 *     Field @n
 *     MessageFieldIterator
 */
class GMSEC_API Message
{
public:
	/**
	 * @enum MessageKind
	 *
	 * @brief Allowable type of messages that can be created.
	 *
	 * @note Message kind numbering was chosen to match that of API 3.x
	 */
	enum MessageKind
	{
		PUBLISH = 1, ///< For messages that will be published
		REQUEST = 2, ///< For request messages
		REPLY   = 3  ///< For reply messages
	};


	/**
	 * @fn Message(const char* subject, MessageKind kind)
	 *
	 * @brief Default constructor - initializes the message instance
	 *
	 * Example:
	 * @code
	 * // To publish a message
	 * // Create a Message object
	 * 
	 * Message msg("GMSEC.TEST.SUBJECT", Message::PUBLISH);
	 * 
	 * // You can add Fields to the message with the addField functions
	 * // depending on the type of field you want to add.
	 * // There are two ways to add Fields, this shows how to add a String Field
	 *
	 * bool replaced = msg.addField(StringField("STRING-FIELD-NAME-1", "Example Text"));
	 * //  or 
	 * bool replaced = msg.addField("STRING-FIELD-NAME-2", "Example Text");
	 *
	 * // Check to see if you replaced an existing field
	 * if (replaced) {
	 *     //field was replaced
	 * }
	 * else {
	 *     //new field added
	 * }
	 *
	 * //Create a connection with Connection::create()
 	 * Config cfg(argc,argv);
	 *
	 * try {
	 *     // Create the Connection
	 *     Connection* conn = Connection::create(cfg);
         *
	 *     // Establish the connection
	 *     conn->connect();
	 *
	 *     ...
	 *
	 *     // Publish message
	 *     conn->publish(msg);
	 *
	 *     ...
	 *
	 *     // Disconnect from middleware server
	 *     conn->disconnect();
	 *
	 *     // Destroy the Connection object
	 *     Connection::destroy(conn);
	 * }
	 * catch (Exception& e) {
	 *     //handle error
	 * }
	 *
	 * @endcode
	 *
	 * @param subject - the subject string for the message.
	 * @param kind - the kind of message to instantiate.
	 *
	 * @throw An Exception is thrown if the subject is NULL or contains an empty-string.
	 */
	Message(const char* subject, MessageKind kind);


	/**
	 * @fn Message(const char* subject, MessageKind kind, const Config& config)
	 *
	 * @brief constructor - initializes the message instance and associates a Configuration object
	 *
	 * Example:
	 * @code
	 * // To publish a message
	 * // Create a Message object
	 * 
	 * Config cfg(//message-related config arguments);
	 *
	 * Message msgConfig("GMSEC.TEST.CONFIG.SUBJECT", Message::PUBLISH, cfg);
	 * 
	 * // You can add Fields to the message with the addField functions
	 * // depending on the type of field you want to add.
	 * // There are two ways to add Fields, this shows how to add a String Field
	 *
	 * bool replaced = msg.addField(StringField("STRING-FIELD-NAME-1", "Example Text"));
	 * //  or 
	 * bool replaced = msg.addField("STRING-FIELD-NAME-2", "Example Text");
	 *
	 * // Check to see if you replaced an existing field
	 * if (replaced) {
	 *     //field was replaced
	 * }
	 * else {
	 *     //new field added
	 * }
	 *
	 * //Create a connection with Connection::create()
 	 * Config config(argc,argv);
	 *
	 * try {
	 *     // Create the Connection
	 *     Connection* conn = Connection::create(config);
     *
	 *     // Establish the connection
	 *     conn->connect();
	 *
	 *     ...
	 *
	 *     // Publish message
	 *     conn->publish(msgConfig);
	 *
	 *     ...
	 *
	 *     // Disconnect from middleware server
	 *     conn->disconnect();
	 *
	 *     // Destroy the Connection object
	 *     Connection::destroy(conn);
	 * }
	 * catch (Exception& e) {
	 *     //handle error
	 * }
	 *
	 * @endcode
	 *
	 * @param subject - the subject string for the message.
	 * @param kind - the kind of message to instantiate.
	 * @param config - a configuration to associate with the message.
	 *
	 * @throw An Exception is thrown if the subject is NULL or contains an empty-string.
	 *
	 * @note Only Message-related configuration options are held by the Message object.
	 * These configuration options are NOT included with the message when it is sent
	 * across the GMSEC Bus.
	 */
	Message(const char* subject, MessageKind kind, const Config& config);


	/**
	 * @fn Message(const char* data)
	 *
	 * @brief constructor - initializes the message instance using XML or JSON text
	 *
	 * Example:
	 * @code
	 * // To publish a message with XML text or JSON
	 * // Create a Message object with XML text
	 * 
	 * const char* XML = "<MESSAGE SUBJECT=\"GMSEC.TEST.XML.SUBJECT\" KIND=\"PUBLISH\">"
	 *                   "    <CONFIG>"
	 *                   "        <PARAMETER NAME=\"gmsec-msgfld-store-type\">tree</PARAMETER>"
	 *                   "        <PARAMETER NAME=\"gmsec-msgfld-store-size\">200</PARAMETER>"
	 *                   "    </CONFIG>"
	 *                   "    <FIELD TYPE=\"STRING\" NAME=\"EXAMPLE-STRING-NAME\">EXAMPLE TEXT</FIELD>"
	 *                   "    <FIELD TYPE=\"BOOL\" NAME=\"EXAMPLE-BOOL-FIELD-NAME\">TRUE</FIELD>"
	 *                   "</MESSAGE>";
	 *
	 * Message msgXML(XML);
	 * 
	 * // Create a Message object with JSON text 
	 * 
 	 * const char* JSON = "{\"MESSAGE\":{ \
	 *                       \"KIND\":\"PUBLISH\", \
	 *                       \"SUBJECT\":\"GMSEC.TEST.JSON.SUBJECT\", \
	 *                       \"FIELD\":[ \
	 *                         { \
	 *                            \"NAME\":\"CHAR-FIELD\", \
	 *                            \"TYPE\":\"CHAR\", \
	 *                            \"VALUE\":\"c\" \
	 *                         }, \
	 *                         { \
	 *                            \"NAME\":\"BOOL-FIELD\", \
	 *                            \"TYPE\":\"BOOL\", \
	 *                            \"VALUE\":\"TRUE\" \
	 *                         } \
	 *                         ] \
	 *                       } \
	 *                     }";
	 *              
	 * Message msgJSON(JSON);
	 * 
	 * //Create a connection with Connection::create()
	 * Config cfg(argc,argv);
	 * 
	 * try {
	 *     // Create the Connection
	 *     Connection* conn = Connection::create(cfg);
	 * 
	 *     // Establish the connection
	 *     conn->connect();
	 * 
	 *     ...
	 * 
	 *     // Publish message
	 *     conn->publish(msgXML);
	 *     conn->publish(msgJSON);
	 * 
	 *     ...
	 * 
	 *     // Disconnect from middleware server
	 *     conn->disconnect();
	 * 
	 *     // Destroy the Connection object
	 *     Connection::destroy(conn);
	 * }
	 * catch (Exception& e) {
	 *     //handle error
	 * }
	 *
	 * @endcode
	 * @param data - an XML or JSON string
	 *
	 * @throw Exception is thrown if the XML or JSON string cannot be parsed.
	 *
	 * @sa toXML() for the XML string structure for a Message object
	 * @sa toJSON() for the JSON string structure for a Message object
	 */
	Message(const char* data);


	/**
	 *@fn Message(const Message& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given Message
	 *
	 * @param other - the other Message object to copy.
	 */
	Message(const Message& other);


	 /**
	 * @fn ~Message()
	 *
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~Message();


    /**
	 * @fn Message& operator=(const Message& other)
	 *
     * @brief Overloaded assignment operator method for making a copy of other Message object
	 *
	 * @param other - the other Message object to copy.
	 *
     * @return A copy of the other Message object that was given.
     */
	Message& operator=(const Message& other);
	 

	/**
	 * @fn void addConfig(const Config& config)
	 *
	 * @brief This function adds configuration items to the message.
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
	void CALL_TYPE addConfig(const Config& config);


	/**
	 * @fn const Config& getConfig() const
	 *
	 * @brief This function gets a copy of the Configuration associated with the message.
	 *
	 * @return A reference to the Config object associated with the Message.
	 */
	const Config& CALL_TYPE getConfig() const;


	/**
	 * @fn const char* getSubject() const
	 *
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @return The subject assigned to the Message.
	 */
	const char* CALL_TYPE getSubject() const;


	/**
	 * @fn void setSubject(const char* subject) const
	 *
	 * @brief Allows for the setting/changing of the message subject.
	 *
	 * @param subject - the subject/topic to associate with the message.
	 *
	 * @throw An Exception is thrown if the subject is NULL or contains an empty-string.
	 */
	void CALL_TYPE setSubject(const char* subject);


	/**
	 * @fn MessageKind getKind() const
	 *
	 * @brief This function gets the 'kind' of this message.
	 *
	 * @return The MessageKind assigned to the Message.
	 *
	 * @sa MessageKind
	 */
	MessageKind CALL_TYPE getKind() const;


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
	 * @fn bool addField(const char* name, GMSEC_BIN bin, size_t len)
	 *
	 * @brief Adds a BinaryField to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char* name, GMSEC_BIN bin, size_t len);


	/**
	 * @fn bool addField(const char* name, bool value)
	 *
	 * @brief Adds a BooleanField to the Message.
	 *
	 * @return Returns true if the Field is replacing one with the same name; false otherwise.
	 *
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
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
	 * @throws An Exception is thrown if the given field name is null or contains an empty string.
	 *
	 * @sa bool addField(const Field& field)
	 */
	bool CALL_TYPE addField(const char*  name, GMSEC_U64 value);


	/**
	 * @fn bool addFields(const gmsec::api::util::DataList<Field*> fields)
	 *
	 * @brief Adds each Field in the given list to the Message.
	 *
	 * @return Returns true if any existing field in the Message has been replaced; false otherwise.
	 *
	 */
	bool CALL_TYPE addFields(const gmsec::api::util::DataList<Field*>& fields);


	/**
	 * @fn void clearFields()
	 *
	 * @desc This function will remove all fields form this message.
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
	 * @fn GMSEC_I64 getIntegerValue(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into a signed 64-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an integer value.
	 *
	 * @throws An Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an integer.
	 */
	GMSEC_I64 CALL_TYPE getIntegerValue(const char* fieldName) const;


	/**
	 * @fn GMSEC_U64 getUnsignedIntegerValue(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into an unsigned 64-bit integer number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as an unsigned 64-bit integer value.
	 *
	 * @throws An Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to an unsigned 64-bit integer.
	 */
	GMSEC_U64 CALL_TYPE getUnsignedIntegerValue(const char* fieldName) const;


	/**
	 * @fn GMSEC_F64 getDoubleValue(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into an 64-bit floating-point number representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a floating-point value.
	 *
	 * @throws An Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a float-point value.
	 */
	GMSEC_F64 CALL_TYPE getDoubleValue(const char* fieldName) const;


	/**
	 * @fn const char* getStringValue(const char* fieldName) const
	 *
	 * @brief Attempts to convert the field value into string representation.
	 *
	 * @param fieldName - the name of the field from which to reference the value.
	 *
	 * @return Returns the field value as a string value.
	 *
	 * @throws An Exception is thrown if the field cannot be found, or if it cannot successfully
	 * be converted to a string value.
	 */
	const char* CALL_TYPE getStringValue(const char* fieldName) const;


	/**
	 * @fn bool hasField(const char* name) const
	 *
	 * @brief Reports whether the field, identified by name, is present in the Message.
	 *
	 * @param name - the name of field to find
	 *
	 * @return True if the field is found; false otherwise.
	 *
	 * @throws Exception is thrown if the given field name is NULL or contains an empty string.
	 */
	bool hasField(const char* name) const;


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
	 * @fn Field::FieldType getFieldType(const char* name) const
	 *
	 * @brief Returns the FieldType associated with the named field.
	 *
	 * @param name - name of field to get the type for
	 *
	 * @return FieldType as defined in Field.
	 *
	 * @throws An Exception is thrown if the named field cannot be found.
	 */
	Field::FieldType CALL_TYPE getFieldType(const char* name) const;


	/**
	 * @fn BinaryField& getBinaryField(const char* name) const
	 *
	 * @brief Returns a reference to the BinaryField associated with the named field.
	 *
	 * @return A refences to a BinaryField object.
	 *
	 * @throws An Exception is thrown if the named field is NOT a BinaryField or cannot be found.
	 */
	const BinaryField& CALL_TYPE getBinaryField(const char* name) const;


	/**
	 * @fn BooleanField& getBooleanField(const char* name) const
	 *
	 * @brief Returns a reference to the BooleanField associated with the named field.
	 *
	 * @return A reference to a BooleanField object.
	 *
	 * @throws An Exception is thrown if the named field is NOT a BooleanField or cannot be found.
	 */
	const BooleanField& CALL_TYPE getBooleanField(const char* name) const;


	/**
	 * @fn CharField& getCharField(const char* name) const
	 *
	 * @brief Returns a reference to the CharField associated with the named field.
	 *
	 * @return A reference to a CharField object.
	 *
	 * @throws An Exception is thrown if the named field is NOT a CharField or cannot be found.
	 */
	const CharField& CALL_TYPE getCharField(const char* name) const;


	/**
	 * @fn F32Field& getF32Field(const char* name) const
	 *
	 * @brief Returns a reference to the F32Field associated with the named field.
	 *
	 * @return A reference to an F32Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an F32Field or cannot be found.
	 */
	const F32Field& CALL_TYPE getF32Field(const char* name) const;


	/**
	 * @fn F64Field& getF64Field(const char* name) const
	 *
	 * @brief Returns a reference to the F64Field associated with the named field.
	 *
	 * @return A reference to an F64Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an F64Field or cannot be found.
	 */
	const F64Field& CALL_TYPE getF64Field(const char* name) const;


	/**
	 * @fn I8Field& getI8Field(const char* name) const
	 *
	 * @brief Returns a reference to the I8Field associated with the named field.
	 *
	 * @return A reference to an I8Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an I8Field or cannot be found.
	 */
	const I8Field& CALL_TYPE getI8Field(const char* name) const;


	/**
	 * @fn I16Field& getI16Field(const char* name) const
	 *
	 * @brief Returns a reference to the I16Field associated with the named field.
	 *
	 * @return A reference to an I16Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an I16Field or cannot be found.
	 */
	const I16Field& CALL_TYPE getI16Field(const char* name) const;


	/**
	 * @fn I32Field& getI32Field(const char* name) const
	 *
	 * @brief Returns a reference to the I32Field associated with the named field.
	 *
	 * @return A refernce to an I32Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an I32Field or cannot be found.
	 */
	const I32Field& CALL_TYPE getI32Field(const char* name) const;


	/**
	 * @fn I64Field& getI64Field(const char* name) const
	 *
	 * @brief Returns a reference to the I64Field associated with the named field.
	 *
	 * @return A reference to an I64Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an I64Field or cannot be found.
	 */
	const I64Field& CALL_TYPE getI64Field(const char* name) const;


	/**
	 * @fn U8Field& getU8Field(const char* name) const
	 *
	 * @brief Returns a reference to the U8Field associated with the named field.
	 *
	 * @return A reference to an U8Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an U8Field or cannot be found.
	 */
	const U8Field& CALL_TYPE getU8Field(const char* name) const;


	/**
	 * @fn U16Field& getU16Field(const char* name) const
	 *
	 * @brief Returns a reference to the U16Field associated with the named field.
	 *
	 * @return A reference to an U16Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an U16Field or cannot be found.
	 */
	const U16Field& CALL_TYPE getU16Field(const char* name) const;


	/**
	 * @fn U32Field& getU32Field(const char* name) const
	 *
	 * @brief Returns a reference to the U32Field associated with the named field.
	 *
	 * @return A refernce to an U32Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an U32Field or cannot be found.
	 */
	const U32Field& CALL_TYPE getU32Field(const char* name) const;


	/**
	 * @fn U64Field& getU64Field(const char* name) const
	 *
	 * @brief Returns a reference to the U64Field associated with the named field.
	 *
	 * @return A reference to an U64Field object.
	 *
	 * @throws An Exception is thrown if the named field is NOT an U64Field or cannot be found.
	 */
	const U64Field& CALL_TYPE getU64Field(const char* name) const;


	/**
	 * @fn StringField& getStringField(const char* name) const
	 *
	 * @brief Returns a reference to the StringField associated with the named field.
	 *
	 * @return A reference to a StringField object.
	 *
	 * @throws An Exception is thrown if the named field is NOT a StringField or cannot be found.
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
	 * JSON Format example:
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
	 * The iterator is reset each time getFieldIterator() is called.  The iterator itself is destroyed when the
	 * %Message object is destroyed.
	 *
	 * @note Only one MessageFieldIterator object is associated with a %Message object; multiple calls to
	 * getFieldIterator() will return a reference to the same MessageFieldIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a MessageFieldIterator object.
	 */
	MessageFieldIterator& CALL_TYPE getFieldIterator(MessageFieldIterator::Selector selector = MessageFieldIterator::ALL_FIELDS) const;


protected:
	/** @cond
	 *  Constructor to be used by sub-classes of Message
	 */
	Message(gmsec::api::internal::InternalMessage* internal);
	/** @endcond */


	/** @cond
	 *  Used by sub-classes to register internal message object outside of a constructor
	 *  (e.g. within assignment operator method)
	 */
	void CALL_TYPE registerInternal(gmsec::api::internal::InternalMessage* internal);
	/** @endcond */


	/** @cond
	 *  Used by sub-classes to get a reference to the internal message object.
	 */
	gmsec::api::internal::InternalMessage& CALL_TYPE getInternal() const;
	/** @endcond */


private:
	friend class gmsec::api::internal::GMSECadapter;
	friend class gmsec::api::internal::MessageBuddy;
	friend class gmsec::api::internal::InternalMessageFieldIterator;
	friend class gmsec::api::mist::message::internal::InternalMistMessage;

	// defined, but not implemented
	Message();


	gmsec::api::internal::InternalMessage* m_iMessage;
};

} // namespace api
} // namespace gmsec

#endif
