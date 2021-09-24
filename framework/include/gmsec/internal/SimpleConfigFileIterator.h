/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file SimpleConfigFileIterator.h
 *
 *  @This file contains a definition for an iterator for the ConfigFile 
 *
 **/

#ifndef gmsec_SimpleConfigFileIterator_h
#define gmsec_SimpleConfigFileIterator_h

#include <gmsec/Status.h>
#include <gmsec/ConfigFileIterator.h>
#include <gmsec/util/List.h>
#include <gmsec/internal/ci_less.h>
#include <tinyxml2.h>

namespace gmsec
{
namespace internal
{


class GMSEC_API SimpleConfigFileIterator
{
private:
	typedef std::map<std::string, tinyxml2::XMLElement *, ci_less> NodeMap;
	typedef NodeMap::iterator NodeItr;
	
	//structure to hold information for iterators
	struct iterInfo{
		NodeItr cursorMain;
		NodeItr cursorName;
		NodeItr end;
	};

	bool initialized;		//whether ConfigFileIterator Initialized by ConfigFile
	iterInfo *cfgIter;		//iterator info for Configs
	iterInfo *msgIter;		//iterator info for Messages
	iterInfo *subIter;		//iterator info for Subscriptions
	iterInfo *customXmlIter;	//iterator info for Custom XML Elements

	/** @fn HasNextAux(iterInfo *iter, bool name)
	 * @brief Helper function to determine whether hasNext
	 * @param iter - iterInfo struct of element type
	 * @param name - whether it is the name field to be checked
	 * @return bool - whether there is a next
	 */
	bool HasNextAux(iterInfo *iter, bool name);
	//Helper function to retrieve a attribute from an XML element
	const char* getStrAttr(tinyxml2::XMLElement *element, const char *field);
	//Cleans up memory to be reinitialized
	void uninitialize();
	
	/** @fn Error_UNINITIALIZED()
	 * @brief Reports an error that ConfigFileIterator hasn't been initialized
	 */
	Status Error_UNINITIALIZED();
	
	/** @fn Error_END()
	 * @brief Reports an error that Next has been called with a iterator on 'end'
	 */
	Status Error_END();

	// Declared, but not implemented.
	SimpleConfigFileIterator(const SimpleConfigFileIterator&);
	SimpleConfigFileIterator& operator=(const SimpleConfigFileIterator&);


public:
	/** @fn SimpleConfigFileIterator()
	 * @brief Constructs a SimpleConfigFileIterator. Object is not internally 
	 * 'initialized' until passed into ConfigFile.Reset(ConfigFileIterator) 
	 */
	SimpleConfigFileIterator();

	/** @fn ~SimpleConfigFileIterator()
	 * @brief Deconstructor for SimpleConfigFileIterator
	 */
	~SimpleConfigFileIterator();
	
	/** @fn Initialize(gmsec::util::List<nodeItr> nodeIters)
	 * @brief Initializes the ConfigFileIterator. Only to be called from ConfigFile.
	 * @param nodeIters - List of begin/end iterators
	 * @return status - whether initializing the CFIterator was successful.
	 */
	Status Initialize(gmsec::util::List<NodeItr> &nodeIters);

	/** @fn HasNextSubscriptionName()
	 * @brief Determines if there is a next subscriptionName in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextSubscriptionName();

	/** @fn HasNextConfigName()
	 * @brief Determines if there is a next configName in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextConfigName();

	/** @fn HasNextMessageName()
	 * @brief Determines if there is a next messageName in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextMessageName();

	/** @fn HasNextSubscription()
	 * @brief Determines if there is a next subscription in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextSubscription();

	/** @fn HasNextConfig()
	 * @brief Determines if there is a next Config in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextConfig();

	/** @fn HasNextMessage()
	 * @brief Determines if there is a next Message in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextMessage();	

	/** @fn HasNextCustomElement()
	 * @brief Determines if there is another custom XML element in the iterator
	 * @return bool - whether there is a next
	 */
	bool HasNextCustomElement();	
	
	/** @fn NextSubscriptionName(gmsec::util::String &gmstr)
	 * @brief Sets the next subscriptionName to the passed string object
	 * @param gmstr - gmsec String to recieve next subscriptionName
	 * @return status - whether next action was successful
	 */
	Status NextSubscriptionName(gmsec::util::String &gmstr);

	/** @fn NextConfigName(gmsec::util::String &gmstr)
	 * @brief Sets the next configName to the passed string object
	 * @param gmstr - gmsec String to recieve next configName
	 * @return status - whether next action was successful
	 */
	Status NextConfigName(gmsec::util::String &gmstr);

	/** @fn NextMessageName(gmsec::util::String &gmstr)
	 * @brief Sets the next messageName to the passed string object
	 * @param gmstr - gmsec String to recieve next messageName
	 * @return status - whether next action was successful
	 */
	Status NextMessageName(gmsec::util::String &gmstr);

	/** @fn NextSubscription(gmsec::util::String &gmstr)
	 * @brief Sets the next subscription to the passed string object
	 * @param gmstr - gmsec String to recieve next subscription
	 * @return status - whether next action was successful
	 */
	Status NextSubscription(gmsec::util::String &gmstr);

	/** @fn NextConfig(Config *config)
	 * @brief Sets the next config to the passed config object
	 * @param config - config object to recieve next config
	 * @return status - whether next action was successful
	 */
	Status NextConfig(Config *config);

	/** @fn NextMessage(Message *message)
	 * @brief Sets the next message to the passed message object
	 * @param message - message object to recieve next message
	 * @return status - whether next action was successful
	 */
	Status NextMessage(Message *message);

	/** @fn NextCustomElement(gmsec::util::String &gmstr)
	 * @brief Sets the next Custom XML Element to the passed string object
	 * @param gmstr - gmsec String to recieve next messageName
	 * @return status - whether next action was successful
	 */
	Status NextCustomElement(gmsec::util::String &gmstr);

}; //class SimpleConfigFileIterator
}  //namespace internal
}  //namespace gmsec

#endif /* gmsec_SimpleConfigFileIterator_h */

