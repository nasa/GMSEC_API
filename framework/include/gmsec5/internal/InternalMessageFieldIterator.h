/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_INTERNAL_MESSAGE_FIELD_ITERATOR_H
#define GMSEC_API5_INTERNAL_MESSAGE_FIELD_ITERATOR_H

#include <gmsec5/MessageFieldIterator.h>

#include <gmsec5/internal/InternalMessage.h>

#include <gmsec5/util/wdllexp.h>

#include <map>
#include <string>


namespace gmsec
{
namespace api5
{
	class Field;

namespace internal
{


class GMSEC_API InternalMessageFieldIterator
{
public:
    InternalMessageFieldIterator(InternalMessage& msg, MessageFieldIterator::Selector selector = MessageFieldIterator::Selector::ALL_FIELDS);

	~InternalMessageFieldIterator();

    bool CALL_TYPE hasNext();

    const Field& CALL_TYPE next();

	void CALL_TYPE reset();

    void CALL_TYPE setSelector(MessageFieldIterator::Selector selector);

private:
	// Declared, but not implemented
	InternalMessageFieldIterator(const InternalMessageFieldIterator&);
	InternalMessageFieldIterator& operator=(const InternalMessageFieldIterator&);

	typedef bool (*FieldSelector)(const Field& field);

	bool makeReady();

	static bool selectAllFields(const Field& field);
    static bool CALL_TYPE selectHeaderFields(const Field& field);
    static bool CALL_TYPE selectNonHeaderFields(const Field& field);
    static bool CALL_TYPE selectTrackingFields(const Field& field);

	InternalMessage& m_msg;
	FieldSelector    m_selector;
	bool             m_ready;
	const Field*     m_nextField;
};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
