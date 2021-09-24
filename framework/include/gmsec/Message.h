/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Message.h
 *
 *  @brief This file contains the public interface to messages.
 *
 */


#ifndef gmsec_Message_h
#define gmsec_Message_h


#include <gmsec/FieldIterator.h>
#include <gmsec/Config.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

namespace internal
{
class InternalMessage;
class InternalConnection;
class MessageBuddy;
}

/** @class Message
 * @brief The Message object is a container for GMSEC Messages.  The methods of this class
 * allow the construction and manipulation of the data in a message.
 *
 * Example creation & use:
 * @code
 * Status result;
 * Message *msg = NULL;
 *
 * result = conn->CreateMessage("GMSEC.TEST.PUBLISH", GMSEC_MSG_PUBLISH,msg);
 * if( result.isError() )
 *	//handle error
 *
 * // send message
 * result = conn->Publish(msg);
 * if( result.isError() )
 *	//handle error
 *
 * result = conn->DestroyMessage(msg);
 * if( result.isError() )
 *	//handle error
 * @endcode
 *
 * @note Users should note that message fields are not retrieved in a deterministic manner.
 *
 * @sa Connection
 *     Field
*/

class GMSEC_API Message
{
public:

	/** @fn isValid()
	 * @brief This function will check if the message is Valid.
	 *
	 * @note This function has been deprecated; use IsValid() instead.
	 *
	 * @return true if the message is valid
	 *
	 */
	bool CALL_TYPE isValid();

	/** @fn IsValid()
	 * @brief This function will check if the message is Valid.
	 *
	 * @return true if the message is valid
	 *
	 */
	bool CALL_TYPE IsValid();

	/** @fn GetLibraryRootName()
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this message is associated with. This information
	 * must match the library root name of the connection in order to be sent
	 * on that connection.
	 *
	 * @return middleware implementation library root name
	 *
	 * @sa Connection::GetLibraryRootName()
	 */
	const char * CALL_TYPE GetLibraryRootName();

	/** @fn SetConfig(Config *config)
	 * @brief This function sets the Configuration of this message.  This can be used to
	 * pass middleware specific items to the Message object.
	 *
	 * All messages implmentations understand the following config values:
	 *
	 * "subject"	-	sets the message subject
	 *
	 * "kind"		-	sets the message kind
	 *
	 * Configuration values from the passed in config will be copied or set within the message. Therefore
	 * the pased in config WILL NOT BE STORED and is the responsibility of the client program to clean up.
	 *
	 * @param config - configuration object that contains values to use
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetConfig(Config *config);

	/** @fn SetKind(GMSEC_MSG_KIND kind)
	 * @brief This function set the 'kind' of this message.
	 *
	 * @param kind - valid message kind
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetKind(GMSEC_MSG_KIND kind);

	/** @fn GetKind(GMSEC_MSG_KIND &kind)
	 * @brief This function get the 'kind' of this message.
	 *
	 * @param kind - out parameter, current message kind
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetKind(GMSEC_MSG_KIND &kind);

	/** @fn SetSubject(const char *subject)
	 * @brief This function set the subject to publish/request this message upon.
	 *
	 * @param subject - valid message subject name
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetSubject(const char *subject);

	/** @fn GetSubject(const char *&subject)
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @param subject - out parameter, subject of the message
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetSubject(const char *&subject);

	/** @fn ClearFields()
	 * @brief This function will remove all fields form this message.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @return status - result of the operation
	 */
	Status CALL_TYPE ClearFields();

	/** @fn AddField(Field &field)
	 * @brief This function will add a new field to this message. A new field will be added to the message
	 * that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	 * The client program must also clean up that field when no longer needed.
	 *
	  * Example:
	  * @code
	 * Status result;
	  * Field fd;
	  *
	  * //set field name & value
	  * result = fd.SetName("char_field");
	  * if( result.isError() )
	  *	//handle error
	  * result fd.SetValue((GMSEC_CHAR)'c');
	  * if( result.isError() )
	  *	//handle error
	  *
	  * //add to message (will copy)
	  * result = msg->AddField(fd);
	  * if( result.isError() )
	  *	//handle error
	  * @endcode
	 *
	 * @param field - field object to copy info from
	 * @return status - result of the operation
	 */
	Status CALL_TYPE AddField(Field &field);

