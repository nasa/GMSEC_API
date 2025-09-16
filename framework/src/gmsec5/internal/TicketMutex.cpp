/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file TicketMutex.cpp
 *
 *  This file contains the ticket (FIFO) Mutex definition.
 */

#include <gmsec5/internal/TicketMutex.h>

#include <gmsec5/util/StdThread.h>
#include <gmsec5/util/Log.h>


using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


TicketMutex::TicketMutex()
	: head(0),
	  tail(0)
{
}


TicketMutex::~TicketMutex()
{
}


void TicketMutex::enter()
{
	AutoMutex hold(condition.getMutex());
	GMSEC_U32 id = tail++;

	while (id != head)
	{
		condition.wait();
	}
}


void TicketMutex::leave()
{
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
