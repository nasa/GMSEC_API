/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file SubscriptionInfo.cpp
 *
 *  @brief This file contains the opaque wrapper for subscription info used
 *  when setting up and tearing down subscriptions to the middleware.
 */

#include <gmsec5/SubscriptionInfo.h>

#include <gmsec5/internal/InternalSubscriptionInfo.h>

#include <gmsec5/Callback.h>

using namespace gmsec::api5;
using namespace gmsec::api5::internal;



SubscriptionInfo::SubscriptionInfo(InternalSubscriptionInfo* internal)
	: m_internal(internal)
{
}


SubscriptionInfo::~SubscriptionInfo()
{
}


const char* SubscriptionInfo::getSubjectPattern() const
{
	return m_internal->getSubjectPattern();
}


InternalSubscriptionInfo* SubscriptionInfo::getInternal() const
{
	return m_internal.get();
}
