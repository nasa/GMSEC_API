
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file RequestDispatcher.cpp
 *
 *  This file contains the Dispatcher class for Request messages.
 *
 *
*/

#include <gmsec/internal/RequestDispatcher.h>
#include <gmsec/internal/BaseConnection.h>
#include <gmsec/Message.h>
#include <gmsec/Callback.h>
#include <gmsec/ReplyCallback.h>

namespace gmsec
{
namespace internal
{

RequestDispatcher::RequestDispatcher()
	:
	gmsec::util::Thread(true),
	fConn(NULL),
	fRequest(NULL),
	fReply(NULL),
	fRCallbk(NULL),
	fCallbk(NULL),
	fTimeout(GMSEC_WAIT_FOREVER),
	fRunning(false),
	mAlive(true)
		
{
	setName("RequestDispatcher");
}


RequestDispatcher::~RequestDispatcher()
{
}


bool RequestDispatcher::init(BaseConnection *conn, Message *req, GMSEC_I32 timeout, Callback *cb)
{
	if ((fRunning == false) &&
	        (conn != NULL) &&
	        (req != NULL) &&
	        (cb != NULL))
	{
		fConn = conn;
		fCallbk = cb;
		fTimeout = timeout;
		fStatus = fConn->CloneMessage(req,fRequest);
		return true;
	}

	return false;

}


bool RequestDispatcher::init(BaseConnection *conn, Message *req, GMSEC_I32 timeout, ReplyCallback *cb)
{
	if ((fRunning == false) &&
	        (conn != NULL) &&
	        (req != NULL) &&
	        (cb != NULL))
	{
		fConn = conn;
		fRCallbk = cb;
		fTimeout = timeout;
		fStatus = fConn->CloneMessage(req,fRequest);
		return true;
	}

	return false;

}

void RequestDispatcher::run()
{
	Status result;
	fRunning = true;
	bool waitForever = false;
	bool timeOut = true;
	fStatus.ReSet();

	fRunning = true;

	GMSEC_I32 republish = BaseConnection::REPUBLISH_NEVER;

	/* If the timeout is negative, just run forever */
	if (fTimeout < 0)
		waitForever = true;

	while (mAlive && (fTimeout > 0 || waitForever))
	{
		fReply = NULL;

		/* To avoid a thread lock, we check the m_alive */
		/* variable every 250 millisecond */
		fStatus = fConn->Request(fRequest, 250, fReply, republish);
		if (!waitForever)
			fTimeout -= 250;

		if (fCallbk != NULL)
		{
			if (!fStatus.isError())
			{
				ConnectionBuddy extconn(fConn);
				fCallbk->OnMessage(extconn.ptr(), fReply);
				// clean up reply
				fConn->DestroyMessage(fReply);
				fReply = NULL;
				timeOut = false;
				break;
			}
		}
		else if (fRCallbk != NULL)
		{
			if (fReply != NULL)
			{
				ConnectionBuddy extconn(fConn);
				fRCallbk->OnReply(extconn.ptr(), fRequest, fReply);
				fConn->DestroyMessage(fReply);
				fReply = NULL;
				timeOut = false;
				break;
			}
		}
	}

	/* If there was an error and the thread hasn't been destroyed */
	if (mAlive && timeOut)
	{
		if (fRCallbk != NULL)
		{
			ConnectionBuddy extconn(fConn);
			fRCallbk->OnError(extconn.ptr(), fRequest, &fStatus, GMSEC_CONNECTION_REQUEST_TIMEOUT);
		}

		fConn->DispatchError(GMSEC_CONNECTION_REQUEST_TIMEOUT,fRequest,&fStatus);
	}

	// clean up request
	fConn->DestroyMessage(fRequest);
	fRequest = NULL;

	fRunning = false;
}


void RequestDispatcher::destroy()
{
	mAlive = false;
}


} // namespace internal
} // namespace gmsec

