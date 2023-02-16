/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFactory.h
 *
 * @brief This file contains the public interface for the MessageFactory.
 */

#ifndef GMSEC_API5_MESSAGE_FACTORY_H
#define GMSEC_API5_MESSAGE_FACTORY_H

#include <gmsec5_defs.h>

#include <gmsec5/c/message.h>

#include <gmsec5/util/List.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Field;
	class Message;
	class MessageValidator;
	class Specification;

	namespace internal
	{
	class InternalMessageFactory;
	class MessageFactoryBuddy;
	}


/**
 * @class MessageFactory
 *
 * @brief The %MessageFactory is used to create a Message object based on
 * an optionally provided schema ID associated with a message template
 * derived from the configured message specification.
 *
 * @sa Config
 * @sa Field
 * @sa Message
 */
class GMSEC_API MessageFactory
{
public:
	/**
	 * @fn MessageFactory()
	 *
	 * @brief Basic constructor that initializes the %MessageFactory to produce
	 * messages based on the latest available message specification.
	 */
	MessageFactory();


	/**
	 * @fn MessageFactory(const Config& config)
	 *
	 * @brief Constructor that initializes the %MessageFactory to produce
	 * messages based on information contained within the configuration object
	 * (e.g. gmsec-specification-version=201900).
	 *
	 * @param config - The %Config object.
	 *
	 * @sa Config
	 */
	MessageFactory(const Config& config);


	/**
	 * @fn ~MessageFactory()
	 *
	 * @brief %MessageFactory destructor.
	 */
	virtual ~MessageFactory();


	/**
	 * @fn void setStandardFields(const util::List<Field*>& standardFields)
	 *
	 * @brief Allows for the setting of common (standard) fields that should be
	 * applied to all messages created by the %MessageFactory.
	 *
	 * @param standardFields - The list of common/standard fields to apply to all messages.
	 *
	 * @sa Message::addFields()
	 * @sa MessageFactory::clearStandardFields()
	 */
	void CALL_TYPE setStandardFields(const util::List<Field*>& standardFields);


	/**
	 * @fn void clearStandardFields()
	 *
	 * @brief Removes any common/standard fields that have previously been associated
	 * with the %MessageFactory.
	 *
	 * @sa MessageFactory::setStandardFields()
	 */
	void CALL_TYPE clearStandardFields();


	/**
	 * @fn void setMessageConfig(const Config& msgConfig)
	 *
	 * @brief Allows for the setting of a common message configuration that will be
	 * applied to all messages created by the %MessageFactory.
	 *
	 * @param msgConfig - The message %Config object.
	 *
	 * @sa Message::setConfig()
	 */
	void CALL_TYPE setMessageConfig(const Config& msgConfig);


	/**
	 * @fn Message createMessage() const
	 *
	 * @brief Creates and returns a %Message object. The %Message will contain any user-supplied
	 * message attributes (such as a message configuration, standard fields, and 
	 * custom message validator).
	 *
	 * @return A Message object is returned.
	 *
	 * @sa MessageFactory::setMessageConfig()
	 * @sa MessageFactory::setStandardFields()
	 * @sa MessageFactory::registerMessageValidator()
	 */
	Message CALL_TYPE createMessage() const;


	/**
	 * @fn Message createMessage(const char* schemaID) const
	 *
	 * @brief Creates and returns a %Message object that is populated with fields derived
	 * from the working message specification and schema ID. In addition, the %Message object
	 * will include any user-supplied message attributes (such as a message configuration, standard
	 * fields, and custom message validator).
	 *
	 * @param schemaID - The string identifying which message to create (e.g. "HB").
	 *
	 * @return A Message object is returned.
	 *
	 * @throw A GmsecException is thrown if the schemaID string is NULL, contains an empty
	 * string, or references an unknown/illegal schema ID.
	 *
	 * @sa MessageFactory::setMessageConfig()
	 * @sa MessageFactory::setStandardFields()
	 * @sa MessageFactory::registerMessageValidator()
	 */
	Message CALL_TYPE createMessage(const char* schemaID) const;


	/**
	 * @fn Message fromData(const char* data, DataType type)
	 *
	 * @brief Creates and returns a Message object based on the given XML or JSON data.
	 *
	 * @param data - An XML or JSON data string that represents a GMSEC-style message.
	 * @param type - The type of data being provided
	 *
	 * @returns A Message object that is populated using the given data is returned.
	 *
	 * @throw A GmsecException is thrown if the given data is NULL, or does not represent
	 * a GMSEC-style message in either XML or JSON format.
	 */
	Message CALL_TYPE fromData(const char* data, DataType type);


	/**
	 * @fn const Specification& getSpecification() const
	 *
	 * @brief Returns a reference to the Specification object associated with the %MessageFactory.
	 */
	const Specification& CALL_TYPE getSpecification() const;


	/**
	 * @fn void registerMessageValidator(MessageValidator* validator)
	 *
	 * @brief Registers the given message validator with each message
	 * created using the Message Factory.
	 *
	 * @param validator - A custom message validation object.
	 *
	 * @throw A GmsecException is thrown if the validator object is NULL.
	 *
	 * @sa MessageValidator
	 * @sa Message::registerMessageValidator()
	 */
	void CALL_TYPE registerMessageValidator(MessageValidator* validator);


	/* @cond For C API support ONLY! */
	void CALL_TYPE registerMessageValidator(GMSEC_MessageValidator* validator);
	/* @endcond */

private:
	// Defined, but not implemented
	MessageFactory(const MessageFactory& other);
	MessageFactory& operator=(const MessageFactory& other);

	friend class gmsec::api5::internal::MessageFactoryBuddy;

	gmsec::api5::internal::InternalMessageFactory* m_internal;
};

}  // end namespace api5
}  // end namespace gmsec


#endif
