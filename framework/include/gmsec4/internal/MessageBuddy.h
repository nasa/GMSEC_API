#ifndef GMSEC_API_INTERNAL_MESSAGE_BUDDY_H
#define GMSEC_API_INTERNAL_MESSAGE_BUDDY_H

#include <gmsec4/Message.h>
#include <gmsec4/internal/InternalMessage.h>


namespace gmsec
{
namespace api
{
namespace internal
{


class MessageBuddy
{
public:
	static InternalMessage& getInternal(const Message& msg)
	{
		return *msg.m_iMessage;
	}
};


}  // namespace internal
}  // namespace api
}  // namespace gmsec

#endif
