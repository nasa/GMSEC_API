
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/* @file VoidMessage.cpp
 *  This file provides a template for implementing a middleware wrapper.
 */

#include <VoidMessage.h>

#include <gmsec/internal/Log.h>


using namespace gmsec;
using namespace gmsec::util;

VoidMessage::VoidMessage()
{
	LOG_VERBOSE << "VoidMessage::VoidMessage()";
}


VoidMessage::~VoidMessage()
{
	LOG_VERBOSE << "VoidMessage::~VoidMessage()";
}


#ifdef SUBCLASS_BASEMESSAGE

bool VoidMessage::isValid()
{
	LOG_VERBOSE << "VoidMessage::isValid()";

	return (true);
}


Status VoidMessage::SetKind(GMSEC_MSG_KIND kind)
{
	Status result;

	LOG_VERBOSE << "VoidMessage::mwSetKind(" << kind << ')';

	mType = (size_t) kind;
	switch (kind)
	{
	case GMSEC_MSG_UNSET:
		printf("gmsec_void		Not Specified\n");
		break;
	case GMSEC_MSG_PUBLISH:
		printf("gmsec_void		Proxy - non-blocking\n");
		break;
	case GMSEC_MSG_REQUEST:
		printf("gmsec_void		Send()/Receive() - blocking\n");
		break;
	case GMSEC_MSG_REPLY:
		printf("gmsec_void		Reply() - non/un-blocking\n");
		break;
	default:
		printf("gmsec_void		Error\n");
		result.Set(
		    GMSEC_STATUS_MESSAGE_ERROR,
		    GMSEC_UNKNOWN_MSG_TYPE,
		    "Neither Send, Reply, nor Proxy message type");
		break;
	}

	return result;
}


Status VoidMessage::GetKind(GMSEC_MSG_KIND &kind)
{
	Status result;

	LOG_DEBUG << "VoidMessage::GetKind(" << &kind << ')';

	kind = mType;

	return result;
}


Status VoidMessage::mwSetSubject(const char *subject)
{
	Status result;

	LOG_DEBUG << "VoidMessage::mwSetSubject("
	       (subject ? subject : "[null]") << ')';

	return result;
}


Status VoidMessage::GetSubject(const char *&subject)
{
	Status result;

	LOG_DEBUG << "VoidConnection::GetSubject(" << &subject << ')';

	subject = mName;

	return result;
}


bool VoidMessage::ProcessConfigValue(const char *name, const char *value)
{
	LOG_DEBUG << "VoidConnection::ProcessConfigValue(name="
			<< (name ? name : "[null]")
			<< ", value=" << (value ? value : "[null]");

	return true;
}


Status VoidMessage::ClearFields()
{
	Status result;

	LOG_DEBUG << "VoidConnection::ClearFields()";

	return result;
}


Status VoidMessage::ClearField(const char *name)
{
	Status result;

	LOG_DEBUG << "VoidConnection::ClearField(name="
			<< (name ? name : "[null]") << ')';

	return result;
}


Status VoidMessage::GetField(const char *name, Field &field)
{
	Status result;

	LOG_DEBUG << "VoidConnection::GetField(name="
			<< (name ? name : "[null]")
			<< ", field=" << &field << ')';

	return result;
}


Status VoidMessage::GetFieldCount(GMSEC_I32 &count)
{
	Status result;

	LOG_DEBUG << "VoidConnection::GetFieldCount(@count=" << &count << ')';

	return result;
}


Status VoidMessage::GetFirstField(Field &field)
{
	Status result;

	LOG_DEBUG << "VoidConnection::GetFirstField(@field=" << &field << ')';

	return result;
}


Status VoidMessage::GetNextField(Field &field)
{
	Status result;

	LOG_DEBUG << "VoidConnection::GetNextField(@field=" << &field << ')';
	result.Set(GMSEC_STATUS_MESSAGE_ERROR,
		GMSEC_FIELDS_END_REACHED,
		"Nominal end-of-fields");

	return result;
}


Status VoidMessage::GetMSGSize(unsigned long &size)
{
	Status result;

	LOG_DEBUG << "VoidConnection::GetMSGSize(@size=" << &size << ')';

	return result;
}

#endif /* SUBCLASS_BASEMESSAGE */


//	EOF	VoidMessage.cpp