	/** @fn ClearField(const char *name)
	 * @brief This function will remove a field from this message by name.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param name - name of field to remove
	 * @return status - result of the operation
	 */
	Status CALL_TYPE ClearField(const char *name);

	/** @fn GetField(const char *name, Field &field)
	 * @brief This function will copy a field within a message into a field created by the client program, by name.
	  *
	  * Example:
	  * @code
	  * GMSEC_STRING value = NULL;
	  * Field fd;
	  *
	  * result = msg->GetField("char_field",fd);
	  * if (result.isError())
	  *	//handle error
	  *
	  * result = fd.GetValue(fd, &value);
	  * if (result.isError())
	  *	//handle error
	  *
	  * cout << "value = " << value << endl;
	  * @endcode
	 *
	 * @param name - name of field to get
	 * @param field - out parameter, field object to copy info into
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetField(const char *name, Field &field);

	/** @fn GetFieldCount(GMSEC_I32 &count)
	 * @brief This function gets the number of fields contained in this message.
	 *
	 * @param count - out parameter, number of fields in this message
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetFieldCount(GMSEC_I32 &count);

	/** @fn GetFirstField(Field &field)
	 * @brief This function and GetNextField() can be used to iterate through the fields of a message.
	  *
	  * Example:
	  * @code
	  * Field fd;
	  *
	  * // get first field
	  * result = msg->GetFirstField(fd);
	  * while (!result.isError())
	  * {
	  *	// do something with the field here
	  *
	  *	// get next field
	  *	result = msg->GetNextField(fd);
	  * }
	  * @endcode
	 *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa GetNextField()
	 */
	Status CALL_TYPE GetFirstField(Field &field);

	/** @fn GetNextField(Field &field)
	 * @brief This function and GetFirstField() are used to iterate through the fields of a message.
	 *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa GetFirstField()
	 */
	Status CALL_TYPE GetNextField(Field &field);

	/** @fn CopyFields(Message *toMsg)
	 * @brief This function will copy all fields from this message to a new message in the fastest way possible
	 *
	 * @note The fields are NOT copied by reference. As such, the original message and its fields can be
	 * deleted or modified after it has been copied to the new message.
	 *
	 * @param toMsg - the message to copy fields into
	 * @return status - result of the operation
	 */
	Status CALL_TYPE CopyFields(Message *toMsg);

	/** @fn ToXML( const char *&xml )
	 * @brief This function will dump a message to an xml string. This format is compatible with
	  * the format used by FromXML() and ConfigFile::GetMessage().
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
	  * 	<FIELD TYPE="STRING" NAME="STRING-FIELD">This is a test of the Message::ToXML function</FIELD>
	  * 	<FIELD TYPE="F32" NAME="LONG-FIELD" BITS="42F6E666">123.45</FIELD>
	  * 	<FIELD TYPE="F64" NAME="DOUBLE-FIELD" BITS="4B94216CA930D75A">1.234e56</FIELD>
	  * 	<FIELD TYPE="BIN" NAME="BIN-FIELD">4a4c4d4e4f5051</FIELD>
	  * </MESSAGE>
	  * @endcode
	  *
	  * @param xml - out parameter, xml string
	  * @return status - result of the operation
	  *
	  * @sa ConfigFile::GetMessage()
	 */
	Status CALL_TYPE ToXML(const char *&xml);

	/** @fn FromXML( const char *xml )
	 * @brief This function will set subject, kind, and fields into a message as specified by the passed in
	 * xml string. The format is compatible with what is used by ToXML() and ConfigFile::GetMessage();
	 *
	 * @param xml - xml string to parse
	 * @return status - result of the operation
	 *
	 * @sa ConfigFile::GetMessage()
	 * @sa ToXML(const char *&xml) for the JSON string format for a Message
	 */
	Status CALL_TYPE FromXML(const char *xml);

	/** @fn const char* ToJSON()
	  * @brief This method will generate a JSON formatting string representaton of the Message.
	  * This format is compatible with the format used by FromJSON().
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
	  * @return JSON string representation of the Message
	 */
	const char* CALL_TYPE ToJSON();

