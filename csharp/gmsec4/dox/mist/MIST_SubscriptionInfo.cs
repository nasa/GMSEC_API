/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>Container for storing information that a user has provided when setting up a subscription.</summary>
///
/// <remarks>Users typically would not need to use this class unless they are unsubscribing.</remarks>
///
/// <seealso cref="ConnectionManager.Subscribe(string)"/>
/// <seealso cref="ConnectionManager.Subscribe(string, Config)"/>
/// <seealso cref="ConnectionManager.Subscribe(string, ConnectionManagerCallback)"/>
/// <seealso cref="ConnectionManager.Subscribe(string, Config, ConnectionManagerCallback)"/>
/// <seealso cref="ConnectionManager.Unsubscribe()"/>

public class MIST_SubscriptionInfo : global::System.IDisposable {

  /// <summary>Returns the subject/topic associated with the subscription.</summary>
  public string GetSubject() {
    string ret = GmsecPINVOKE.MIST_SubscriptionInfo_GetSubject(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  /// <summary>Returns the handle to the ConnectionManagerCallback (if any) associated with the subscription.</summary>
  public ConnectionManagerCallback GetCallback() {
    global::System.IntPtr cPtr = GmsecPINVOKE.MIST_SubscriptionInfo_GetCallback(swigCPtr);
    ConnectionManagerCallback ret = (cPtr == global::System.IntPtr.Zero) ? null : new ConnectionManagerCallback(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
