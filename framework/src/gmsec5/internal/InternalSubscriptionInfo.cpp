/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSubscriptionInfo.cpp
 */

#include <gmsec5/internal/InternalSubscriptionInfo.h>

#include <gmsec5/Connection.h>
#include <gmsec5/Callback.h>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;


InternalSubscriptionInfo::InternalSubscriptionInfo(Connection* conn, const char* subject, const Config& config, Callback* cb)
	: m_conn(conn),
	  m_subjectPattern(subject),
	  m_config(config),
	  m_callback(cb)
{
}


Connection* InternalSubscriptionInfo::getConnection() const
{
	return m_conn;
}


const char* InternalSubscriptionInfo::getSubjectPattern() const
{
	return m_subjectPattern.c_str();
}


const Config& InternalSubscriptionInfo::getConfig() const
{
	return m_config;
}


Callback* InternalSubscriptionInfo::getCallback() const
{
	return m_callback;
}


void* InternalSubscriptionInfo::getCallbackFunction() const
{
	return m_callbackFunc;
}


void InternalSubscriptionInfo::setCallbackFunction(void* cb)
{
	m_callbackFunc = cb;
}
