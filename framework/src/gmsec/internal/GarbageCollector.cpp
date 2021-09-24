/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file GarbageCollector.cpp
 */

#include <gmsec/internal/GarbageCollector.h>

#include <gmsec/internal/API4_Adapter.h>

#include <gmsec/Connection.h>
#include <gmsec/Callback.h>
#include <gmsec/ErrorCallback.h>
#include <gmsec/ReplyCallback.h>
#include <gmsec/Message.h>

#include <gmsec4/Callback.h>
#include <gmsec4/EventCallback.h>
#include <gmsec4/ReplyCallback.h>


using namespace gmsec;
using namespace gmsec::util;


namespace gmsec
{
namespace internal
{

class CAPICallback : public Callback
{
private:
	GMSEC_CALLBACK *fCb;

public:
	CAPICallback(GMSEC_CALLBACK *cb)
	{
		fCb = cb;
	}

	virtual void CALL_TYPE OnMessage(Connection *conn, Message *msg)
	{
		if (NULL != fCb)
		{
			(*fCb)(conn,msg);
		}
	}

};


class CAPIErrorCallback : public ErrorCallback
{
private:
	GMSEC_ERROR_CALLBACK *fCb;

public:
	CAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb)
	{
		fCb = cb;
	}

	virtual void CALL_TYPE OnError(Connection *conn, Message *msg, Status *status, const char* event)
	{
		if (NULL != fCb)
		{
			(*fCb)(conn,msg,status,event);
		}
	}
};


class CAPIReplyCallback : public ReplyCallback
{
private:
	GMSEC_REPLY_CALLBACK *fReplyCb;
	GMSEC_ERROR_CALLBACK *fCb;

public:
	CAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er)
	{
		fReplyCb = cb;
		fCb = er;
	}

	virtual void CALL_TYPE OnReply(Connection *conn, Message *request, Message *reply)
	{
		if (NULL != fReplyCb)
		{
			(*fReplyCb)(conn,request,reply);
		}
	}
	virtual void CALL_TYPE OnError(Connection *conn, Message *request, Status *result, const char* event)
	{
		if (NULL != fCb)
		{
			(*fCb)(conn,request,result,event);
		}
	}
};


GarbageCollector::GarbageCollector()
{
}


GarbageCollector::~GarbageCollector()
{
	cbAPILkpItr itr = fCbAPILkps.begin();
	while (itr != fCbAPILkps.end())
	{
		CAPICallback *cb = (*itr).second;
		delete cb;
		itr++;
	}
	cbErrorLkpItr itr2 = fCbErrorLkps.begin();
	while (itr2 != fCbErrorLkps.end())
	{
		CAPIErrorCallback *cb = (*itr2).second;
		delete cb;
		itr2++;
	}

	for (EventCallbackLookup::iterator it = m_eventCallbackLookup.begin(); it != m_eventCallbackLookup.end(); ++it)
	{
		delete it->second;
	}

	for (DeprecatedReplyCallbackLookup::iterator it = m_depReplyCallbackLookup.begin(); it != m_depReplyCallbackLookup.end(); ++it)
	{
		delete it->second;
	}

	for (ReplyCallbackLookup::iterator it = m_replyCallbackLookup.begin(); it != m_replyCallbackLookup.end(); ++it)
	{
		delete it->second;
	}
}


CAPICallback *GarbageCollector::newCAPICallback(GMSEC_CALLBACK *cb)
{
	CAPICallback *cbObj = getCAPICallback(cb);
	if (cbObj == NULL)
	{
		cbObj = new CAPICallback(cb);
		fCbAPILkps[cb] = cbObj;
	}
	return cbObj;
}


CAPICallback *GarbageCollector::getCAPICallback(GMSEC_CALLBACK *cb)
{
	return fCbAPILkps[cb];
}


void GarbageCollector::deleteCAPICallback(GMSEC_CALLBACK *cb)
{
	CAPICallback *cbObj = fCbAPILkps[cb];
	fCbAPILkps[cb] = NULL;
	if (cbObj != NULL)
		delete cbObj;
}


CAPIErrorCallback *GarbageCollector::newCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb)
{
	CAPIErrorCallback *cbObj = getCAPIErrorCallback(cb);
	if (cbObj == NULL)
	{
		cbObj = new CAPIErrorCallback(cb);
		fCbErrorLkps[cb] = cbObj;
	}
	return cbObj;
}


