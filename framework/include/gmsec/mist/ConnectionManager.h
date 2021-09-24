/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManager.h
 *
 *  @brief The ConnectionManager is designed to provide several high-level management
 *  functions for operation of a GMSEC connection object. These management functions 
 *  include the application of message content validation, the operation of a
 *  standard service for producing heartbeats, and a set of functions which streamline 
 *  log message production.
 *
**/

#ifndef gmsec_ConnectionManager_h
#define gmsec_ConnectionManager_h

#include <gmsec/Status.h>

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>

#include <stddef.h>   // for size_t


namespace gmsec
{

class Config;
class Connection;
class Field;
class Message;

namespace mist
{
class ConnectionManagerCallback;
class ConnectionManagerErrorCallback;
class ConnectionManagerReplyCallback;

namespace internal
{
class BaseConnectionManager;
class CMCallback;
class CMErrorCallback;
class CMReplyCallback;
class HeartbeatService;
class Specification;
}

/** @class ConnectionManager
 * @brief This class wraps the GMSEC Connection object, supplying both normal
 * middleware interaction functions and some additional quality of service. 
 * This is accomplished through standard services for producing Log and 
 * Heartbeat messages, as well as functionality for validating messages. 
 * 
 * If the object is configured to validate messages, all messages sent 
 * from this ConnectionManager via either Publish or Request will be compared
 * to an available schema definition. If a template definition for the message
 * in question is available, it will be validated. The message will not be sent
 * over the middleware if validation fails. Messages without a template
 * definition can be sent as well.
 *
 * The following messages are currently supported, and the Fields required for 
 * the ConnectionManager to recognize them are listed as well.
 *
 * GMSEC Heartbeat Message
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "C2CX"
 * C2CX-SUBTYPE    = "HB"
 *
 * GMSEC Log Message
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "LOG"
 *
 * GMSEC Directive Request Message
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "DIR"
 *
 * GMSEC C2CX Configuration Message
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "C2CX"
 * C2CX-SUBTYPE    = "CFG"
 *
 * GMSEC C2CX Control Message
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "C2CX"
 * C2CX-SUBTYPE    = "CNTL"
 *
 * GMSEC C2CX Device Message
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "C2CX"
 * C2CX-SUBTYPE    = "DEV"
 *
 * GMSEC C2CX Resource Message
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "C2CX"
 * C2CX-SUBTYPE    = "RSRC"
 *
 * GMSEC Product Message for Archive and Assessment
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "AAA"
 *
 * GMSEC Product Message for Automation
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "AUTO"
 *
 * GMSEC Product Message for Flight Dynamics
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "FD"
 *
 * GMSEC Product Message for Modeling, Simulation, and Front End Processors
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "MAS"
 *
 * GMSEC Product Message for Planning and Scheduling
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "PAS"
 *
 * GMSEC Product Message for Script Control
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "SC"
 *
 * GMSEC Product Message for Telemetry and Command
 * MESSAGE-TYPE    = "MSG"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "TAC"
 *
 * GMSEC Product Request Message for Archive and Assessment
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "AAA"
 *
 * GMSEC Product Request Message for Automation
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "AUTO"
 *
 * GMSEC Product Request Message for Flight Dynamics
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "FD"
 *
 * GMSEC Product Request Message for Modeling, Simulation, and Front End Processors
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "MAS"
 *
 * GMSEC Product Request Message for Planning and Scheduling
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "PAS"
 *
 * GMSEC Product Request Message for Script Control
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "SC"
 *
 * GMSEC Product Request Message for Telemetry and Command
 * MESSAGE-TYPE    = "REQ"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "TAC"
 *
 * GMSEC Product Response Message for Archive and Assessment
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "AAA"
 *
 * GMSEC Product Response Message for Automation
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "AUTO"
 *
 * GMSEC Product Response Message for Flight Dynamics
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "FD"
 *
 * GMSEC Product Response Message for Modeling, Simulation, and Front End Processors
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "MAS"
 *
 * GMSEC Product Response Message for Planning and Scheduling
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "PAS"
 *
 * GMSEC Product Response Message for Script Control
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "SC"
 *
 * GMSEC Product Response Message for Telemetry and Command
 * MESSAGE-TYPE    = "RESP"
 * MESSAGE-SUBTYPE = "PROD"
 * PROD-TYPE       = "TAC"
 *
 * @sa Connection @n
 *		Config
*/
class GMSEC_API ConnectionManager
{
	public:
		/**
		 * @brief 2014 version of the GMSEC Interface Specification Document (ISD)
		 */
		static const unsigned int GMSEC_ISD_2014_00 = 201400;

