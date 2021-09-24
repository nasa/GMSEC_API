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
 */

#include <gmsec4/mist/SubscriptionInfo.h>

#include <gmsec4/mist/ConnectionManagerCallback.h>

#include <gmsec4/internal/mist/InternalSubscriptionInfo.h>

#include <gmsec4/SubscriptionInfo.h>



namespace gmsec
{
namespace api
{
namespace mist
{


SubscriptionInfo::SubscriptionInfo(gmsec::api::SubscriptionInfo* info, ConnectionManagerCallback* cb)
	: m_internal(new internal::InternalSubscriptionInfo(info, cb))
{
}


SubscriptionInfo::~SubscriptionInfo()
{
	delete m_internal;
}


gmsec::api::SubscriptionInfo* SubscriptionInfo::getInfo() const
{
	return m_internal->getInfo();
}


const char* SubscriptionInfo::getSubject() const
{
	return m_internal->getSubject();
}


ConnectionManagerCallback* SubscriptionInfo::getCallback() const
{
	return m_internal->getCallback();
}


void* SubscriptionInfo::getCallbackFunction() const
{
	return m_internal->getCallbackFunction();
}



void* SubscriptionInfo::getInternal() const
{
	return m_internal->getInternal();
}


void SubscriptionInfo::setInternal(void* internal)
{
	m_internal->setInternal(internal);
}


void SubscriptionInfo::setCallbackFunction(void* cb)
{
	m_internal->setCallbackFunction(cb);
}


} // namespace mist
} // namespace api
} // namespace gmsec
