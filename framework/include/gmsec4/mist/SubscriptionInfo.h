/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file SubscriptionInfo.h
 *
 *  @brief This file contains the opaque wrapper for subscription info used
 *  when setting up and tearing down subscriptions to the middleware.
 */

#ifndef GMSEC_API_MIST_SUBSCRIPTIONINFO_H
#define GMSEC_API_MIST_SUBSCRIPTIONINFO_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class SubscriptionInfo;

namespace mist
{
	// Forward declaration(s)
	class ConnectionManagerCallback;
	namespace internal
	{
	class InternalConnectionManager;
	class InternalSubscriptionInfo;
	}


/**
 * @class SubscriptionInfo
 * @brief Structure that contains the information the user has supplied when setting up a subscription.
 *
 * @sa Connection::subscribe @n
 *     Connection::unsubscribe
 */
class GMSEC_API SubscriptionInfo
{
public:
	/**
	 * @fn ~SubscriptionInfo()
	 * @brief Destructor
	 */
	~SubscriptionInfo();


	/**
	 * @fn const char* getSubject() const
	 * @brief Returns the subject string used when setting up the subscription.
	 * @return Subject string.
	 */
	const char* CALL_TYPE getSubject() const;


	/**
	 * @fn ConnectionManagerCallback* getCallback() const
	 * @brief Returns the ConnectionManagerCallback object, if any, used when setting up the subscription.
	 * @return Pointer to ConnectionManagerCallback object.
	 */
	ConnectionManagerCallback* CALL_TYPE getCallback() const;


	/**
	 * @cond
	 * @fn void* getCallbackFunction() const
	 * @return Returns pointer to callback function that is used by C subscribers.
	 */
	void* CALL_TYPE getCallbackFunction() const;
	/* @endcond */


private:
	friend class gmsec::api::mist::internal::InternalConnectionManager;


	// Defined, but not implemented
	SubscriptionInfo(const SubscriptionInfo&);
	SubscriptionInfo& operator=(const SubscriptionInfo&);


	/**
	 * @fn SubscriptionInfo(gmsec::api::SubscriptionInfo* info, ConnectionManagerCallback* cb = 0)
	 *
	 * @desc Default constructor
	 *
	 * @param info - SubscriptionInfo object that is returned by Connection::subscribe()
	 * @param cb   - ConnectionManagerCallback, if any, associated with the subscription
	 */
	SubscriptionInfo(gmsec::api::SubscriptionInfo* info, ConnectionManagerCallback* cb = 0);


	/**
	 * @fn gmsec::api::SubscriptionInfo* getInfo() const
	 * @desc Returns the SubscriptionInfo object that was provided by gmsec::api::Connection.
	 */
	gmsec::api::SubscriptionInfo* CALL_TYPE getInfo() const;


	/**
	 * @fn void* getInternal() const
	 * @brief Returns an internal object, if any, used for specialized purposes for the subscription.
	 * @return Pointer to an opaque object.
	 */
	void* CALL_TYPE getInternal() const;


	/**
	 * @fn void setInternal(void* internal)
	 * @brief Allows caller to associate an opaque object, for specialized purposes, with the subscription info.
	 * @param internal - Pointer to an opaque object.
	 */
	void CALL_TYPE setInternal(void* internal);


	/**
	 * @fn void setCallbackFunction(void* cb) const
	 * @brief Stores the address of callback function that is used by C subscribers.
	 * @param The handle to the callback function.
	 */
	void CALL_TYPE setCallbackFunction(void* cb);


	// Member data
	gmsec::api::mist::internal::InternalSubscriptionInfo* m_internal;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
