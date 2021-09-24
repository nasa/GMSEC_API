
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Shared_h
#define bolt_Shared_h

#include <bolt/Test.h>
#include <bolt/TCPSocket.h>

#include <list>


namespace bolt {

class Connection;
class Packet;

typedef counted_ptr<Packet> SharedPacket;

class PacketHandler
{
public:
	virtual ~PacketHandler() { }
	virtual void onPacket(SharedPacket packet) = 0;
};


class Shared
{
public:

	Shared(Connection *connection, const Options &opt);
	~Shared();

	string tag(const char *place) const;

	Options getOptions() const;

	void setState(State state, bool notifyListeners = false);
	State getState();

	// runManager manages the Condition
	Condition &getCondition();

	Result connect();
	Result disconnect();
	Result shutdown();

	Result put(const SharedPacket &packet);

	int write(const u8 *raw, int count);
	int read(u8 *dest, int count);

	Result getInput(SharedPacket &packet, int timeout_ms);

	Code getInput(SharedPacket &packet);

	// bool received(const ByteBuffer &buffer);

	void handle(const SharedPacket &packet);

	// wait for Test to be true (or timeout)
	Result await(int timeout_ms, Test &t);

	Code isAcknowledged(const string &id);

	void setDefaultHandler(PacketHandler *handler);
	void setReplyHandler(PacketHandler *handler);


private:

	State _getState() {
		return (State) fState;
	}

	Mutex &getMutex() {
		return fMutex;
	}


public:
	ConnectState getMode() {
		return fMode;
	}

	Result ioError(const string &s);
	void resubscribe();
	void ping();
	i32 getIoCounter(i32 &traps);

private:

	void setMode(ConnectState m);
	void welcome(const SharedPacket &in);
	void negotiate(const SharedPacket &in);

	Result write(const ByteBuffer &buffer);


	Options fOptions;
	volatile i32 fState;
	volatile i32 fIoCounter;
	volatile i32 fTraps;
	ConnectState fMode;
	Connection *fConnection;
	TCPSocket fSocket;
	Mutex fMutex;
	Condition fCondition;

	PacketHandler *fDefaultHandler;
	PacketHandler *fReplyHandler;

	std::list<SharedPacket> qIncoming;
	std::list<string> qAcks;

	string fTag;
	string fReplyTo;
	bool fReplyExpose;
	bool fWarnAlways;
	i32 fNullRead_ms;
	i32 fNullReadLog_ms;
};


void runManager(counted_ptr<Shared> shared);
void runReader(counted_ptr<Shared> shared);


} // namespace bolt


#endif /* bolt_Shared_h */

