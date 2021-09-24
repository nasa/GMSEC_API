/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_SUBSCRIPTION_INFO_NET_H
#define GMSEC_API_SUBSCRIPTION_INFO_NET_H


// C++ API native
#include <gmsec4/SubscriptionInfo.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Callback;


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


	/// <summary>Returns the handle to the callback (if any) associated with the subscription.</summary>
	Callback^ GetCallback();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!SubscriptionInfo();


internal:
    /// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
    ///
    /// <param name="info">unmanaged implementation to initialize with</param>
	SubscriptionInfo(gmsec::api::SubscriptionInfo* info);


    /// <summary>Get unmanaged implementation version</summary>
    ///
    /// <returns>Unmanaged implementation</returns>
    gmsec::api::SubscriptionInfo* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


    /// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
    void SetUnmanagedImplementation(gmsec::api::SubscriptionInfo* info);


private:
	gmsec::api::SubscriptionInfo* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
