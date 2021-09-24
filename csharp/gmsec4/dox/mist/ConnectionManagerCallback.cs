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

/// <summary>
/// This class is an abstract base class for receiving a message via a callback.
/// A user created class, derived from this class, can be passed into Subscribe()
/// to have user code executed asynchronously when a message is received.
/// </summary>
/// <p>
/// Note that because users are able to create their own ConnectionManagerCallback class,
/// reentrancy is not guaranteed unless if reentrancy rules are specified.
/// <p>
/// In addition, if a ConnectionManagerCallback is registered to multiple Connection Managers,
/// OnMessage() can be invoked concurrently from different threads. Use of an AutoMutex is
/// suggested to enforce thread safety.
///
/// <seealso cref="ConnectionManager.Subscribe(String, ConnectionMangerCallback)"/>

public class ConnectionManagerCallback : global::System.IDisposable {

  /// <summary>
  /// This method is called by the API in response to a message, from either the Dispatch()
  /// call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class derived from
  /// ConnectionManagerCallback needs to be registered with a ConnectionManger, using Subscribe()
  /// in order to be called for a particular subject registration pattern.
  /// </summary>
  ///
  /// <remarks>
  /// <b>DO NOT STORE or CHANGE STATE</b> of the ConnectionManager object; it should only be used within
  /// the scope of the callback method.
  /// 
  /// <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback. Otherwise,
  /// make a copy of the Message object.
  /// </remarks>
  ///
  /// <param name="connMgr">ConnectionManager on which the message was received</param>
  /// <param name="msg">the received message</param>
  public virtual void OnMessage(ConnectionManager connMgr, Message msg) {
  }

  protected ConnectionManagerCallback() {
  }

}

}
