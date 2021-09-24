/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSubscriptionInfo.cpp
 *
 *  @brief This file contains the opaque wrapper for subscription info used
 *  when setting up and tearing down subscriptions to the middleware.
 */


#include <gmsec4/internal/mist/InternalSubscriptionInfo.h>

#include <gmsec4/SubscriptionInfo.h>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{


InternalSubscriptionInfo::InternalSubscriptionInfo(gmsec::api::SubscriptionInfo* info, ConnectionManagerCallback* cb)
	: m_info(info),
	  m_callback(cb)
{
}


gmsec::api::SubscriptionInfo* InternalSubscriptionInfo::getInfo() const
{
	return m_info;
}


const char* InternalSubscriptionInfo::getSubject() const
{
	return m_info->getSubject();
}


ConnectionManagerCallback* InternalSubscriptionInfo::getCallback() const
{
	return m_callback;
}


void* InternalSubscriptionInfo::getInternal() const
{
	return m_info->getInternal();
}


void InternalSubscriptionInfo::setInternal(void* internal)
{
	m_info->setInternal(internal);
}


void* InternalSubscriptionInfo::getCallbackFunction() const
{
	return m_info->getCallbackFunction();
}


void InternalSubscriptionInfo::setCallbackFunction(void* cb)
{
	m_info->setCallbackFunction(cb);
}


} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec
