/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec/internal/MessageBuddy.h>


#include <gmsec/Message.h>
#include <gmsec/internal/InternalMessage.h>

#include <gmsec4/Message.h>


using namespace gmsec;
using namespace gmsec::internal;


InternalMessage* MessageBuddy::getInternal(Message* msg)
{
	if (msg != NULL)
	{
		return msg->ptr;
	}

	return NULL;
}


Message* MessageBuddy::createMessage(gmsec::api::Message* msg4)
{
	return new Message(new InternalMessage(msg4));
}


void MessageBuddy::destroyMessage(Message* msg)
{
	if (msg != NULL)
	{
		delete msg;
	}
}