	/** @fn FromJSON( const char *json )
	 * @brief This function will set subject, kind, and fields into a message as specified by the passed in
	 * json string. The format is compatible with what is used by ToJSON()
	 *
	  * @param json - json string to parse
	 *  @return status - result of the operation
	 *
	 *  @sa ToJSON(const char *&json) for the JSON string format for a Message
	 */
	Status CALL_TYPE FromJSON(const char *json);

	/** @fn GetMSGSize( GMSEC_U32 &size )
	 * @brief This function will return the physical storage size of the message in bytes.
	 * Please note that additional tracking data fields are added to messages at the
	 * time of publication which will affect the size of the message.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param size - out parameter, size of message in bytes
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetMSGSize(GMSEC_U32 &size);

	/** @fn GetTime()
	 * @brief This function will return the current GMT time/date in GMSEC standard format.
	 * @warning This function is not reentrant and is deprecated.
	 * Please use gmsec::util::{getTime_s/formatTime_s}.
	 *
	 * Example format:
	 * @code
	 * 2004-314-18:56:57.350
	 * @endcode
	 *
	 * @return current date/time in GMSEC standard format
	 */
	GMSEC_DEPRECATED static const char * CALL_TYPE GetTime();

	/** @fn LookupKind( const char *kind )
	 * @brief Lookup a standard message kind identifier by string name. The valid strings
	 * correspond to the last part of the identifier names. ex. "PUBLISH" => GMSEC_MSG_PUBLISH
	 *
	 * @param kind - string that matches a known GMSEC message type
	 * @return GMSEC message kind identifier, or GMSEC_MSG_UNSET
	 *
	 * @sa GMSEC_MSG_KIND
	 */
	GMSEC_DEPRECATED static GMSEC_MSG_KIND LookupKind(const char *kind);

	/** @fn Reset(FieldIterator &iter)
	 * @brief Resets a field iterator object to start from the beginning of a message's
	 * fields.
	 *
	 * @param iter - The FieldIterator object
	 * @return status - result of the operation
	 *
	 * @sa GMSEC_MSG_KIND
	 */
	Status CALL_TYPE Reset(FieldIterator &iter);

#ifdef GMSEC_EXPOSE_TRACKING
	/** @fn SetTracking( int flag )
	 * @brief Turn tracking fields on/off for this Message
	 */
	void SetTracking(int flag);
	void SetTrackingNode(int flag);
	void SetTrackingProcessId(int flag);
	void SetTrackingUserName(int flag);
	void SetTrackingConnectionId(int flag);
	void SetTrackingPublishTime(int flag);
	void SetTrackingUniqueId(int flag);
	void SetTrackingMwInfo(int flag);

	/** @fn GetTracking()
	 * @brief Get the tracking fields state for this Message
	 * @return (MESSAGE_TRACKINGFIELDS_UNSET,MESSAGE_TRACKINGFIELDS_ON,MESSAGE_TRACKINGFIELDS_OFF)
	 */
	int GetTracking();
	int GetTrackingNode();
	int GetTrackingProcessId();
	int GetTrackingUserName();
	int GetTrackingConnectionId();
	int GetTrackingPublishTime();
	int GetTrackingUniqueId();
	int GetTrackingMwInfo();
#endif /* GMSEC_EXPOSE_TRACKING */

	/* Unfortunately, some compilers we currently support (gcc 3.2.3) will
	not allow some things that need to be done if the constructor is locked
	down.
	*/

private:
	// not implemented
	Message &operator=(const Message &);
	Message(const Message &);

	/** @fn Message()
	 * @brief Default constructor - initializes the message instance
	 *
	 */
	Message();

	Message(const char* subject, GMSEC_MSG_KIND msgKind);
	Message(const char* subject, GMSEC_MSG_KIND msgKind, Config* config);
	Message(internal::InternalMessage* p);


	/** @fn ~Message()
	 * @brief Destructor - cleans up the message instance
	 */
	~Message();


	friend class internal::MessageBuddy;
	friend class internal::InternalConnection;

	internal::InternalMessage *ptr;
};

} //namespace gmsec

#endif /* gmsec_Message_h */
