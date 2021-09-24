
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file VoidConnection.cpp
 *  This file provides a template for implementing a middleware wrapper.
 */

#include <VoidConnection.h>

#include <gmsec/internal/rawbuf.h>


using namespace gmsec;
using namespace gmsec::util;

using namespace std;


VoidConnection::VoidConnection(Config *cfg)
		:
		BaseConnection(cfg)
{
	Status result;

	char raw[4000];
	rawbuf buffer(raw, sizeof(raw));
	ostream os(&buffer);

	os << "VoidConnection::VoidConnection:";

	if (NULL != cfg)
	{
		const char * key;
		const char * value;
		for (result = cfg->GetFirst(key, value);
		        GMSEC_STATUS_NO_ERROR != result.GetCode();
		        result = cfg->GetNext(key, value))
		{
			os << "\n\t(" << key << ',' << value << ')';
		}
	}
	os << ends;

	LOG_DEBUG << raw;
}


VoidConnection::~VoidConnection()
{
	LOG_DEBUG << "gmsec_void:VoidConnection::~VoidConnection()";
}


const char *VoidConnection::GetLibraryVersion()
{
	return "v0.2.1";
}


Status VoidConnection::mwConnect()
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwConnect()";

	return result;
}


Status VoidConnection::mwDisconnect()
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwDisconnect()";

	return result;
}


Status VoidConnection::mwSubscribe(const char *subject, const gmsec::Config &config)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwSubscribe(" << subject << ')';

	return result;
}


Status VoidConnection::mwUnSubscribe(const char *subject)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwUnSubscribe(" << subject << ')';

	return result;
}


Status VoidConnection::mwCreateMessage(const char *subject,
		GMSEC_MSG_KIND kind, Message *&msg)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwCreateMessage("
			<< (subject ? subject : "[null]")
			<< ", kind=" << kind << ", msg=" << msg << ')';

	{
		VoidMessage * tmp = new VoidMessage();
		tmp->SetSubject(subject);
		tmp->SetKind(kind);
		msg = tmp->createExternal();
	}

	if (!msg)
	{
		result.Set(
		    GMSEC_STATUS_MESSAGE_ERROR,
		    GMSEC_INVALID_MESSAGE,
		    "VoidMessage object not created");
		LOG_WARNING << result.Get();
	}

	LOG_DEBUG << "gmsec_void:VoidConnection::mwCreateMessage => " << msg;

	return result;
}


#if 0
Status VoidConnection::DestroyMessage(Message *msg)
{
	Status result;

	LOG_INFO << "gmsec_void:VoidConnection::DestroyMessage(" << msg << ')';

	return result;
}
#endif


Status VoidConnection::mwPublish(Message *msg, const gmsec::Config &config)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::Publish(" << msg << ')';

	return result;
}


Status VoidConnection::mwRequest(Message *request, std::string &id)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwRequest(" << request << ')';

	return result;
}


Status VoidConnection::mwReply(Message *request, Message *reply)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwReply(request=" << request
			<< ", reply=" << reply << ')';

	return result;
}


Status VoidConnection::mwGetNextMsg(Message *&msg, GMSEC_I32 timeout)
{
	Status result;

	LOG_DEBUG << "gmsec_void:VoidConnection::mwGetNextMsg(msg=" << msg
			<< ", timeout=" << timeout << ')';

	msg = NULL;
	result.Set(GMSEC_STATUS_CONNECTION_ERROR,
			GMSEC_NO_MESSAGE_AVAILABLE,
			"Nothing left.");

	return result;
}


//	EOF VoidConnection.cpp

