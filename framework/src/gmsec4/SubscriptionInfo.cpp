/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file SubscriptionInfo.cpp
 *
 *  @brief This file contains the opaque wrapper for subscription info used
 *  when setting up and tearing down subscriptions to the middleware.
**/

#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/Callback.h>
#include <gmsec4/Connection.h>

#include <gmsec4/internal/InternalSubscriptionInfo.h>

using namespace gmsec::api;
using namespace gmsec::api::internal;



SubscriptionInfo::SubscriptionInfo(Connection* conn, const char* subject, Callback* cb, void* internal)
	: m_iSubInfo(new InternalSubscriptionInfo(conn, subject, cb, internal))
{
}


SubscriptionInfo::~SubscriptionInfo()
{
	delete m_iSubInfo;
}


const char* SubscriptionInfo::getSubject() const
{
	return m_iSubInfo->getSubject();
}


Callback* SubscriptionInfo::getCallback() const
{
	return m_iSubInfo->getCallback();
}


Connection* SubscriptionInfo::getConnection() const
{
	return m_iSubInfo->getConnection();
}


void* SubscriptionInfo::getInternal() const
{
	return m_iSubInfo->getInternal();
}


void SubscriptionInfo::setInternal(void* internal)
{
	m_iSubInfo->setInternal(internal);
}


void* SubscriptionInfo::getCallbackFunction() const
{
	return m_iSubInfo->getCallbackFunction();
}


void SubscriptionInfo::setCallbackFunction(void* cb)
{
	m_iSubInfo->setCallbackFunction(cb);
}
