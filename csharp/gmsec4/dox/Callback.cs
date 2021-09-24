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
/// <p>
/// Note that because users are able to create their own Callback class,
/// reentrancy is not guaranteed unless if they implement their own reentrancy rules.
/// <p>
/// In addition, if a Callback is registered to multiple Connections object, OnMessage() can be invoked
/// concurrently from different connection threads. Use of a AutoMutex is suggested to enforce thread safety.
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
    /// <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to
    /// the callback method, nor store it for use beyond the scope of the callback method.
    ///
    /// <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback.
    /// Otherwise, make a copy of the Message object.
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
