/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef bolt_Connection_h
#define bolt_Connection_h

#include <bolt/Packet.h>
#include <bolt/Shared.h>

#include <gmsec4/util/StdThread.h>

#include <list>


namespace bolt {


using gmsec::api::util::StdThread;

typedef counted_ptr<Packet> SharedPacket;

class EventListener;


class Connection
{
public:

	Connection(Server server, const Options &options);
	~Connection();

	Server getServer() const;

	Result connect();
	Result disconnect();

	Result subscribe(const string &pattern);
	Result unsubscribe(const string &pattern);
	Result put(SharedPacket packet);
	Result get(SharedPacket &packet, int timeout_ms);


	bool resubscribe();

	void setDefaultHandler(PacketHandler *handler);
	void setReplyHandler(PacketHandler *handler);

	bool isSubscribed(const string &topic);

	void registerEventListener(EventListener* listener);

	void notifyEventListeners(State state);

private:

	Connection(const Connection &); // = delete;
	Connection &operator=(const Connection &); // = delete;

	string generateID();
	Result _subscribe(const std::list<string> &patterns, bool await);
	Result ping();


	Options options;
	Server server;
	counted_ptr<Shared> shared;
	StdThread manager;
	StdThread reader;

	std::list<string> subscriptions;
	long long counter;
	string fTag;
	string replyTopic;

	std::list<EventListener*> listeners;
	State lastState;
};


} // namespace bolt


#endif /* bolt_Connection_h */

