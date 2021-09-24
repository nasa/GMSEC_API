/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicMessage.h
 *
 * @brief This file contains the public interface to Mnemonic messages.
 */


#ifndef GMSEC_API_MESSAGE_MNEMONIC_MESSAGE_H
#define GMSEC_API_MESSAGE_MNEMONIC_MESSAGE_H

#include <gmsec4/mist/message/MistMessage.h>

#include <gmsec4/util/Deprecated.h>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Config;

namespace mist
{
	// Forward declaration(s)
	class Mnemonic;
	class MnemonicIterator;
	class Specification;

namespace message
{
	// Forward declaration(s)
	namespace internal
	{
		class InternalMnemonicMessage;
	}


/**
 * @class MnemonicMessage
 *
 * @brief A MistMessage object that is capable of storing Mnemonic objects
 * The following message schema IDs and their templates are supported: @n
 * MSG.MVAL @n
 * REQ.MVAL @n
 * RESP.MVAL @n
 *
 * @sa Message @n
 *	   MistMessage @n
 *     Config @n
 *	   Specification @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API MnemonicMessage : public MistMessage
{
public:
	/**
	 * @fn MnemonicMessage(const char* subject, const char* schemaID, const Specification& spec)
	 *
	 * @brief constructor - Initializes the message instance with a given schema ID
	 *
	 * @param subject - The subject string for the message.
	 * @param schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType (e.g. 2019.00.C2MS.MSG.MVAL). You may also
	 * use the shorthand notation of messageKind.messageType (e.g. MSG.MVAL).
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * 
	 * @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	 */
	MnemonicMessage(const char* subject, 
					const char* schemaID,
					const Specification& spec);


	/**
	 * @fn MnemonicMessage(const char* subject, const char* schemaID, const Config& config, const Specification& spec)
	 *
	 * @brief constructor - Initializes the message instance with a given schema ID and associates a Configuration object
	 *
	 * @param subject - The subject string for the message.
	 * @param schemaID - the string used to identify the message schema in C2MS or other message specification. The schema ID has the
	 * format of: major.minor.schemaLevelName.messageKind.messageType (e.g. 2019.00.C2MS.MSG.MVAL). You may also
	 * use the shorthand notation of messageKind.messageType (e.g. MSG.MVAL).
	 * @param config - A configuration to associate with the message.
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * 
	 * @throw An exception is thrown if Specification fails to load the template directory or if schemaID is not a valid ID.
	 */
	MnemonicMessage(const char* subject, 
					const char* schemaID,
					const gmsec::api::Config& config,
					const Specification& spec);


	/**
	 * @fn MnemonicMessage(const MnemonicMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given MnemonicMessage
	 *
	 * @param other - the other MnemonicMessage object to copy.
	 */
	MnemonicMessage(const MnemonicMessage& other);


	/**
	 * @fn MnemonicMessage(const char* data)
	 *
	 * @brief Initializes a MnemonicMessage from an XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize MnemonicMessage.
	 */
	GMSEC_DEPRECATED MnemonicMessage(const char* data);


	/**
	 * @fn MnemonicMessage(const Specification& spec, const char* data)
	 *
	 * @brief Initializes a Mnemonic Message using the given XML or JSON data representation of the message,
	 * and ensures the message adheres to the provided specification.
	 *
	 * @param spec - A reference to the specification this message's schema will adhere to.
	 * @param data - XML or JSON string used to initialize the message
	 *
	 * @throw An Exception is thrown if the given data does not represent a valid XML or JSON statement.
	 * @throw An Exception is thrown if the given data does not represent a Mnemonic Message.
	 */
	MnemonicMessage(const Specification& spec, const char* data);


	/**
	 * @fn ~MnemonicMessage()
	 *
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~MnemonicMessage();


	/**
	 * @fn MnemonicMessage& operator=(const MnemonicMessage& other)
	 *
	 * @brief Overloaded assignment operator method for making a copy of other MnemonicMessage object
	 *
	 * @param other - the other MnemonicMessage object to copy.
	 *
	 * @return A copy of the other MnemonicMessage object that was given.
	 */
	MnemonicMessage& CALL_TYPE operator=(const MnemonicMessage& other);


	/**
	 * @fn void addMnemonic(const Mnemonic& mnemonic)
	 *
	 * @brief Add a mnemonic to the message.
	 */
	void CALL_TYPE addMnemonic(const Mnemonic& mnemonic);


	/**
	 * @fn const Mnemonic& getMnemonic(size_t index) const
	 *
	 * @brief Get mnemonic numbered at index. Note that the index which will be
	 * retrieved does not correspond to the GMSEC ISD and starts from 0 instead of 1.
	 * For example, getMnemonic(0) would return the Mnemonic corresponding to
	 * MNEMONIC.1.
	 *
	 * @return Mnemonic at specified index inside of MnemonicMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the range of Mnemonics in this message.
	 *
	 * @note This function has been deprecated, use MnemonicIterator instead.
	 */
	GMSEC_DEPRECATED const Mnemonic& CALL_TYPE getMnemonic(size_t index) const;


	/**
	 * @fn size_t getNumMnemonics() const
	 *
	 * @brief Get the number of mnemonics in this message.
	 *
	 * @return Return the number of mnemonics in the message.
	 */
	size_t CALL_TYPE getNumMnemonics() const;


	/**
	 * @fn MnemonicIterator& getMnemonicIterator() const
	 *
	 * @desc Method that allows the callee to get the MnemonicIterator associated with the %MnemonicMessage.
	 * This iterator will allow for applications to iterate over the Mnemonic objects stored within the %MnemonicMessage.
	 * The iterator is reset each time getMnemonicIterator() is called.  The iterator itself is destroyed when the
	 * %MnemonicMessage object is destroyed.
	 *
	 * @note Only one MnemonicIterator object is associated with a %MnemonicMessage object; multiple calls to
	 * getMnemonicIterator() will return a reference to the same MnemonicIterator object.  Each call will reset the iterator.
	 *
	 * @return A reference to a MnemonicIterator object.
	 */
	MnemonicIterator& CALL_TYPE getMnemonicIterator() const;


	/**
	 * @fn MnemonicMessage convertMessage(const Message& message)
	 *
	 * @brief Constructs and returns a MnemonicMessage from an an ordinary message.
	 */
	static MnemonicMessage CALL_TYPE convertMessage(const Message& message);
};

} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
