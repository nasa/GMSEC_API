/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file SubscriptionInfo.h
 *
 *  @brief This file contains the opaque wrapper for subscription info used
 *  when setting up and tearing down subscriptions to the middleware.
 */

#ifndef GMSEC_API5_SUBSCRIPTIONINFO_H
#define GMSEC_API5_SUBSCRIPTIONINFO_H

#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Callback;

	namespace internal
	{
	class InternalConnection;
	class InternalSubscriptionInfo;
	}


/**
 * @class SubscriptionInfo
 *
 * @brief Structure that contains the information the user has supplied when setting up a subscription.
 *
 * @sa Connection::subscribe
 * @sa Connection::unsubscribe
 */
class GMSEC_API SubscriptionInfo
{
public:
	/**
	 * @fn ~SubscriptionInfo()
	 *
	 * @brief Destructor
	 */
	~SubscriptionInfo();


	/**
	 * @fn const char* getSubjectPattern() const
	 *
	 * @brief Returns the subject pattern string used when setting up the subscription.
	 *
	 * @return Subject string.
	 */
	const char* CALL_TYPE getSubjectPattern() const;


private:
	friend class gmsec::api5::internal::InternalConnection;


	// Defined, but not implemented
	SubscriptionInfo();
	SubscriptionInfo(const SubscriptionInfo&);
	SubscriptionInfo& operator=(const SubscriptionInfo&);


	/**
	 * @fn SubscriptionInfo(gmsec::api5::internal::InternalSubscriptionInfo* internal)
	 * @brief Specialized constructor.
	 */
	SubscriptionInfo(gmsec::api5::internal::InternalSubscriptionInfo* internal);


	/**
	 * @fn void* getInternal() const
	 * @brief Returns an internal object, if any, used for specialized purposes for the subscription.
	 * @return Pointer to an opaque object.
	 */
	gmsec::api5::internal::InternalSubscriptionInfo* CALL_TYPE getInternal() const;


	// Member data
	gmsec::api5::util::StdUniquePtr<gmsec::api5::internal::InternalSubscriptionInfo> m_internal;
};

} // namespace api5
} // namespace gmsec

#endif
