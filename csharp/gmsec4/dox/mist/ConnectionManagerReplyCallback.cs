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
/// </summary>
/// <p>
/// Note that because users are able to create their own ConnectionManagerReplyCallback class,
/// reentrancy is not guaranteed unless they implement their own reentrancy rules.
/// <p>
/// In addition, if a ConnectionManagerReplyCallback is registered to multiple Connection Manager objects,
/// OnReply() can be invoked concurrently from different threads. Use of an AutoMutex is suggested to
/// enforce thread safety.
///
/// <seealso cref="ConnectionManager.Request(Message, int, ConnectionManagerReplyCallback)"/>
/// <seealso cref="ConnectionManager.Request(Message, int, ConnectionManagerReplyCallback, int)"/>

public class ConnectionManagerReplyCallback : ConnectionManagerEventCallback {

  /// <summary>
  /// This method is called in response to an asynchronous reply message being received. 
  /// <p>
  /// Note that if a ReplyCallback is registered to multiple connections, OnReply() can
  /// be invoked concurrently from the different connection threads.
  /// </summary>
  ///
  /// <remarks>
  /// <b>DO NOT STORE or CHANGE STATE</b> of the ConnectionManager object that is passed to the callback
  /// method.
  ///
  /// <b>DO NOT STORE</b> the Message objects for use beyond the scope of the callback. Otherwise,
  /// make a copy of the Message object(s).
  /// </remarks>
  ///
  /// <param name="connMgr">ConnectionManager on which the message was received</param>
  /// <param name="request">The pending request message</param>
  /// <param name="reply">The reply message</param>
  public virtual void OnReply(ConnectionManager connMgr, Message request, Message reply) {
  }

  /// <summary>Standard constructor</summary>
  protected ConnectionManagerReplyCallback() {
  }

}

}
