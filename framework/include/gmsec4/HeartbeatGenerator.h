/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file HeartbeatGenerator.h
 *
 *  @brief This file contains a class for starting a heartbeat message generator.
 */

#ifndef GMSEC_API_HEARTBEAT_GENERATOR_H
#define GMSEC_API_HEARTBEAT_GENERATOR_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/util/DataList.h>

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Config;
class Field;

namespace internal
{
	class InternalHeartbeatGenerator;
}

/**
 * @class HeartbeatGenerator
 *
 * @brief This class can be used to start a thread that will continuously publish C2CX HB messages.
 *
 * The Heartbeat Generator creates a C2CX HB message using MistMessage. It then uses a
 * ConnectionManager to publish this message on a periodic basis.
 *
 * @sa Config
 * @sa gmsec::api::mist::ConnectionManager
 * @sa gmsec::api::mist::message::MistMessage
 */ 
class GMSEC_API HeartbeatGenerator
{
public:
	/**
	 * @fn HeartbeatGenerator(const Config& config, const char* hbMsgSubject, GMSEC_U16 hbPubRate)
	 *
	 * @brief Basic class constructor.
	 *
	 * @param config - the Config object to configure the ConnectionManager that is used to publish messages
	 * @param hbMsgSubject - the subject to include with the published heartbeat messages
	 * @param hbPubRate - the message publish rate (in seconds)
	 *
	 * @throw An Exception is thrown if message subject is NULL or contains an empty string
	 * @throw An Exception is thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus
	 * @throw An Exception is thrown if message validation is enabled and a valid C2CX HB message cannot be created
	 *
	 * @note If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 * @note Consider using gmsec::api::mist::message::MistMessage::setStandardFields() to automatically include
	 * standard field(s) within the heartbeat message.
	 *
	 * @sa setField
	 * @sa gmsec::api::mist::message::MistMessage::setStandardFields
	 */
	HeartbeatGenerator(const Config& config, const char* hbMsgSubject, GMSEC_U16 hbPubRate);


	/**
	 * @fn HeartbeatGenerator(const Config& config, const char* hbMsgSubject, GMSEC_U16 hbPubRate, const gmsec::api::util::DataList<Field*>& fields)
	 *
	 * @brief Class constructor where a list of message fields can be provided.
	 *
	 * @param config - the Config object to configure the ConnectionManager that is used to publish messages
	 * @param hbMsgSubject - the subject to include with the published heartbeat messages
	 * @param hbPubRate - the message publish rate (in seconds)
	 * @param fields - fields to add to the heartbeat message
	 *
	 * @throw An Exception is thrown if message subject is NULL or contains an empty string
	 * @throw An Exception is thrown if the ConnectionManager cannot be created or connect to the GMSEC Bus
	 * @throw An Exception is thrown if message validation is enabled and a valid C2CX HB message cannot be created
	 *
	 * @note If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
	 *
	 * @sa setField() 
	 * @sa gmsec::api::mist::message::MistMessage::setStandardFields
	 */
	HeartbeatGenerator(const Config& config, const char* hbMsgSubject, GMSEC_U16 hbPubRate, const gmsec::api::util::DataList<Field*>& fields);


	/**
	 * @fn ~HeartbeatGenerator()
	 *
	 * @brief Class destructor.
	 *
	 * @Note If the heartbeat generator thread is running, and the application fails to call stop(), then the heartbeat generator thread will be
	 * terminated when the destructor is called.
	 */
	~HeartbeatGenerator();


	/**
	 * @fn bool start()
	 *
	 * @brief Starts the heartbeat generator thread.
	 *
	 * @return True if the heartbeat generator thread has started; false otherwise.
	 *
	 * @note If the publish rate is set to 0 (zero), only one heartbeat message will be published,
	 * however the heartbeat generator thread will continue to run.
	 *
	 * @sa stop()
	 */
	bool CALL_TYPE start();


