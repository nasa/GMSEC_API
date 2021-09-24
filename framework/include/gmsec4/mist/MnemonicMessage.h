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


#ifndef GMSEC_API_MNEMONIC_MESSAGE_H
#define GMSEC_API_MNEMONIC_MESSAGE_H

#include <gmsec4/Message.h>

#include <gmsec4/util/Deprecated.h>


namespace gmsec
{

namespace api
{
	class Config;

namespace mist
{
	class Mnemonic;
	class MnemonicIterator;

namespace message
{
	namespace internal
	{
		class InternalMnemonicMessage;
	}
}

/**
 * @class MnemonicMessage
 *
 * @brief The Message object contains a GMSEC MSG MVAL message.
 * The following message schema IDs and their templates are supported: @n
 * 2014.00.GMSEC.MSG.MVAL
 * 2016.00.GMSEC.MSG.MVAL
 *
 * @note gmsec::api::mist::MnemonicMessage has been deprecated.  It is recommended to use gmsec::api::mist::message::MnemonicMessage
 *
 * @sa Message @n
 *     Config @n
 *     Connection @n
 *     Field @n
 *     MsgFieldIterator
 */
class GMSEC_API MnemonicMessage : public Message
{
public:
	/**
	 * @fn MnemonicMessage(const char* subject, unsigned int version)
	 *
	 * @brief Default constructor - Initializes the message instance
	 *
	 * @param subject - The subject string for the message.
	 * @param version - The version of the GMSEC message specification to be used.
	 */
	GMSEC_DEPRECATED MnemonicMessage(const char* subject, unsigned int version);


	/**
	 * @fn MnemonicMessage(const char* subject, const Config& config, unsigned int version)
	 *
	 * @brief constructor - Initializes the message instance and associates a Configuration object
	 *
	 * @param subject - The subject string for the message.
	 * @param config  - A configuration to associate with the message.
	 * @param version - The version of the GMSEC message specification to be used.
	 */
	GMSEC_DEPRECATED MnemonicMessage(const char* subject, const Config& config, unsigned int version);


	/**
	 * @fn MnemonicMessage(const MnemonicMessage& other)
	 *
	 * @brief Copy constructor - initializes the message instance using the other given MnemonicMessage
	 *
	 * @param other - the other MnemonicMessage object to copy.
	 */
	GMSEC_DEPRECATED MnemonicMessage(const MnemonicMessage& other);


	/**
	 * @fn MnemonicMessage(const char* data)
	 *
	 * @brief Initializes a MnemonicMessage from an XML or JSON string.
	 *
	 * @param data - XML or JSON string used to initialize MnemonicMessage.
	 */
	GMSEC_DEPRECATED MnemonicMessage(const char* data);


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
	 * @brief Get mnemonic numbered at index.
	 *
	 * @return Mnemonic at specified index inside of MnemonicMessage.
	 *
	 * @throw Throws an exception if the index specified is not in the range of Mnemonics in this message.
	 */
	const Mnemonic& CALL_TYPE getMnemonic(size_t index) const;


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
	gmsec::api::mist::MnemonicIterator& CALL_TYPE getMnemonicIterator() const;


	/**
	 * @fn MnemonicMessage convertMessage(const Message& message)
	 *
	 * @brief Constructs and returns a MnemonicMessage from an an ordinary message.
	 */
	GMSEC_DEPRECATED static MnemonicMessage CALL_TYPE convertMessage(const Message& message);


private:
	// Defined so as to preserve binary compatibility with API 4.1
	void* filler;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
