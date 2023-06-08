/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file RequestThread.h
 *
 * @brief Interface between BaseConnection and middleware implementation of request/reply.
 */

#ifndef GMSEC_API5_INTERNAL_REQUEST_THREAD_H
#define GMSEC_API5_INTERNAL_REQUEST_THREAD_H

#include <gmsec5_defs.h>

#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/CountDownLatch.h>

#include <string>
#include <list>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class ReplyCallback;
	class Message;

namespace internal
{
	// Forward declaration(s)
	class InternalConnection;


/**	@struct PendingRequest
 *	@brief Holds state of a pending request
 */
struct PendingRequest
{
	PendingRequest(Message* request, GMSEC_I32 timeout, GMSEC_I32 localRepublish_ms);

	~PendingRequest();

	Message*       request;
	Message*       reply;
	bool           needToSend;
	bool           neverExpire;
	double         publishTime_s;
	double         expireTime_s;
	GMSEC_I32      republish_ms;
	ReplyCallback* replyCallback;
	std::string    id;
	bool           sentOnce;
	bool           isDone;
	bool           multiResponse;

	gmsec::api5::util::CountDownLatch sentLatch;

private:
	// Declared, but not implemented.
	PendingRequest(const PendingRequest &);
	PendingRequest &operator=(const PendingRequest &);
};


/**	@class RequestShared
 *	@brief Holds state for interface between BaseConnection and RequestThread.
 */
class RequestShared
{
public:
	enum Reasons
	{
		QUIT = gmsec::api5::util::Condition::USER,
		MANAGED,
		REQUEST,
		RECEIVE_REPLY,
		DELIVER_REPLY
	};

	RequestShared(InternalConnection* connection);
	~RequestShared();

	void run();

	bool add(gmsec::api5::util::StdSharedPtr<PendingRequest>& pending);

	void onReply(Message* reply);

	gmsec::api5::util::Condition& getCondition()
	{
		return m_condition;
	}

	void shutdown();

private:
	// Declared, but not implemented.
	RequestShared(const RequestShared&);
	RequestShared& operator=(const RequestShared&);

	void sendRequests();
	int waitLimit_ms();
	void deliverReplies();
	PendingRequest* findPending(const std::string& id);
	void deliverReply(Message* reply);


	typedef std::list<gmsec::api5::util::StdSharedPtr<PendingRequest> > PendingRequests;

	InternalConnection*          m_connection;
	gmsec::api5::util::Condition m_condition;
	PendingRequests              m_pending;
	std::list<Message*>          m_replies;
};


/**	@function runRequestThread
 *
 *	@brief This is body of the request thread.
 */
void runRequestThread(gmsec::api5::util::StdSharedPtr<RequestShared> shared);


} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
