/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef MESSAGE_BUDDY_H
#define MESSAGE_BUDDY_H

#include <gmsec/util/wdllexp.h>


namespace gmsec
{
class Message;

namespace api
{
class Message;
}

namespace internal
{
class InternalMessage;


class GMSEC_API MessageBuddy
{
public:
	static InternalMessage* CALL_TYPE getInternal(Message* msg);

	static Message* CALL_TYPE createMessage(gmsec::api::Message* msg4);

	static void CALL_TYPE destroyMessage(Message* msg);
};


}  // end namespace internal
}  // end namespace gmsec


#endif
