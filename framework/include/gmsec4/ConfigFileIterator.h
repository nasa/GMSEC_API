/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConfigFileIterator.h
 *
 *  @This file contains a definition for an iterator for the ConfigFile.
 */

#ifndef GMSEC_API_CONFIG_FILE_ITERATOR_H
#define GMSEC_API_CONFIG_FILE_ITERATOR_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/ConfigFile.h>

#include <string>
#include <cstring>


namespace gmsec
{
namespace api
{
namespace internal
{
	// Forward declaration(s)
	class InternalConfigFile;
	class InternalConfigFileIterator;
}


/**
 * @class ConfigFileIterator
 *
 * @brief This class is used to access elements of a configuration file (ConfigFile) object.
 */
class GMSEC_API ConfigFileIterator
{
public:
	/**
	 * @cond
	 *
	 * @fn ~ConfigFileIterator()
	 *
	 * @brief Deconstructor for ConfigFileIterator
	 */
	~ConfigFileIterator();
	/** @endcond */


	/**
	 * @fn bool hasNextConfig() const
	 *
	 * @brief Determines if there is a next Config in the iterator
	 *
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE hasNextConfig() const;


	/**
	 * @fn bool hasNextMessage() const
	 *
	 * @brief Determines if there is a next Message in the iterator
	 *
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE hasNextMessage() const;


	/**
	 * @fn bool hasNextSubscription() const
	 *
	 * @brief Determines if there is a next subscription in the iterator
	 *
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE hasNextSubscription() const;


	/**
	 * @fn bool hasNextCustomElement() const
	 *
	 * @brief Determines if there is another custom XML element in the iterator
	 *
	 * @return bool - whether there is a next
	 */
	bool CALL_TYPE hasNextCustomElement() const;

	
	/**
	 * @fn const ConfigEntry& nextConfig() const
	 *
	 * @brief Returns the next ConfigEntry.
	 *
	 * @return The next ConfigEntry.
	 *
	 * @throw Exception is thrown if no more config entries are available.
	 *
	 * @sa hasNextConfig()
	 */
	const ConfigFile::ConfigEntry& CALL_TYPE nextConfig() const;


	/**
	 * @fn const MessageEntry& nextMessage() const
	 *
	 * @brief Returns the next MessageEntry.
	 *
	 * @return The next MessageEntry.
	 *
	 * @throw Exception is thrown if no message entries are available.
	 *
	 * @sa hasNextMessage()
	 */
	const ConfigFile::MessageEntry& CALL_TYPE nextMessage() const;


	/**
	 * @fn const SubscriptionEntry& nextSubscription() const
	 *
	 * @brief Returns the next SubscriptionEntry.
	 *
	 * @note The returned structure contains a string that is volatile; the callee should consider
	 * making a copy of the returned string.
	 *
	 * @return The next SubscriptionEntry.
	 *
	 * @throw Exception is thrown if no more subscription entries are available.
	 *
	 * @sa hasNextSubscription()
	 */
	const ConfigFile::SubscriptionEntry& CALL_TYPE nextSubscription() const;


	/**
	 * @fn const char* nextCustomElement() const
	 *
	 * @brief Returns the next custom XML element.
	 *
	 * @note The returned string is volatile; the callee should consider
	 * making a copy of the returned string.  For example:
	 *
	 * @code
	 * try {
	 *     ConfigFile cfgFile;
	 *     cfgFile.load("someFile.xml");
	 *
	 *     ConfigFileIterator iter;
	 *     iter = cfgFile.getIterator();
	 *
	 *     std::string customElement;
	 *
	 *     if (iter.hasNextCustomElement()) {
	 *         customElement = iter.nextCustomElement();  // creates copy of custom element
	 *     }
	 *
	 *     if (!customElement.empty()) {
	 *         std::cout << customElement << std::endl;
	 *     }
	 * }
	 * catch (Exception& e) {
	 *     //handle error loading ConfigFile
	 * }
	 * @endcode
	 *
	 * @return An XML string.
	 *
	 * @throw Exception is thrown if no custom elements are available.
	 *
	 * @sa hasNextCustomElement()
	 */
	const char* CALL_TYPE nextCustomElement() const;


	/**
	 * @fn void reset()
	 *
	 * @desc Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFile.
	 */
	void CALL_TYPE reset();


private:
	friend class gmsec::api::internal::InternalConfigFile;


	/**
	 * @fn ConfigFileIterator()
	 *
	 * @brief Constructs a ConfigFileIterator. Object is not internally 
	 *
	 * @param cfgFile - reference to the internal ConfigFile object.
	 */
	ConfigFileIterator(internal::InternalConfigFile& cfgFile);


	// Declared, but not implemented.
	ConfigFileIterator(const ConfigFileIterator&);
	ConfigFileIterator& operator=(const ConfigFileIterator&);

	internal::InternalConfigFileIterator* m_iter;
};

}  //namespace api
}  //namespace gmsec

#endif
