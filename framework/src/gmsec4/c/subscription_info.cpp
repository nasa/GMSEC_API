/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file subscription_info.cpp
 *
 * @brief This file contains functions for the management of a SubscriptionInfo object.
 */


#include <gmsec4/c/subscription_info.h>

#include <gmsec4/SubscriptionInfo.h>

#include <gmsec4/mist/SubscriptionInfo.h>



const char* CALL_TYPE subscriptionInfoGetSubject(const GMSEC_SubscriptionInfo sinfo)
{
	const char* subject = NULL;

	if (sinfo.info && sinfo.fromMIST)
	{
		const gmsec::api::mist::SubscriptionInfo* i = reinterpret_cast<const gmsec::api::mist::SubscriptionInfo*>(sinfo.info);

		if (i)
		{
			subject = i->getSubject();
		}
	}
	else if (sinfo.info)
	{
		const gmsec::api::SubscriptionInfo* i = reinterpret_cast<const gmsec::api::SubscriptionInfo*>(sinfo.info);

		if (i)
		{
			subject = i->getSubject();
		}
	}

	return subject;
}


GMSEC_Callback* CALL_TYPE subscriptionInfoGetCallback(const GMSEC_SubscriptionInfo sinfo)
{
	GMSEC_Callback* cb = NULL;

	if (sinfo.info && !sinfo.fromMIST)
	{
		const gmsec::api::SubscriptionInfo* i = reinterpret_cast<const gmsec::api::SubscriptionInfo*>(sinfo.info);

		if (i)
		{
			cb = reinterpret_cast<GMSEC_Callback*>(i->getCallbackFunction());
		}
	}

	return cb;
}


GMSEC_ConnectionMgrCallback* CALL_TYPE subscriptionInfoGetConnectionMgrCallback(const GMSEC_SubscriptionInfo sinfo)
{
	GMSEC_ConnectionMgrCallback* cb = NULL;

	if (sinfo.info && sinfo.fromMIST)
	{
		const gmsec::api::mist::SubscriptionInfo* i = reinterpret_cast<const gmsec::api::mist::SubscriptionInfo*>(sinfo.info);

		if (i)
		{
			cb = reinterpret_cast<GMSEC_ConnectionMgrCallback*>(i->getCallbackFunction());
		}
	}

	return cb;
}