		/**
		 * @brief Current version of the GMSEC Interface Specification Document (ISD)
		*/
		static const unsigned int GMSEC_ISD_CURRENT = GMSEC_ISD_2014_00;



		/** @fn ConnectionManager(Config &cfg, unsigned int version = GMSEC_ISD_CURRENT, bool validate = false)
		 * @brief Constructor - Initializes the ConnectionManager instance without
		 * attempting a connection to the middleware.
		 *
		 * @param cfg - The config object used to construct the underlying 
		 * GMSEC connection object
		 * @param version - the version of the GMSEC message specification to be used
		 * in validating messages (default is GMSEC_ISD_CURRENT)
		 * @param validate - boolean flag to indicate whether the ConnectionManager
		 * should validate messages produced (default is false)
		 *
		 */
		GMSEC_DEPRECATED ConnectionManager(Config &cfg, unsigned int version = GMSEC_ISD_CURRENT, bool validate = false);

		/** @fn ~ConnectionManager()
		 * @brief Destructor - tears down the ConnectionManager.
		 * If "Cleanup" had not been called already, the function invokes that function.
		 *
		 * @sa ConnectionManager::Cleanup() @n
		 */
		~ConnectionManager();

		/** @fn SetStandardFields(Field* standardFields[], size_t fieldCount)
		 * @brief SetStandardFields - SetStandardFields sets the internal list of fields
		 * which are to be automatically placed in all messages sent by this ConnectionManager. 
		 * Internal copies of the Fields are made, ownership is not retained by the 
		 * ConnectionManager. The supplied set of fields will not be validated here, validation
		 * occurs at the time a message is to be published. 
		 *
		 * @param standardFields - The array of fields to be copied to the internal
		 * set of fields to be appended to all messages.
		 * @param fieldCount - The number of elements in standardFields
		 */
		Status CALL_TYPE SetStandardFields(Field* standardFields[], size_t fieldCount);

		/** @fn Initialize()
		 * @brief Initialize - Uses the config object supplied in the 
		 * constructor to establish a connection with the defined GMSEC middleware server.
		 * The underlying connection object is created and connected in one operation, 
		 * returning an error status if either operation is a failure. Once this call
		 * successfully returns, the ConnectionManager is ready for messagin operations.
		 *
		 * Behind the scenes, the ConnectionManager is calling the
		 * ConnectionFactory::CreateConnection function and Connection::Connect.
		 * If either operation fails, the results will be propagated to the user.
		 *
		 * @sa Connection::Connect() @n
		 */
		Status CALL_TYPE Initialize();

		/** @fn Cleanup()
		 * @brief Cleanup - This function disconnects and destroys the 
		 * underlying Connection object, returning an error status if this operation is
		 * not successful.
		 *
		 * Behind the scenes, the ConnectionManager is calling the
		 * Connection::Disconnect and ConnectionFactory::DestroyConnection function.
		 * If either operation fails, the results will be propagated to the user.
		 *
		 * @sa Connection::Disconnect() @n
		 */
		Status CALL_TYPE Cleanup();

		/** @fn RegisterErrorCallback(const char *event, ConnectionManagerErrorCallback *cb)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param event - name of event to register
		 * @param cb - object derrived from ErrorCallback to register for this error event
		 * @return status - result of the connection operation
		 *
		 * @sa Connection::RegisterErrorCallback(const char *event, ErrorCallback *cb) @n
		 */
		Status CALL_TYPE RegisterErrorCallback(const char *event, ConnectionManagerErrorCallback *cb);


		/** @fn Subscribe(const char *subject)
		 * @brief This is a pass-through function to the underlying Connection.
		 *
		 * @param subject - subject pattern to match received messages
		 * @return status - result of the connection operation
		 */
		Status CALL_TYPE Subscribe(const char *subject);


		/** @fn Subscribe(const char *subject, ConnectionManagerCallback *cb)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param subject - subject pattern to match received messages
		 * @param cb - callback to be called when message is received
		 * @return status - result of the connection operation
		 */
		Status CALL_TYPE Subscribe(const char *subject, ConnectionManagerCallback *cb);


		/** @fn Subscribe(const char *subject, const Config &config)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param subject - subject pattern to match received messages
		 * @param config - config object to be used for subscription operation
		 * @return status - result of the connection operation
		 */
		Status CALL_TYPE Subscribe(const char *subject, const Config &config);


