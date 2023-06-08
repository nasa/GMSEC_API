/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Callback.h
 *
 *  @brief This file contains the abstract base class for receiving a message using a callback.
 */

#ifndef GMSEC_API5_CALLBACK_H
#define GMSEC_API5_CALLBACK_H


#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Connection;
	class Message;


/**
 * @class Callback
 *
 * @brief This class is the abstract base class for received message callbacks.
 * A user created class, derived from this class, can be passed into
 * %subscribe() to have user code executed asynchronously when a message is received.
 *
 * Note that because users are able to create their own Callback class,
 * reentrancy is not guaranteed unless if reentrancy rules are specified.
 *
 * In addition, if a Callback is registered to multiple Connection objects,
 * onMessage() can be invoked concurrently from different threads. Use of an AutoMutex
 * is suggested to enforce thread safety.
 *
 * Example Callback class:
 * @code
   class MyCallback : public Callback
   {
   public:
       virtual void onMessage(Connection& conn, const Message& msg)
       {
           gmsec::api5::util::AutoMutex lock(myMutex);
  
           std::cout << msg.toXML() << std::endl;
       }
  
   private:
       gmsec::api5::util::Mutex myMutex;
   };
 * @endcode
 *
 * Example callback registration:
 * @code
   int main(int argc, char** argv)
   {
       Config config(argc, argv);
       MyCallback* cb = new MyCallback();
       try {
           Connection conn(config);
           conn.connect();
           conn.subscribe("GMSEC.MISSION.SAT.>", cb);
           ...
       }
       catch (GmsecException& e) {
           std::cerr << e.what() << std::endl;
       }
   }
 * @endcode
*/
class GMSEC_API Callback
{
public:
	// base class needs virtual destructor
	virtual ~Callback() { }


	/**
	 * @fn onMessage(Connection& conn, const Message& msg)
	 *
	 * @brief This method is called by the API in response to a message, from either
	 * the dispatch() call or inside the Auto-dispatcher after a startAutoDispatch() call.
	 * A class derived from Callback needs to be registered with a connection,
	 * using subscribe() in order to be called for a particular subject registration pattern.
	 *
	 * If a Callback is registered to multiple connections, onMessage() can be
	 * invoked concurrently from different connection threads.
	 *
	 * The prototype for this method is:
	 * @code virtual void onMessage(Connection& conn, const Message& msg) @endcode
	 *
	 * @note <b>DO NOT STORE or CHANGE STATE</b> of the Connection object; it should only be used within
	 * the scope of the callback method.
	 *
	 * @note <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback. Otherwise,
	 * make a copy of the Message object.
	 *
	 * @param conn - Connection on which the message was received
	 * @param msg - the received Message
	 *
	 * @sa Connection::subscribe(const char* subject, Callback* cb)
	 * @sa Connection::dispatch(const Message& msg)
	 * @sa Connection::startAutoDispatch()
	 */
	virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg) = 0;
};

} // namespace api5
} // namespace gmsec

#endif