CAPIErrorCallback *GarbageCollector::getCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb)
{
	return fCbErrorLkps[cb];
}


void GarbageCollector::deleteCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb)
{
	CAPIErrorCallback *cbObj = fCbErrorLkps[cb];
	fCbErrorLkps[cb] = NULL;
	if (cbObj != NULL)
		delete cbObj;
}


CAPIReplyCallback *GarbageCollector::newCAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er)
{
	CAPIReplyCallback *cbObj = getCAPIReplyCallback(cb);
	if (cbObj == NULL)
	{
		cbObj = new CAPIReplyCallback(cb,er);
		fCbReplyLkps[cb] = cbObj;
	}
	return cbObj;
}


void GarbageCollector::deleteCAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb)
{
	CAPIReplyCallback *cbObj = fCbReplyLkps[cb];
	fCbReplyLkps[cb] = NULL;
	if (cbObj != NULL)
		delete cbObj;
}


CAPIReplyCallback *GarbageCollector::getCAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb)
{
	return fCbReplyLkps[cb];
}


// Supports 3.x ErrorCallback to 4.x EventCallback
//
gmsec::api::EventCallback* GarbageCollector::newEventCallback(gmsec::Connection* conn, gmsec::ErrorCallback* ecb)
{
	gmsec::api::EventCallback* ecb4x = getEventCallback(ecb);

	if (ecb4x == NULL)
	{
		ecb4x = new API4_Adapter::API4EventCallback(conn, ecb);

		m_eventCallbackLookup[ecb] = ecb4x;
	}

	return ecb4x;
}


void GarbageCollector::delEventCallback(gmsec::ErrorCallback* ecb)
{
	gmsec::api::EventCallback* ecb4 = m_eventCallbackLookup[ecb];

	if (ecb4 != NULL)
	{
		m_eventCallbackLookup[ecb] = NULL;

		delete ecb4;
	}
}


gmsec::api::EventCallback* GarbageCollector::getEventCallback(gmsec::ErrorCallback* ecb)
{
	return m_eventCallbackLookup[ecb];
}


// Supports 3.x Callback to 4.x ReplyCallback (deprecated)
//
gmsec::api::ReplyCallback* GarbageCollector::newReplyCallback(gmsec::Connection* conn, gmsec::Callback* cb)
{
	gmsec::api::ReplyCallback* rcb4x = getReplyCallback(cb);

	if (rcb4x == NULL)
	{
		rcb4x = new API4_Adapter::API4ReplyCallback(conn, cb, NULL);

		m_depReplyCallbackLookup[cb] = rcb4x;
	}

	return rcb4x;
}


void GarbageCollector::delReplyCallback(gmsec::Callback* cb)
{
	gmsec::api::ReplyCallback* rcb4 = m_depReplyCallbackLookup[cb];

	if (rcb4 != NULL)
	{
		m_depReplyCallbackLookup[cb] = NULL;

		delete rcb4;
	}
}


gmsec::api::ReplyCallback* GarbageCollector::getReplyCallback(gmsec::Callback* cb)
{
	return m_depReplyCallbackLookup[cb];
}


// Supports 3.x ReplyCallback to 4.x ReplyCallback
//
gmsec::api::ReplyCallback* GarbageCollector::newReplyCallback(gmsec::Connection* conn, gmsec::ReplyCallback* rcb)
{
	gmsec::api::ReplyCallback* rcb4x = getReplyCallback(rcb);

	if (rcb4x == NULL)
	{
		rcb4x = new API4_Adapter::API4ReplyCallback(conn, NULL, rcb);

		m_replyCallbackLookup[rcb] = rcb4x;
	}

	return rcb4x;
}


void GarbageCollector::delReplyCallback(gmsec::ReplyCallback* rcb)
{
	gmsec::api::ReplyCallback* rcb4 = m_replyCallbackLookup[rcb];

	if (rcb4 != NULL)
	{
		m_replyCallbackLookup[rcb] = NULL;

		delete rcb4;
	}
}


gmsec::api::ReplyCallback* GarbageCollector::getReplyCallback(gmsec::ReplyCallback* rcb)
{
	return m_replyCallbackLookup[rcb];
}

} // namespace internal
} // namespace gmsec