		/** @fn UnSubscribe(const char *subject)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param subject - subject pattern that was used to match received messages
		 * @return status - result of the connection operation
		 */
		Status CALL_TYPE UnSubscribe(const char *subject);


		/** @fn UnSubscribe(const char *subject, ConnectionManagerCallback *cb)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param subject - subject pattern to match received messages
		 * @param cb - callback to be called when message is received
		 * @return status - result of the connection operation
		 */
		Status CALL_TYPE UnSubscribe(const char *subject, ConnectionManagerCallback *cb);


		/** @fn CreateMessage(const char* subject, GMSEC_MSG_KIND kind, Message *&msg)
		 * @brief This function creates a message via a pass-through to the underlying
		 * connection, and then pre-populations that message with any fields set in the
		 * via SetStandardFields. The message is NOT validated at that time.
		 *
		 * @param subject - The GMSEC subject to be used for the message.
		 * @param kind - The GMSEC message kind
		 * @param msg - The pointer which will be modified to point to the created
		 * GMSEC message object
		 *
		 * @sa Connection::CreateMessage(const char *subject, GMSEC_MSG_KIND kind, Message *&msg)
		 */
		Status CALL_TYPE CreateMessage(const char* subject, GMSEC_MSG_KIND kind, Message *&msg);

		/** @fn ConvertMessage(Message *in, Message *&out)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param in - message to convert FROM
		 * @param out - out parameter filled with the coverted message (could be the same if conversion wasn't necessary)
		 * @return status - result of the connection operation
		 *
		 * @sa Connection::ConvertMessage(Message *in, Message *&out) @n
		 */
		Status CALL_TYPE ConvertMessage(Message *in, Message *&out);

		/** @fn ConvertCleanup(Message *in, Message *out)
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param in - message that was originally coverted FROM
		 * @param out - message that was output by the ConvertMessage() call
		 * @return status - result of the connection operation
		 *
		 * @sa Connection::ConvertCleanup(Message *in, Message *&out) @n
		 */
		Status CALL_TYPE ConvertCleanup(Message *in, Message *out);

		/** @fn CloneMessage( Message *in, Message *&out )
		 * @brief This is a pass-through function to the underlying connection.
		 *
		 * @param in - message to be cloned
		 * @param out - cloned message
		 * @return status - result of the connection operation
		 *
		 * @sa Connection::CloneMessage(Message *in, Message *&out) @n
		 */
		Status CALL_TYPE CloneMessage(Message *in, Message *&out);

		/** @fn DestroyMessage(Message *msg)
		 * @brief This function destroys a message and cleans up any associated memory.
		 *
		 * @param msg - message to be destroyed
		 * @return status - result of the connection operation
		 *
		 * @sa Connection::DestroyMessage(Message *msg) @n
		 */
		Status CALL_TYPE DestroyMessage(Message *msg);

		/** @fn Publish(Message *msg)
		 * @brief Publish - If this connection manager has been created with "validate"
		 * option disabled, this is a pass-through function to the underlying connection.
		 * However, if this BaseConnectionManager is set to validate its messages, the message
		 * will be validated. If a valid schema definition is available for the type of message
		 * and an error is detected during validation, an error Status will be returned.
		 *
		 * Any Connection errors occuring during the attempt to Publish will also
		 * be passed through.
		 *
		 * @param msg - The GMSEC message to be published.
		 *
		 * @sa Connection::Publish(Message *msg) @n
		 */
		Status CALL_TYPE Publish(Message *msg);

		/** @fn Publish(Message *msg, const Config &config)
		 * @brief Publish - If this connection manager has been created with "validate"
		 * option disabled, this is a pass-through function to the underlying connection.
		 * However, if this BaseConnectionManager is set to validate its messages, the message
		 * will be validated. If a valid schema definition is available for the type of message
		 * and an error is detected during validation, an error Status will be returned.
		 *
		 * @param msg - The GMSEC message to be published
		 * @param config - config object to be used by the publish operation
		 * @return status - result of the connection operation
		 *
		 * @sa Connection::Publish(Message *msg, const Config &config) @n
		 */
		Status CALL_TYPE Publish(Message *msg, const Config &config);

