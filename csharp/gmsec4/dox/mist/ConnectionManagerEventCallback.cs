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
/// This class is the abstract base class for event callbacks. A user created class, derived
/// from this class, can be passed into RegisterEventCallback() to have user code executed
/// asynchronously when an event occurs in the connection object.
/// <p>
/// Note that because users are able to create their own ConnectionManagerEventCallback class, reentrancy
/// is not guaranteed unless if they implement their own reentrancy rules.
/// <p>
/// In addition, if a ConnectionManagerEventCallback is registered to multiple Connection Manager objects,
/// OnEvent() can be invoked concurrently from different threads. Use of an AutoMutex is suggested to
/// enforce thread safety.
/// </summary>
///
/// <seealso cref="Connection,RegisterEventCallback(Connection,ConnectionEvent, EventCallback)"/>

public class ConnectionManagerEventCallback : global::System.IDisposable {

  /// <summary>
  /// This method is called in response to an event after a call to RegisterEventCallback().
  /// Note that if an EventCallback is registered to multiple connections, OnEvent() can
  /// be invoked concurrently from the different connection threads.
  /// </summary>
  ///
  /// <remarks>
  /// <b>DO NOT STORE or CHANGE STATE</b> of the ConnectionManager object that is passed to the callback
  /// method.
  /// 
  /// <b>DO NOT STORE</b> the Status object for use beyond the scope of the callback. Otherwise,
  /// make a copy of the Status object.
  /// </remarks>
  ///
  /// <param name="connMgr">ConnectionManager on which the event was received</param>
  /// <param name="status">The status of the event being reported</param>
  /// <param name="event">The event that has occurred</param>
  public virtual void OnEvent(ConnectionManager connMgr, Status status, Connection.ConnectionEvent event) {
  }

  protected ConnectionManagerEventCallback() {
  }

}

}
