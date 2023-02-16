/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file InternalSubscriptionInfo.h
 */

#ifndef GMSEC_API5_INTERNAL_SUBSCRIPTIONINFO_H
#define GMSEC_API5_INTERNAL_SUBSCRIPTIONINFO_H

#include <gmsec5/Config.h>

#include <gmsec5/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Connection;
	class Callback;

namespace internal
{

class GMSEC_API InternalSubscriptionInfo
{
public:
	/**
	 * @fn InternalSubscriptionInfo(Connection* conn, const char* subject, const Config& config, Callback* cb)
	 *
	 * @brief Default constructor.
	 *
	 * @param conn - Pointer to the Connection object to associate with the subscription request.
	 * @param subject - The subject/pattern to associate with the subscription request.
	 * @param cb - The Callback (if any) to associate with the subscription request.
	 */
	InternalSubscriptionInfo(Connection* conn, const char* subject, const Config& config, Callback* cb);


	/**
	 * @fn std::string getSubjectPattern() const
	 * @brief Returns the subject pattern string used when setting up the subscription.
	 * @return Subject string.
	 */
	const char* CALL_TYPE getSubjectPattern() const;


	/**
	 * @fn const Config& getConfig() const
	 * @brief Returns the Config object associated with the subscription.
	 * @return A reference to a Config object.
	 */
	const Config& CALL_TYPE getConfig() const;


	/**
	 * @fn Callback* getCallback() const
	 * @brief Returns the Callback object, if any, used when setting up the subscription.
	 * @return Pointer to Callback object.
	 */
	Callback* CALL_TYPE getCallback() const;


	/**
	 * @fn Connection& getConnection() const
	 * @brief Returns the connection object associated with the subscription.
	 * @return Reference to Connection object.
	 */
	Connection* CALL_TYPE getConnection() const;


    /**
     * @fn void* getCallbackFunction() const
     * @return Returns pointer to callback function that is used by C subscribers.
     */
    void* CALL_TYPE getCallbackFunction() const;


    /**
     * @fn void setCallbackFunction(GMSEC_Callback* cb) const
     * @return Stores the address of callback function that is used by C subscribers.
     */
    void CALL_TYPE setCallbackFunction(void* cb);

private:
	// Defined, but not implemented
	InternalSubscriptionInfo();
	InternalSubscriptionInfo(const InternalSubscriptionInfo&);
	InternalSubscriptionInfo& operator=(const InternalSubscriptionInfo&);

	Connection*  m_conn;
    std::string  m_subjectPattern;
	Config       m_config;
    Callback*    m_callback;
	void*        m_internal;
	void*        m_callbackFunc;
};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
