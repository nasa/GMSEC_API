/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MBReaderThread.h
 *
 *  @author Arturo Mayorga
 *  @author GSFC NASA
 *  @date
 *
 *  This file contains the definition for the MBReaderThread class used by the Magic Bus
 */


#ifndef MB_READER_THREAD_H
#define MB_READER_THREAD_H

#include "MBConnection.h"
#include "TCPSocketClientArray.h"

#include <gmsec4/internal/RequestSpecs.h>

#include <gmsec4/Message.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/Mutex.h>

#include <queue>
#include <string>

#define SUBSCRIBE_ACK 27


namespace gmsec_messagebus
{

class MBConnection;
class InputManager;

class MBReaderThread
{
public:
	/**
	 * @fn MBReaderThread(TCPSocketClientArray* sock)
	 * constructor that requires a socket instance
	 * for communication
	 */
	MBReaderThread(TCPSocketClientArray* sock);


	/**
	 * @fn ~MBReaderThread()
	 * default destructor
	 */
	~MBReaderThread();


	/**
	 * @fn run()
	 * main entry point for the Thread class
	 * that contains the main reading loop
	 */
	void CALL_TYPE run();


	/**
	 * @fn shutdown()
	 * tells the main loop to exit and waits for confirmation
	 */
	void CALL_TYPE shutdown();


	/**
	 * @fn getQueuedMsg(long timeout, double endTime)
	 * extract the next message from the published message queue
	 */
	gmsec::api::Message* getQueuedMsg(long timeout, double endTime);


	/**
	 * @fn searchForReply(const std::string& msgID, long timeout, double endTime)
	 * extract the message corresponding to the hash value provided by msgID
	 */
	gmsec::api::Message* searchForReply(const std::string& msgID, long timeout, double endTime);


	/**
	 * @fn getConnWasDropped()
	 * returns true if there was a broken pipe
	 */
	bool getConnWasDropped();


	/**
	 * @fn getIsRunning()
	 * returns true if the reader is running, otherwise false
	 */
	bool getIsRunning();


	void setConnection(MBConnection* conn, const gmsec::api::internal::RequestSpecs& specs);


private:
	/**
	 * @fn pushMsg(gmsec::api::Message* msg)
	 * add a new message to the queue
	 */
	void pushMsg(gmsec::api::Message* msg);


	/**
	 * @fn pushReply(gmsec::api::Message* reply)
	 * forwards the reply to the associated MBConnection
	 */
	void pushReply(gmsec::api::Message* reply);


	// Member data
	TCPSocketClientArray*            m_sock;           // socket used to read data

	std::queue<gmsec::api::Message*> m_msgQueue;       // incoming message queue
	gmsec::api::util::Mutex          m_queueMutex;     // mutex for the message gueue

	MBConnection*                    m_connection;     // the connection to deliver replys to

	gmsec::api::util::AtomicBoolean  m_isRunning;      // flag indicating if the reader thread is running
	gmsec::api::util::AtomicBoolean  m_stopRunning;    // flag to indicate the reader thread to stop
	gmsec::api::util::AtomicBoolean  m_connWasDropped; // this flag will go up in case of a broken pipe

	bool                             m_replyLegacy;
	bool                             m_replyExpose;
	std::string                      m_replySubject;
};

}  // end namespace gmsec_messagebus

#endif
