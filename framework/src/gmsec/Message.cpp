/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file Message.cpp
 *
 * This file forwards functions from the C++ Message interface to the
 * implementation.
 */


#include <gmsec/Message.h>
#include <gmsec/internal/InternalMessage.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/util/timeutil.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::util;


Message::Message()
	: ptr(new InternalMessage())
{
}


Message::Message(const char* subject, GMSEC_MSG_KIND msgKind)
	: ptr(new InternalMessage(subject, msgKind))
{
}


Message::Message(const char* subject, GMSEC_MSG_KIND msgKind, Config* config)
	: ptr(new InternalMessage(subject, msgKind, config))
{
}


Message::Message(InternalMessage* p)
	: ptr(p)
{
}


Message::~Message()
{
	delete ptr;
	ptr = 0;
}


// This function has been deprecated; remove for API 4.0
bool Message::isValid()
{
	return IsValid();
}


bool Message::IsValid()
{
	return ptr->isValid();
}


const char * Message::GetLibraryRootName()
{
	return ptr->GetLibraryRootName();
}


Status Message::SetConfig(Config *config)
{
	return ptr->SetConfig(config);
}


Status Message::SetKind(GMSEC_MSG_KIND kind)
{
	return ptr->SetKind(kind);
}


Status Message::GetKind(GMSEC_MSG_KIND &kind)
{
	return ptr->GetKind(kind);
}


Status Message::SetSubject(const char *subject)
{
	return ptr->SetSubject(subject);
}


Status Message::GetSubject(const char *&subject)
{
	return ptr->GetSubject(subject);
}


Status Message::ClearFields()
{
	return ptr->ClearFields();
}


Status Message::AddField(Field &field)
{
	return ptr->AddField(field);
}


Status Message::ClearField(const char *name)
{
	return ptr->ClearField(name);
}


Status Message::GetField(const char *name, Field &field)
{
	return ptr->GetField(name, field);
}


Status Message::GetFieldCount(GMSEC_I32 &count)
{
	return ptr->GetFieldCount(count);
}


Status Message::GetFirstField(Field &field)
{
	return ptr->GetFirstField(field);
}


Status Message::GetNextField(Field &field)
{
	return ptr->GetNextField(field);
}


Status Message::CopyFields(Message *toMsg)
{
	return ptr->CopyFields(toMsg->ptr);
}


Status Message::ToXML(const char *&xml)
{
	return ptr->ToXML(xml);
}


Status Message::FromXML(const char *xml)
{
	return ptr->FromXML(xml);
}


const char* Message::ToJSON()
{
	return ptr->ToJSON();
}


Status Message::FromJSON(const char *json)
{
	return ptr->FromJSON(json);
}


Status Message::GetMSGSize(GMSEC_U32 &size)
{
	return ptr->GetMSGSize(size);
}


// TODO: public or private?
// TODO: is UNSET different from invalid?
// TODO: make the string constants part of the interface?

GMSEC_MSG_KIND Message::LookupKind(const char *kind)
{
	if (kind == NULL)
		return GMSEC_MSG_UNSET;

	else if (stringEquals(kind, "PUBLISH"))
		return GMSEC_MSG_PUBLISH;

	else if (stringEquals(kind, "REQUEST"))
		return GMSEC_MSG_REQUEST;

	else if (stringEquals(kind, "REPLY"))
		return GMSEC_MSG_REPLY;

	return GMSEC_MSG_UNSET;
}


const char *Message::GetTime()
{
	static char buffer[32];
	double time_s = getTime_s();
	formatTime_s(time_s, buffer);
	return buffer;
}

Status Message::Reset(FieldIterator &iter)
{
	return ptr->Reset(iter);
}

