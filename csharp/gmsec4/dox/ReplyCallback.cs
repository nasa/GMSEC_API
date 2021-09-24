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
/// This class defines the abstract base class for receiving replies from asynchronous request
/// messages. A user created class, derived from this class, can be passed into Request()
/// to have user code executed asynchronously when a reply is received or when an error
/// occurs.
/// <p>
/// Note that because users are able to create their own ReplyCallback class, reentrancy is not
/// guaranteed unless if they implement their own reentrancy rules.
/// <p>
/// In addition, if a ReplyCallback can be registered to multiple Connection objects, OnReply()
/// can be invoked concurrently from different threads. Use of an AutoMutex is suggested to
/// enforce thread safety.
/// </summary>
///
/// <seealso cref="Connection.Request(Message, int, ReplyCallback)"/>
/// <seealso cref="Connection.Request(Message, int, ReplyCallback, int)"/>

public class ReplyCallback : EventCallback {

  /// <summary>
  /// This method is called in response to an asynchronous reply message being received. 
  /// <p>
  /// Please note that if a ReplyCallback is registered to multiple connections, OnReply() can
  /// be invoked concurrently from the different connection threads.
  /// </summary>
  ///
  /// <remarks>
  /// <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to the callback method,
  /// nor store it for use beyond the scope of the callback method.
  ///
  /// <b>DO NOT STORE</b> the Message objects for use beyond the scope of the callback. Otherwise,
  /// make a copy of the Message object(s).
  /// </remarks>
  ///
  /// <param name="conn">connection on which the message was received</param>
  /// <param name="request">The pending request message</param>
  /// <param name="reply">The received reply message</param>
  public virtual void OnReply(Connection conn, Message request, Message reply) {
  }

  /// <summary>Default constructor</summary>
  protected ReplyCallback() {
  }

}

}
