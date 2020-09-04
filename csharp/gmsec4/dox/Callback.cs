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
/// This class is an abstract base class for receiving a message via a callback.
/// A user created class, derived from this class, can be passed into Subscribe()
/// to have user code executed asynchronously when a message is received.
/// <p>
/// Please note that because users are able to create their own Callback class,
/// reentrancy is not guaranteed unless if they implement their own reentrancy rules. Also note that
/// because a Callback can be registered to multiple Connections, it can be run
/// concurrently amongst those connections. Because of this, the use of a AutoMutex is suggested to
/// enforce thread safety.
/// </summary>
///
/// <seealso cref="Connection.Subscribe(String, Callback)"/>

public class Callback : global::System.IDisposable {

    /// <summary>
    /// This method is called by the API in response to a message, from either the Dispatch()
    /// call or inside the Auto-dispatcher after a StartAutoDispatch() call. A class derived from
    /// Callback needs to be registered with a connection, using Subscribe() in order
    /// to be called for a particular subject registration pattern.
    /// </summary>
    ///
    /// <remarks>
    /// <b>DO NOT DESTROY</b> the Connection, or the Message that is passed into this method by the API.
    /// They are owned by the API and do not need to be managed by the client program. Also, they can
    /// not be stored by the client program beyond the scope of this callback method.
    /// </remarks>
    ///
    /// <param name="conn">connection on which the message was received</param>
    /// <param name="msg">the received message</param>
	public virtual void OnMessage(Connection conn, Message msg) {
	}

	protected Callback() {
	}
}

}
