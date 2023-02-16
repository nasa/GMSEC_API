/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ResourceGenerator.h
 *
 * @brief Utility for starting a service that periodically publishes
 * Resource Messages to the GMSEC Bus.
 */

#ifndef GMSEC_API5_RESOURCE_GENERATOR_H
#define GMSEC_API5_RESOURCE_GENERATOR_H

#include <gmsec5/Message.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/wdllexp.h>

#include <gmsec5_defs.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Field;
	class MessageFactory;

	namespace internal
	{
	class InternalResourceGenerator;
	}

/**
 * @class ResourceGenerator
 *
 * @brief This class can be used to start a thread that will continuously publish Resource Messages
 * using a Connection object.
 *
 * @sa Config
 * @sa gmsec::api5::Connection
 * @sa gmsec::api5::Message
 */ 
class GMSEC_API ResourceGenerator
{
public:
	/**
	 * @fn ResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
	 *
	 * @brief Basic class constructor.
	 *
	 * @param config          - the Config object to configure the Connection that is used to publish messages
	 * @param pubRate         - the message publish rate (in seconds)
	 * @param sampleInterval  - the time distance between sample collection (in seconds)
	 * @param averageInterval - time window over which to make a moving average of samples (in seconds)
	 *
	 * @throw A GmsecException is thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval
	 * @throw A GmsecException is thrown if a Connection object cannot be created
	 *
	 * @note If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @sa setField
	 */
	ResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval);


	/**
	 * @fn ResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval,
	 *                       const gmsec::api5::util::List<Field*>& fields)
	 *
	 * @brief Class constructor where a list of message fields can be provided.
	 *
	 * @param config          - the Config object to configure the Connection that is used to publish messages
	 * @param pubRate         - the message publish rate (in seconds)
	 * @param sampleInterval  - the time distance between sample collection (in seconds)
	 * @param averageInterval - time window over which to make a moving average of samples (in seconds)
	 * @param fields          - fields to add to the resource message
	 *
	 * @throw A GmsecException is thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval
	 * @throw A GmsecException is thrown if a Connection object cannot be created
	 *
	 * @note If the publish rate is set to 0 (zero), the resource generator will only publish one message.
	 *
	 * @sa setField() 
	 */
	ResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval,
	                  const gmsec::api5::util::List<Field*>& fields);


	/**
	 * @fn ~ResourceGenerator()
	 *
	 * @brief Class destructor.
	 *
	 * @note If the resource generator thread is running, and the application neglects to call stop(),
	 * then the resource generator thread will be terminated when the destructor is called.
	 */
	~ResourceGenerator();


	/**
	 * @fn bool start()
	 *
	 * @brief Starts the resource generator thread.
	 *
	 * @return True if the resource generator thread has started; false otherwise.
	 *
	 * @throw A GmsecException is thrown if a connection cannot be established with the GMSEC Bus
	 * @throw A GmsecException is thrown if message validation is enabled and a valid RSRC message cannot be created
	 *
	 * @note If the publish rate is set to 0 (zero), only one resource message will be published,
	 * however the resource generator thread will continue to run.
	 *
	 * @sa stop()
	 */
	bool CALL_TYPE start();


	/**
	 * @fn bool stop()
	 *
	 * @brief Stops the resource generator thread.
	 *
	 * @return True if the resource generator thread has stopped; false otherwise.
	 *
	 * @sa start()
	 */
	bool CALL_TYPE stop();


	/**
	 * @fn bool isRunning()
	 *
	 * @brief Accessor that can be used to query whether the resource generator thread is running.
	 *
	 * @return True if the thread is running; false otherwise.
	 */
	bool CALL_TYPE isRunning();


	/**
	 * @fn bool setField(const Field& field)
	 *
	 * @brief Accessor that will add the given field to resource message.
	 *
	 * @param field - the Field to add to the resource message
	 *
	 * @return True if an existing field was overwritten; false otherwise.
	 *
	 * @throw A GmsecException is thrown if for whatever reason the field cannot be added to the message.
	 *
	 * @note If the PUB-RATE field is provided, then the publish rate for the resource generator will
	 * be updated to publish at the (new) rate.
	 */
	bool CALL_TYPE setField(const Field& field);


	/**
	 * @fn Message createResourceMessage(MessageFactory& factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
	 *
	 * @brief Creates/returns a Resource Message using the given MessageFactory.
	 *
	 * @param factory         - the MessageFactory to reference when creating the resource message
	 * @param sampleInterval  - the time distance between sample collection (in seconds)
	 * @param averageInterval - time window over which to make a moving average of samples (in seconds)
	 *
	 * @return A Message object representing a Resource Message.
	 *
	 * @throw A GmsecException is thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval
	 *
	 * @sa Connection.getMessageFactory()
	 */
	static Message CALL_TYPE createResourceMessage(MessageFactory& factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval);


private:
    // Defined, but not implemented
    ResourceGenerator();
    ResourceGenerator(const ResourceGenerator&);

	gmsec::api5::internal::InternalResourceGenerator* m_internal;
};

} // end namespace api5
} // end namespace gmsec

#endif
