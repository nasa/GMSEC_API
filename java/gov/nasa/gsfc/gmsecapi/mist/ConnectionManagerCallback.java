/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** @file ConnectionManagerCallback.java
 *
 *  @brief This file contains the abstract base class for received message callbacks
**/

package gov.nasa.gsfc.gmsecapi.mist;

import gov.nasa.gsfc.gmsecapi.Message;


/**
 * @interface ConnectionManagerCallback
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This interface is for received message callbacks. 
 * A user created class, implemented with this interface, can be passed into 
 * %Subscribe() and to have user code executed asynchronously
 * when a message is received.
 * 
 * Example ConnectionManagerCallback class:
 * @code
 * class PublishCallback implements ConnectionManagerCallback
 * {
 *  public void OnMessage(ConnectionManager connMgr, Message msg) {
 *      GMSEC_String prtMsg = new GMSEC_String();
 *      msg.ToXML(prtMsg);
 *      System.out.println(prtMsg.Get());
 *      // Do not destroy the message here
 *  }
 * }
 * @endcode
 *
 * Example ConnectionManagerCallback registration:
 * @code
 * result = conn.Subscribe("GMSEC.TEST.PUBLISH", new PublishCallback());
 * if (result.isError())
 *     // handle error
 * @endcode
 *
 * @sa ConnectionManager::Subscribe(String subject, ConnectionManagerCallback cb) @n
 */
@Deprecated
public interface ConnectionManagerCallback
{
	/** @fn OnMessage(ConnectionManager connMgr, Message msg)
	 * @brief This function is called by the API in response to a message, from either the DispatchMsg()
	 * call or inside the Auto-dispatcher after a StartAutoDispatch() call.
	 *
	 * gmsec.mist.ConnectionManagerCallback needs to be registered with a ConnectionManager, using
	 * Subscribe() in order to be called for a particular subject registration pattern.
	 *
	 * Please note that if a ConnectionManagerCallback is registered to multiple connections, OnMessage
	 * can be invoked concurrently from different connection threads.
	 *
	 *   The prototype for this funtion is:
	 *   @code void OnMessage(ConnectionManager connMgr, Message msg) @endcode
	 *
	 * @param connMgr - connection manager on which the message was received
	 * @param msg - the received message
	 *
	 * @sa ConnectionManager.Subscribe(String subject, ConnectionManagerCallback cb) @n
	 *	   ConnectionManager.DispatchMsg(Message msg) @n
	 *     ConnectionManager.StartAutoDispatch() @n
	 *	   ConnectionManager.CloneMessage(Message in, Message out) @n
	 */
	public void OnMessage(ConnectionManager connMgr, Message msg);
}
