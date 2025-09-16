/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageFieldIterator.h
 *
 * @brief Defines a class that supports the iteration over the fields of a Message.
 */

#ifndef GMSEC_API5_MESSAGE_FIELD_ITERATOR_H
#define GMSEC_API5_MESSAGE_FIELD_ITERATOR_H

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Field;
	class Message;

namespace internal
{
	class InternalMessage;
	class InternalMessageFieldIterator;
}


/** @class MessageFieldIterator
 * 
 * @brief The MessageFieldIterator supports iterating over the fields of a Message.
 *
 * @note MessageFieldIterator is not thread safe.
 *
 * @sa Message::getFieldIterator()
*/
class GMSEC_API MessageFieldIterator
{
public:
	/**
	 * @enum Selector
	 *
	 * @brief The type of MessageFieldIterator to set up.
	 */
	enum class Selector
	{
		ALL_FIELDS,        ///< Used for interating over all available fields.
		HEADER_FIELDS,     ///< Used for interating only over header fields.
		NON_HEADER_FIELDS, ///< Used for interating only over non-header fields.
		TRACKING_FIELDS    ///< Used for interating only over tracking fields.
	};


	/*
	 * @fn ~MessageFieldIterator()
	 *
	 * @brief Destructor.
	 */
    ~MessageFieldIterator();


	/**
	 * @fn bool hasNext() const
	 *
	 * @brief Provides information as to whether there are additional fields that can be referenced using next().
	 *
	 * @return True if additional fields are available, false otherwise.
	 */
    bool CALL_TYPE hasNext() const;


	/**
	 * @fn const Field& next() const
	 *
	 * @brief Returns a reference to the next available Field object.
	 *
	 * @return A reference to a Field object.
	 *
	 * @throw GmsecException is thrown if the iterator has reached the end (i.e. there are no more fields).
	 */
    const Field& CALL_TYPE next() const;


	/**
	 * @fn void reset()
	 *
	 * @brief Resets the interator to the beginning of the Field list that is maintained by the Message.
	 */
	void CALL_TYPE reset();


private:
	friend class gmsec::api5::internal::InternalMessage;

	/**
	 * @fn MessageFieldIterator(const Message& msg, Selector selector = ALL_FIELDS)
	 *
	 * @brief Default constructor.
	 *
	 * @param msg - the Message object to use when iterating over the Fields.
	 * @param selector - the type of iterator to set up (default is ALL_FIELDS)
	 */
	MessageFieldIterator(internal::InternalMessage& msg, Selector selector = Selector::ALL_FIELDS);


	// Declared, but not implemented
	MessageFieldIterator(const MessageFieldIterator&);


    gmsec::api5::internal::InternalMessageFieldIterator* m_iter;
};

} // namespace api5
} // namespace gmsec

#endif
