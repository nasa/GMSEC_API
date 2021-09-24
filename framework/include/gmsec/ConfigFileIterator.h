/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConfigFileIterator.h
 *
 *  @This file contains a definition for an iterator for the ConfigFile 
 *
 **/

#ifndef gmsec_ConfigFileIterator_h
#define gmsec_ConfigFileIterator_h

#include <gmsec/Status.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/String.h>

namespace gmsec
{
class Config;
class Message;
namespace internal
{
class SimpleConfigFileIterator;
class BaseConfigFile;
}

/** @class ConfigFileIterator
 * @brief This class is used to access elements of a configuration file (ConfigFile) object.
 */
class GMSEC_API ConfigFileIterator
{
private:
	friend class internal::BaseConfigFile;
	internal::SimpleConfigFileIterator *ptr;

	// Declared, but not implemented.
	ConfigFileIterator(const ConfigFileIterator&);
	ConfigFileIterator& operator=(const ConfigFileIterator&);

public:
	/** @fn ConfigFileIterator()
	 * @brief Constructs a ConfigFileIterator. Object is not internally 
	 * 'initialized' until passed into ConfigFile.Reset(ConfigFileIterator) 
	 */
	GMSEC_DEPRECATED ConfigFileIterator();

	/** @fn ~ConfigFileIterator()
	 * @brief Deconstructor for ConfigFileIterator
	 */
	~ConfigFileIterator();

	/** @fn HasNextSubscriptionName()
	 * @brief Determines if there is a next subscriptionName in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextSubscriptionName();

	/** @fn HasNextConfigName()
	 * @brief Determines if there is a next configName in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextConfigName();

	/** @fn HasNextMessageName()
	 * @brief Determines if there is a next messageName in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextMessageName();

	/** @fn HasNextSubscription()
	 * @brief Determines if there is a next subscription in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextSubscription();

	/** @fn HasNextConfig()
	 * @brief Determines if there is a next Config in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextConfig();

	/** @fn HasNextMessage()
	 * @brief Determines if there is a next Message in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextMessage();	

	/** @fn HasNextCustomElement()
	 * @brief Determines if there is another custom XML element in the iterator
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE HasNextCustomElement();	
	
	/** @fn NextSubscriptionName(gmsec::util::String &gmstr)
	 * @brief Sets the next subscriptionName to the passed string object
	 * @param gmstr - gmsec String to recieve next subscriptionName
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextSubscriptionName(gmsec::util::String &gmstr);

	/** @fn NextConfigName(gmsec::util::String &gmstr)
	 * @brief Sets the next configName to the passed string object
	 * @param gmstr - gmsec String to recieve next configName
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextConfigName(gmsec::util::String &gmstr);

	/** @fn NextMessageName(gmsec::util::String &gmstr)
	 * @brief Sets the next messageName to the passed string object
	 * @param gmstr - gmsec String to recieve next messageName
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextMessageName(gmsec::util::String &gmstr);

	/** @fn NextSubscription(gmsec::util::String &gmstr)
	 * @brief Sets the next subscription to the passed string object
	 * @param gmstr - gmsec String to recieve next subscription
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextSubscription(gmsec::util::String &gmstr);

	/** @fn NextConfig(Config *config)
	 * @brief Sets the next config to the passed config object
	 * @param config - config object to recieve next config
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextConfig(Config *config);

	/** @fn NextMessage(Message *message)
	 * @brief Sets the next message to the passed message object
	 * @param message - message object to recieve next message
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextMessage(Message *message);

	/** @fn NextCustomElement(gmsec::util::String &gmstr)
	 * @brief Sets the next Custom XML Element to the passed string object
	 * @param gmstr - gmsec String to recieve next messageName
	 * @return status - whether next action was successful
	 */
	Status CALL_TYPE NextCustomElement(gmsec::util::String &gmstr);

}; //class ConfigFileIterator
}  //namespace gmsec

#endif /* gmsec_ConfigFileIterator_h */