		/** @fn Request(Message *request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
		 * @brief Request - If this connection manager has been created with "validate"
		 * option disabled, this is a pass-through function to the underlying connection.
		 * However, if this BaseConnectionManager is set to validate its messages, the message
		 * will be validated. If a valid schema definition is available for the type of message
		 * and an error is detected during validation, an error Status will be returned.
		 *
		 * @param request - message to be sent
		 * @param timeout - maximum time to wait for reply (in milliseconds)
		 * @param cb - callback to call when reply is receieved
		 * @return status - result of the request operation
		 *
		 * @sa Connection::Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER) @n
		 */
		Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, ConnectionManagerReplyCallback *cb, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);

		/** @fn Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER)
		 * @brief Request - If this connection manager has been created with "validate"
		 * option disabled, this is a pass-through function to the underlying connection.
		 * However, if this BaseConnectionManager is set to validate its messages, the message
		 * will be validated. If a valid schema definition is available for the type of message
		 * and an error is detected during validation, an error Status will be returned.
		 *
		 * @param request - message to be sent
		 * @param timeout - maximum time to wait for reply (in milliseconds)
		 * @param reply - out parameter reply message if received
		 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative value
		 * (eg. -1) it will never republish a request message. If set to 0 it will default to the user-specified default
		 * value supplied at the Connection object's creation time.
		 * @return status - result of the request operation
		 *
		 * @sa Connection::Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER) @n
		 */
		Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms = GMSEC_REQUEST_REPUBLISH_NEVER);

		/** @fn Reply(Message *request,Message *reply)
		 * @brief Reply - If this connection manager has been created with "validate"
		 * option disabled, this is a pass-through function to the underlying connection.
		 * However, if this BaseConnectionManager is set to validate its messages, the message
		 * will be validated. If a valid schema definition is available for the type of message
		 * and an error is detected during validation, an error Status will be returned.
		 *
		 * @param request - the recieved request that we are responding to
		 * @param reply - the reply to be sent
		 * @param republish_ms - request message resubmission interval (in milliseconds). If set to a negative value
		 * (eg. -1) it will never republish a request message. If set to 0 it will default to the user-specified default
		 * value supplied at the Connection object's creation time.
		 * @return status - result of the reply operation
		 *
		 * @sa Connection::Reply(Message *request, Message *reply) @n
		 */
		Status CALL_TYPE Reply(Message *request, Message *reply);

		/** @fn StartAutoDispatch()
		 * @brief This function is a pass-through function to the underlying connection.
		 * It will start a thread that will dispatch messages asynchronously when they are received.
		 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped.
		 * If GetNextMessage() is called while the auto-dispatcher is used, the behavior will be undesireable
		 * and undefined.
		 *
		 * @return status - result of the start operation
		 *
		 * @sa Connection::StartAutoDispatch() @n
		 */
		Status CALL_TYPE StartAutoDispatch();

		/** @fn StopAutoDispatch()
		 * @brief This function will stop the auto dispatch thread.
		 *
		 * @return status - result of the stop operation
		 *
		 * @sa Connection::StopAutoDispatch() @n
		 */
		Status CALL_TYPE StopAutoDispatch();

		/** @fn StopAutoDispatch(bool waitForComplete)
		 * @brief This function will stop the auto dispatch thread.
		 *
		 * @param waitForComplete - Signal the function whether or not to wait
		 * for the dispatcher to finish operations.
		 * @return status - result of the stop operation
		 *
		 * @sa Connection::StopAutoDispatch(bool waitForComplete) @n
		 */
		Status CALL_TYPE StopAutoDispatch(bool waitForComplete);

		/** @fn GetNextMsg(Message *&msg, GMSEC_I32 timeout)
		 * @brief This function is a pass-through function to the underlying connection.
		 * This function returns the next message received within the specified timeout.
		 * The received messages are determined by the %Subscribe() function(s), but
		 * %DispatchMsg() needs to be called messages received from this function to
		 * ensure all registered callbacks are executed. @n
		 * @n
		 * This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
		 *
		 * @param msg - out parameter, the next received message, if any
		 * @param timeout - the maximum time to block waiting for a message, in milliseconds
		 * @return status - result of the operation
		 *
		 * @sa Connection::GetNextMsg(Message *&msg, GMSEC_I32 timeout) @n
		 */
		Status CALL_TYPE GetNextMsg(Message *&msg, GMSEC_I32 timeout);

		/** @fn DispatchMsg(Message *msg)
		 * @brief This is a pass-through function to the underlying Connection.
		 *
		 * @param msg - message to be dispatched
		 * @return status - result of the operation
		 *
		 * @sa Connection::DispatchMsg(Message *&msg)
		 */
		Status CALL_TYPE DispatchMsg(Message *msg);