	/**
	 * @fn bool stop()
	 *
	 * @brief Stops the heartbeat generator thread.
	 *
	 * @return True if the heartbeat generator thread has stopped; false otherwise.
	 *
	 * @sa start()
	 */
	bool CALL_TYPE stop();


	/**
	 * @fn bool isRunning()
	 *
	 * @brief Accessor that can be used to query whether the heartbeat generator thread is running.
	 *
	 * @return True if the thread is running; false otherwise.
	 */
	bool CALL_TYPE isRunning();


	/**
	 * @fn void changePublishRate(GMSEC_U16 pubRate)
	 *
	 * @brief Accessor for changing the publish rate of the heartbeat message generator.
	 *
	 * @param pubRate - the new publish rate (in seconds)
	 *
	 * @note If the publish rate is set to 0 (zero), only one additional heartbeat message
	 * will be published.
	 */
	void CALL_TYPE changePublishRate(GMSEC_U16 pubRate);


	/**
	 * @fn bool setField(const Field& field)
	 *
	 * @brief Accessor that will add the given field to heartbeat message.
	 *
	 * @param field - the Field to add to the heartbeat message
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throw An Exception is thrown if for whatever reason the field cannot be added to the message.
	 *
	 * @note If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will
	 * be updated to publish at the (new) rate.
	 */
	bool CALL_TYPE setField(const Field& field);


	/**
	 * @fn bool setField(const char* fieldName, GMSEC_I64 fieldValue)
	 *
	 * @brief Accessor that will smartly add a Field to heartbeat message.
	 *
	 * @param fieldName - the name of the field
	 * @param fieldValue - the value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throw An Exception is thrown if the field name is NULL or contains an empty string.
	 * @throw An Exception is thrown if for whatever reason the field cannot be added to the message.
	 *
	 * @note This method relies on the usage of gmsec::api::mist::message::MistMessage::setValue(const char*, GMSEC_I64).
	 * @note If the PUB-RATE field is provided, the publish rate for the heartbeat generator will
	 * be updated with the (presumably) new publish rate.
	 */
	bool CALL_TYPE setField(const char* fieldName, GMSEC_I64 fieldValue);


	/**
	 * @fn bool setField(const char* fieldName, GMSEC_F64 fieldValue)
	 *
	 * @brief Accessor that will smartly add a Field to heartbeat message.
	 *
	 * @param fieldName - the name of the field
	 * @param fieldValue - the value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throw An Exception is thrown if the field name is NULL or contains an empty string.
	 * @throw An Exception is thrown if for whatever reason the field cannot be added to the message.
	 *
	 * @note This method relies on the usage of gmsec::api::mist::message::MistMessage::setValue(const char*, GMSEC_F64).
	 * @note If the PUB-RATE field is provided, the publish rate for the heartbeat generator will
	 * be updated with the (presumably) new publish rate.
	 */
	bool CALL_TYPE setField(const char* fieldName, GMSEC_F64 fieldValue);


	/**
	 * @fn bool setField(const char* fieldName, const char* fieldValue);
	 *
	 * @brief Accessor that will smartly add a Field to heartbeat message.
	 *
	 * @param fieldName - the name of the field
	 * @param fieldValue - the value of the field
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throw An Exception is thrown if the field name is NULL or contains an empty string.
	 * @throw An Exception is thrown if the field value is NULL.
	 * @throw An Exception is thrown if for whatever reason the field cannot be added to the message.
	 *
	 * @note This method relies on the usage of gmsec::api::mist::message::MistMessage::setValue(const char*, const char*).
	 * @note If the PUB-RATE field is provided, the publish rate for the heartbeat generator will
	 * be updated with the (presumably) new publish rate.
	 */
	bool CALL_TYPE setField(const char* fieldName, const char* fieldValue);


private:
    // Defined, but not implemented
    HeartbeatGenerator();
    HeartbeatGenerator(const HeartbeatGenerator&);

	gmsec::api::internal::InternalHeartbeatGenerator* m_internal;
};

} // end namespace api
} // end namespace gmsec

#endif
