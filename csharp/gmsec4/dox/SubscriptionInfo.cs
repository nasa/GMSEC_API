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
/// <seealso cref="Connection.Subscribe(string)"/>
/// <seealso cref="Connection.Subscribe(string, Config)"/>
/// <seealso cref="Connection.Subscribe(string, Callback)"/>
/// <seealso cref="Connection.Subscribe(string, Config, Callback)"/>
/// <seealso cref="Connection.Unsubscribe()"/>

public class SubscriptionInfo : global::System.IDisposable {

  /// <summary>Returns the subject/topic associated with the subscription.</summary>
  public string GetSubject() {
  }

  /// summary>Returns the handle to the callback (if any) associated with the subscription.</summary>
  public Callback GetCallback() {
  }

}

}
