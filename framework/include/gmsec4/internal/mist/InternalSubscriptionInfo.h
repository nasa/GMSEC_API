/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSubscriptionInfo.h
 *
 *  @brief This file contains the opaque wrapper for subscription info used
 *  when setting up and tearing down subscriptions to the middleware.
 */

#ifndef GMSEC_API_INTERNAL_MIST_SUBSCRIPTIONINFO_H
#define GMSEC_API_INTERNAL_MIST_SUBSCRIPTIONINFO_H

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


class GMSEC_API InternalSubscriptionInfo
{
public:
	InternalSubscriptionInfo(gmsec::api::SubscriptionInfo* info, ConnectionManagerCallback* cb = 0);


	gmsec::api::SubscriptionInfo* CALL_TYPE getInfo() const;


	const char* CALL_TYPE getSubject() const;


	ConnectionManagerCallback* CALL_TYPE getCallback() const;


	void* CALL_TYPE getInternal() const;


	void CALL_TYPE setInternal(void* internal);


    void* CALL_TYPE getCallbackFunction() const;


    void CALL_TYPE setCallbackFunction(void* cb);


private:
	// Defined, but not implemented
	InternalSubscriptionInfo(const InternalSubscriptionInfo&);
	InternalSubscriptionInfo& operator=(const InternalSubscriptionInfo&);

	gmsec::api::SubscriptionInfo* m_info;
    ConnectionManagerCallback*    m_callback;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
