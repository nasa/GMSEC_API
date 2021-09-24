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
**/

#ifndef GMSEC_API_SUBSCRIPTIONINFO_H
#define GMSEC_API_SUBSCRIPTIONINFO_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
	class Callback;
	class Connection;

	namespace mist
	{
		namespace internal
		{
			class InternalConnectionManager;
			class InternalSubscriptionInfo;
		}
	}

	namespace internal
	{
		class InternalConnection;
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
	 * @fn std::string getSubject() const
	 * @brief Returns the subject string used when setting up the subscription.
	 * @return Subject string.
	 */
	const char* CALL_TYPE getSubject() const;


	/**
	 * @fn Callback* getCallback() const
	 * @brief Returns the Callback object, if any, used when setting up the subscription.
	 * @return Pointer to Callback object.
	 */
	Callback* CALL_TYPE getCallback() const;


	/**
	 * @cond
	 * @fn void* getCallbackFunction() const
	 * @return Returns pointer to callback function that is used by C subscribers.
	 */
	void* CALL_TYPE getCallbackFunction() const;
	/* @endcond */


private:
	// Defined, but not implemented
	SubscriptionInfo(const SubscriptionInfo&);
	SubscriptionInfo& operator=(const SubscriptionInfo&);

	/**
	 * @fn SubscriptionInfo(Connection* conn, const char* subject, Callback* cb, void* internal = 0)
	 * @ desc Default constructor
	 * @param conn - Pointer to a Connection object.
	 * @param subject - Subject/pattern associated with the subscription
	 * @param cb - Callback associated with the subscription
	 * @param internal - Internal data, if any, associated with the subscription
	 */
	SubscriptionInfo(Connection* conn, const char* subject, Callback* cb, void* internal = 0);


	/**
	 * @fn Connection* getConnection() const
	 * @brief Returns the connection object associated with the subscription.
	 * @return Reference to Connection object.
	 */
	Connection* CALL_TYPE getConnection() const;


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
	gmsec::api::internal::InternalSubscriptionInfo* m_iSubInfo;

	friend class gmsec::api::internal::InternalConnection;
	friend class gmsec::api::mist::internal::InternalConnectionManager;
	friend class gmsec::api::mist::internal::InternalSubscriptionInfo;
};

} // namespace api
} // namespace gmsec

#endif
