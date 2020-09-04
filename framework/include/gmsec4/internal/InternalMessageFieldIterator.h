#ifndef GMSEC_API_INTERNAL_MESSAGE_FIELD_ITERATOR_H
#define GMSEC_API_INTERNAL_MESSAGE_FIELD_ITERATOR_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/MessageFieldIterator.h>
#include <gmsec4/internal/InternalMessage.h>

#include <map>
#include <string>


namespace gmsec
{
namespace api
{
class Field;

namespace internal
{


class GMSEC_API InternalMessageFieldIterator
{
public:
    InternalMessageFieldIterator(InternalMessage& msg, MessageFieldIterator::Selector selector = MessageFieldIterator::ALL_FIELDS);

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

	bool CALL_TYPE makeReady();

	static bool CALL_TYPE selectAllFields(const Field& field);
	static bool CALL_TYPE selectHeaderFields(const Field& field);
	static bool CALL_TYPE selectNonHeaderFields(const Field& field);

	InternalMessage& m_msg;
	FieldSelector    m_selector;
	bool             m_ready;
	const Field*     m_nextField;
};

} // namespace internal
} // namespace api
} // namespace gmsec

#endif
