/*
 * Copyright 2007-2018 United States Government as represented by the
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
/// Please note that because users are able to create their own EventCallback class,
/// reentrancy is not guaranteed unless if they implement their own reentrancy rules. Also note that
/// because an EventCallback can be registered to multiple Connections, it can be run
/// concurrently amongst those connections. Because of this, the use of a AutoMutex is suggested to
/// enforce thread safety.
/// </summary>
///
/// <seealso cref="Connection,RegisterEventCallback(Connection,ConnectionEvent, EventCallback)"/>

public class EventCallback : global::System.IDisposable {

  /// <summary>
  /// This function is called in response to a error after a call to RegisterEventCallback().
  /// Please note that if an EventCallback is registered to multiple connections, OnEvent() can
  /// be invoked concurrently from the different connection threads.
  /// </summary>
  ///
  /// <remarks>
  /// <b>DO NOT DESTROY</b> the Connection object that is passed into this function by the API.
  /// It is owned by the API and does not need to be managed by the client program. Also, it
  /// should not be stored by the client program beyond the scope of this callback function.
  /// </remarks>
  ///
  /// <param name="conn">connection on which the event was received</param>
  /// <param name="status">The status of the event being reported</param>
  /// <param name="event">The event that has occurred</param>
  public virtual void OnEvent(Connection conn, Status status, Connection.ConnectionEvent event) {
  }

  /// <summary>Default Constructor</summary>
  protected EventCallback() {
  }

}

}
