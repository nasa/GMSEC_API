#ifndef GMSEC_API_INTERNAL_SUBSCRIPTIONINFO_H
#define GMSEC_API_INTERNAL_SUBSCRIPTIONINFO_H

#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Callback;
class Connection;

namespace internal
{


class GMSEC_API InternalSubscriptionInfo
{
public:
	/**
	 * @fn InternalSubscriptionInfo(Connection* conn, const char* subject, Callback* cb, void* internal = 0)
	 * @desc Default constructor.
	 * @param conn - Pointer to the Connection object to associate with the subscription request.
	 * @param subject - The subject/pattern to associate with the subscription request.
	 * @param cb - The Callback (if any) to associate with the subscription request.
	 * @param internal - Pointer to internal (opaque) object that is associated with the subscription request.
	 */
	InternalSubscriptionInfo(Connection* conn, const char* subject, Callback* cb, void* internal = 0);


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
	 * @fn Connection& getConnection() const
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
	InternalSubscriptionInfo(const InternalSubscriptionInfo&);
	InternalSubscriptionInfo& operator=(const InternalSubscriptionInfo&);

	Connection* m_conn;
    std::string m_subject;
    Callback*   m_callback;
	void*       m_internal;
	void*       m_callbackFunc;
};

} // namespace internal
} // namespace api
} // namespace gmsec

#endif
