/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_INTERNAL_MESSAGE_BUDDY_H
#define GMSEC_API5_INTERNAL_MESSAGE_BUDDY_H

#include <gmsec5/Message.h>
#include <gmsec5/internal/InternalMessage.h>


namespace gmsec
{
namespace api5
{
namespace internal
{


class MessageBuddy
{
public:
	static InternalMessage& getInternal(const Message& msg)
	{
		return *msg.m_internal;
	}
};


}  // namespace internal
}  // namespace api5
}  // namespace gmsec

#endif
