/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscription_info.cpp
 *
 * @brief This file contains functions for the management of a SubscriptionInfo object.
 */


#include <gmsec5/c/subscription_info.h>

#include <gmsec5/SubscriptionInfo.h>

using namespace gmsec::api5;


const char* CALL_TYPE subscriptionInfoGetSubjectPattern(GMSEC_SubscriptionInfo info)
{
	const char* subject = NULL;

	if (info)
	{
		SubscriptionInfo* i = reinterpret_cast<SubscriptionInfo*>(info);

		if (i)
		{
			subject = i->getSubjectPattern();
		}
	}

	return subject;
}
