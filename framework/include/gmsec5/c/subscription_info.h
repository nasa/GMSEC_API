/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file subscription_info.h
 *
 * @brief This file contains functions for the management of a SubscriptionInfo object.
 *
 * Example creation and use:
 * @code
 * GMSEC_SubscriptionInfo info = connectionSubscribe(conn, "GMSEC.FOO.BAR", status);
 *
 * printf("Subject: %s\n", subscriptionInfoGetSubjectPattern(info));
 *
 * connectionUnsubscribe(conn, info, status);
 * @endcode
 *
 * @sa connectionSubscribe()
 * @sa connectionSubscribeWithCallback()
 * @sa connectionUnsubscribe()
 */


#ifndef GMSEC_API5_C_SUBSCRIPTION_INFO_H
#define GMSEC_API5_C_SUBSCRIPTION_INFO_H

#include <gmsec5_defs.h>

#include <gmsec5/c/connection.h>

#include <gmsec5/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn const char* subscriptionInfoGetSubjectPattern(GMSEC_SubscriptionInfo info)
	 *
	 * @brief Provides the ability to examine the subject associated with a current subscription.
	 *
	 * @param[in] info - the handle to the SubscriptionInfo object.
	 *
	 * @return The subject/topic string, or NULL if an invalid SubscriptionInfo handle is given.
	 */
	GMSEC_API const char* subscriptionInfoGetSubjectPattern(GMSEC_SubscriptionInfo info);


#ifdef __cplusplus
}   // extern "C"
#endif

#endif
