/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_SUBSCRIPTION_INFO_NET_H
#define GMSEC_API_SUBSCRIPTION_INFO_NET_H


// C++ API native
#include <gmsec4/mist/SubscriptionInfo.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{

// Forward declarations
ref class ConnectionManagerCallback;


/// <summary>
/// Container for storing information that a user has provided when setting up a subscription.
///
/// <seealso cref="Connection::Subscribe(System::String^ subject, Callback^ cb)"/>
/// </summary>

public ref class SubscriptionInfo
{
public:
	~SubscriptionInfo();


	/// <summary>Returns the subject/topic associated with the subscription.</summary>
	System::String^ GetSubject();


	/// <summary>Returns the handle to the ConnectionManagerCallback (if any) associated with the subscription.</summary>
	ConnectionManagerCallback^ GetCallback();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!SubscriptionInfo();


internal:
    /// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
    ///
    /// <param name="info">unmanaged implementation to initialize with</param>
	SubscriptionInfo(gmsec::api::mist::SubscriptionInfo* info);


    /// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
    ///
    /// <param name="info">unmanaged implementation to initialize with</param>
    /// <param name="callback">managed reference to a ConnectionManagerCallback</param>
	SubscriptionInfo(gmsec::api::mist::SubscriptionInfo* info, ConnectionManagerCallback^ callback);


    /// <summary>Get unmanaged implementation version</summary>
    ///
    /// <returns>Unmanaged implementation</returns>
    gmsec::api::mist::SubscriptionInfo* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


    /// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
    void SetUnmanagedImplementation(gmsec::api::mist::SubscriptionInfo* info);


private:
	gmsec::api::mist::SubscriptionInfo* m_impl;
	ConnectionManagerCallback^          m_callback;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