		/** @fn GetLastDispatcherStatus()
		 * @brief This is a pass-through function to the underlying Connection.
		 *
		 * @sa Connection::GetLastDispatchStatus() @n
		 *
		 */
		Status CALL_TYPE GetLastDispatcherStatus();

		/** @fn ExcludeSubject(const char* subject)
		 * @brief This is a pass-through function to the underlying Connection.
		 *
		 * @sa Connection::ExcludeSubject(const char* subject) @n
		 */
		Status CALL_TYPE ExcludeSubject(const char* subject);

		/** @fn RemoveExcludedSubject(const char* subject)
		 * @brief This is a pass-through function to the underlying Connection.
		 *
		 * @sa Connection::ExcludeSubject(const char* subject) @n
		 */
		Status CALL_TYPE RemoveExcludedSubject(const char* subject);

		/***************Start Heartbeat Section************************************************/

		/** @fn GetHeartbeatMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char* subject)
		 * @brief GetHeartbeatMessage - GetHeartbeatMessage creates a message and passes
		 * ownership to the user. This message is populated with the standard set of required
		 * and optional heartbeat fields, as well as the required common fields defined in
		 * SetStandardFields. If validation is enabled for this ConnectionManager and neither
		 * the common fields from SetStandardFields, nor the fields supplied in the first
		 * argument of this function are sufficient to complete a set of fields required by
		 * validation, an error will be returned.
		 *
		 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
		 * added to the message automatically, according to the GMSEC Message Standard
		 *
		 * @param standardFields - Set of fields to be added to the heartbeat
		 * @param fieldCount - number of fields in standardFields
		 * @param msg - The pointer to be modified to point to the new message
		 * @param subject - The topic (subject) that will be applied to the returned messages.
		 */
		Status CALL_TYPE GetHeartbeatMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject);

		/** @fn StartHeartbeatService(Field * standardFields[], size_t fieldCount, const char * subject)
		 * @brief StartHeartbeatService - This function creates an archetypal 
		 * heartbeat message from the concatenation of the standard fields
		 * applied globally to this Connection Manager and to this function. This
		 * message will then be validated, if the "validate" flag is set. If there
		 * is an error, an error status will be returned. If it has
		 * not been set, the message will be published at an interval supplied
		 * by the "PUB-RATE" field regardless of validation results. If no "PUB-RATE"
		 * has been defined, the service will default to the GMSEC standard 30 second
		 * heartbeat interval.
		 *
		 * MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE fields will all be generated and
		 * added to the message automatically, according to the GMSEC Message Standard
		 *
		 * @param standardFields - Set of fields to be added to the heartbeat
		 * @param fieldCount - number of fields in standardFields
		 * @param subject - subject to set as the default for heartbeat messages
		 */
		Status CALL_TYPE StartHeartbeatService(Field * standardFields[], size_t fieldCount, const char * subject);

		/** @fn ChangeComponentStatus(const Field &componentStatus)
		 * @brief ChangeComponentStatus - This function updates the field which the heartbeat
		 * service uses to indicate component status. If validate is enabled, the message
		 * will then be re-validated with the modified field set. Should the validation
		 * pass, the change will be persisted to the heartbeat service, otherwise
		 * an error will be returned.
		 *
		 * @param componentStatus - Field representing the component status
		 */
		Status CALL_TYPE ChangeComponentStatus(const Field &componentStatus);

		/** @fn ChangeComponentInfo(const Field &componentInfo)
		 * @brief ChangeComponentInfo - This function updates the field which the heartbeat
		 * service uses to indicate component information. If validate is enabled, the message
		 * will then be re-validated with the modified field set. Should the validation
		 * pass, the change will be persisted to the heartbeat service, otherwise
		 * an error will be returned.
		 *
		 * @param componentInfo - Field representing the component information
		 */
		Status CALL_TYPE ChangeComponentInfo(const Field &componentInfo);

		/** @fn ChangeCPUMemory(const Field &cpuMemory)
		 * @brief ChangeCPUMemory - This function updates the field which the heartbeat
		 * service uses to indicate CPU memory. If validate is enabled, the message
		 * will then be re-validated with the modified field set. Should the validation
		 * pass, the change will be persisted to the heartbeat service, otherwise
		 * an error will be returned.
		 *
		 * @param cpuMemory - Field representing the CPU memory
		 */
		Status CALL_TYPE ChangeCPUMemory(const Field &cpuMemory);

		/** @fn ChangeCPUUtil(const Field &cpuUtil)
		 * @brief ChangeCPUUtil - This function updates the field which the heartbeat
		 * service uses to indicate CPU utilization. If validate is enabled, the message
		 * will then be re-validated with the modified field set. Should the validation
		 * pass, the change will be persisted to the heartbeat service, otherwise
		 * an error will be returned.
		 *
		 * @param cpuUtil - Field representing the CPU utilization
		 */
		Status CALL_TYPE ChangeCPUUtil(const Field &cpuUtil);
		
		/** @fn StopHeartbeatService()
		 * @brief StopHeartbeatService - This function terminates opertaion of the heartbeat
		 * service and cleans up related resources. It will be called by the global
		 * connection manager "cleanup" if not done manually by the user.
		 */
		Status CALL_TYPE StopHeartbeatService();

		/***************End Heartbeat Section************************************************/

		/***************Start Log Section************************************************/

		/** @fn GetLogMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char* subject)
		 * @brief GetLogMessage - GetLogMessage creates a message and passes
		 * ownership to the user. This message is populated with the standard set of required
		 * and optional log fields, as well as the required common fields defined in
		 * SetStandardFields. The message is not validated at this time, as MSG-TEXT and SEVERITY
		 * fields must be set by the user at the time the message is to be sent.
		 *
		 * This message automatically is generated with MESSAGE-TYPE and MESSAGE-SUBTYPE
		 * correctly set to the GMSEC Message Standard requirements, regardless of user
		 * input. EVENT-TIME defaults to the time which this function was invoked; the user
		 * may override this time once the message has been returned.
		 *
		 * @param standardFields - Set of fields to be added to the heartbeat
		 * @param fieldCount - number of fields in standardFields
		 * @param msg - The pointer to be modified to point to the new message
		 * @param subject - The topic (subject) to apply to the returned message.
		 */
		Status CALL_TYPE GetLogMessage(Field * standardFields[], size_t fieldCount, Message *&msg, const char * subject);

		/** @fn SetLoggingDefaults(Field * standardFields[], size_t fieldCount, const char * subject)
		 * @brief SetLoggingDefaults - This function sets the standard default
		 * fields which will be added to a new log message as part of "QuickLog"
		 *
		 * @param standardFields - Set of fields to be added to the heartbeat
		 * @param fieldCount - number of fields in standardFields
		 * @param subject - subject to use for log messages
		 */
		Status CALL_TYPE SetLoggingDefaults(Field * standardFields[], size_t fieldCount, const char * subject);

		/** @fn QuickLog(const char* msg, Field &severity)
		 * @brief QuickLog - This function creates a log message comprised of the 
		 * global fields from the entire ConnectionManager, the fields set in
		 * "SetLoggingDefaults", and the log text and severity fields as supplied
		 * as arguments to this function. If validation is enabled, the message
		 * will be published. If validation is enabled, the message will be validated,
		 * returning an argument if validation is not successful, before it
		 * is published.
		 *
		 * This function automatically populates the message with the correct
		 * MESSAGE-TYPE and MESSAGE-SUBTYPE fields. EVENT-TIME is set automatically
		 * to the time of function invocation. 
		 *
		 * @param msg - the message to log
		 * @param severity - the Field object defining the severity of the log message.
		 */
		Status CALL_TYPE QuickLog(const char* msg, const Field &severity);

		/***************End Log Section************************************************/

	private:
		// not implemented
		ConnectionManager &operator=(const ConnectionManager &);
		ConnectionManager(const ConnectionManager &);
		ConnectionManager();

		mist::internal::BaseConnectionManager *mgr;

		friend class gmsec::mist::internal::CMCallback;
		friend class gmsec::mist::internal::CMErrorCallback;
		friend class gmsec::mist::internal::CMReplyCallback;

		// Private constructor that is to be used by only the friend classes
		// defined above.
		//
		ConnectionManager(mist::internal::BaseConnectionManager* baseMgr);

		// The purpose of the following boolean is to record whether the
		// ConnectionManager object owns the BaseConnectionManager
		// instance that it uses.
		//
		// In cases where callbacks are used, the callback only has a
		// reference to the BaseConnectionManager (BCM), but needs to pass on
		// a reference to a ConnectionManager (CM).  Using the BCM, a CM object
		// can be constructed (using the private constructor declared above)
		// which that does not take ownership of the BCM.
		//
		// Once the OnMessage/OnReply/OnError method associated with the callback
		// has been called with the temporary CM, the CM can be destroyed without
		// the risk of the BCM being destroyed as well.
		//
		bool own;
	};
}

}

#endif
