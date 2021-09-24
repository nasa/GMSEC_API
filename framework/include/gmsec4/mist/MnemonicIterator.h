/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MnemonicIterator.h
 *
 * @brief Defines a class that supports the iteration over the Mnemonic objects stored in a MnemonicMessage.
 */

#ifndef GMSEC_API_MNEMONIC_ITERATOR_H
#define GMSEC_API_MNEMONIC_ITERATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	class Mnemonic;
	namespace internal
	{
		class InternalMnemonicIterator;
	}

namespace message
{
	namespace internal
	{
		class InternalMnemonicMessage;
	}
}


/** @class MnemonicIterator
 * 
 * @brief The MnemonicIterator supports iterating over the Mnemonic objects of a MnemonicMessage.
 *
 * @note MnemonicIterator is not thread safe.
 *
 * @sa MnemonicMessage::getMnemonicIterator()
*/
class GMSEC_API MnemonicIterator
{
public:
	/*
	 * @fn ~MnemonicIterator()
	 *
	 * @brief Destructor.
	 */
    ~MnemonicIterator();


	/**
	 * @fn bool hasNext() const
	 *
	 * @brief Provides information as to whether there are additional Mnemonic objects that can be referenced using next().
	 *
	 * @return True if additional fields are available, false otherwise.
	 */
    bool CALL_TYPE hasNext() const;


	/**
	 * @fn const Mnemonic& next() const
	 *
	 * @brief Returns a reference to the next available Mnemonic object.
	 *
	 * @return A reference to a Mnemonic object.
	 *
	 * @throw Exception is thrown if the iterator has reached the end (i.e. there are no more Mnemonics).
	 */
    const Mnemonic& CALL_TYPE next() const;


	/**
	 * @fn void reset()
	 *
	 * @desc Resets the interator to the beginning of the Mnemonic list that is maintained by the MnemonicMessage.
	 */
	void CALL_TYPE reset();


private:
	friend class gmsec::api::mist::message::internal::InternalMnemonicMessage;

	/**
	 * @fn MnemonicIterator(const MnemonicMessage& msg)
	 *
	 * @brief Default constructor.
	 *
	 * @param msg - the MnemonicMessage object to use when iterating over the Mnemonic objects.
	 */
	MnemonicIterator(mist::message::internal::InternalMnemonicMessage& msg);


	// Declared, but not implemented
	MnemonicIterator(const MnemonicIterator&);

	//TODO - Figure out why Visual Studio thinks this method must be implicitly defined.
	//       Might be related to the (false) C4355 warning.
	//MnemonicIterator& operator=(const MnemonicIterator&);


	gmsec::api::mist::internal::InternalMnemonicIterator* m_iter;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
