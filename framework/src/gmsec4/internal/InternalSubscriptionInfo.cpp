/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSubscriptionInfo.cpp
**/

#include <gmsec4/internal/InternalSubscriptionInfo.h>

#include <gmsec4/Callback.h>
#include <gmsec4/Connection.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;


InternalSubscriptionInfo::InternalSubscriptionInfo(Connection* conn, const char* subject, Callback* cb, void* internal)
	: m_conn(conn),
	  m_subject(subject),
	  m_callback(cb),
	  m_internal(internal)
{
}


Connection* InternalSubscriptionInfo::getConnection() const
{
	return m_conn;
}


const char* InternalSubscriptionInfo::getSubject() const
{
	return m_subject.c_str();
}


Callback* InternalSubscriptionInfo::getCallback() const
{
	return m_callback;
}


void* InternalSubscriptionInfo::getInternal() const
{
	return m_internal;
}


void InternalSubscriptionInfo::setInternal(void* internal)
{
	m_internal = internal;
}


void* InternalSubscriptionInfo::getCallbackFunction() const
{
	return m_callbackFunc;
}


void InternalSubscriptionInfo::setCallbackFunction(void* cb)
{
	m_callbackFunc = cb;
}
