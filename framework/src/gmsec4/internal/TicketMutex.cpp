/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file TicketMutex.cpp
 *
 *  This file contains the ticket (FIFO) Mutex definition.
 */

#include <gmsec4/internal/TicketMutex.h>

#include <gmsec4/util/StdThread.h>
#include <gmsec4/util/Log.h>


using namespace gmsec::api::util;

static const int DEBUG = 0;


TicketMutex::TicketMutex()
	: head(0),
	  tail(0)
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "+ TicketMutex:" << this;
	}
}


TicketMutex::~TicketMutex()
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "~TicketMutex:" << this << ": thread " << StdThread::getCurrentID();
	}
}


void TicketMutex::enter()
{
	if (DEBUG)
	{
		GMSEC_VERBOSE << "TicketMutex:" << this << ": thread " << StdThread::getCurrentID() << ": entering";
	}

	AutoMutex hold(condition.getMutex());
	GMSEC_U32 id = tail++;

	while (id != head)
	{
		condition.wait();
	}
}


void TicketMutex::leave()
{
	if (DEBUG)
	{
		GMSEC_DEBUG << "TicketMutex:" << this << ": thread " << StdThread::getCurrentID() << ": leaving";
	}

	AutoMutex hold(condition.getMutex());
	++head;
	condition.broadcast(Condition::USER);
}


AutoTicket::AutoTicket(TicketMutex& t, bool acquire)
	: ticketer(t),
	  owned(false)
{
	if (acquire)
	{
		enter();
	}
}


AutoTicket::~AutoTicket()
{
	leave();
}


void AutoTicket::enter()
{
	if (!owned)
	{
		ticketer.enter();
	}
	owned = true;
}


void AutoTicket::leave()
{
	if (owned)
	{
		owned = false;
		ticketer.leave();
	}
}
