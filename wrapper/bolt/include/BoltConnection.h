/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/*
 * @file BoltConnection.h
 */

#ifndef gmsec_BoltConnection_h
#define gmsec_BoltConnection_h

#include <bolt/Connection.h>
#include <bolt/EventListener.h>
#include <bolt/Types.h>

#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/UniqueFilter.h>

#include <gmsec4/util/StdUniquePtr.h>

#include <list>


namespace gmsec_bolt {


struct LogInfo
{
	bool mwGet;
};

class GMSEC_MW_API BoltConnection : public gmsec::api::internal::ConnectionInterface,
                                    public bolt::EventListener
{
public:
	BoltConnection(const gmsec::api::Config& config);

	virtual ~BoltConnection();

	virtual void setServers(const std::list<bolt::Server> &servers) = 0;

	virtual const char* CALL_TYPE getLibraryRootName();
	virtual const char* CALL_TYPE getLibraryVersion();

	virtual void onEvent(bolt::State state);

private:

	friend class MyPacketHandler;

	virtual void handlePacket(bolt::SharedPacket &packet);
	void handleReply(bolt::SharedPacket &packet);

	GMSEC_U64 counter;

	std::string libVersion;

protected:

	void getOptions(bolt::Options &options);

	std::string generateID();

	bool updateReplyTo(const std::string& in);

	void makePacket(const gmsec::api::Message& message, bolt::SharedPacket& packet, bolt::PacketType type, std::string* preID = 0);

	void fromPacket(bolt::SharedPacket& packet, gmsec::api::Message*& message);

	void mwPublishAux(const gmsec::api::Message& message, bolt::SharedPacket& packet);

	void mwRequestAux(gmsec::api::Message& message, bolt::SharedPacket& packet, std::string& uniqueID);

	void mwReplyAux(gmsec::api::Message& request, gmsec::api::Message& reply, bolt::SharedPacket& packet);


	bolt::PacketHandler* myDefaultHandler;
	bolt::PacketHandler* myReplyHandler;
	std::string          myReplyTo;
	std::string          myTag;
	LogInfo              logInfo;
};


class GMSEC_MW_API SingleConnection : public BoltConnection
{
public:
	SingleConnection(const gmsec::api::Config& config);

	virtual ~SingleConnection();

	virtual void setServers(const std::list<bolt::Server>& servers);

	virtual const char* CALL_TYPE getMWInfo();

	virtual void CALL_TYPE mwConnect();

	virtual void CALL_TYPE mwDisconnect();

	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);

	virtual void CALL_TYPE mwUnsubscribe(const char* subject);

	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& message, const gmsec::api::Config& config);

	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& message, std::string& uniqueID);

	virtual void CALL_TYPE mwReply(const gmsec::api::Message& message, const gmsec::api::Message& reply);

	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& message, GMSEC_I32 timeout);

private:

	bolt::Server server;
	gmsec::api::util::StdUniquePtr<bolt::Connection> connection;

	std::string mwInfo;
};



class GMSEC_MW_API MultipleConnection : public BoltConnection
{
public:
	MultipleConnection(const gmsec::api::Config& config);

	virtual ~MultipleConnection();

	virtual void setServers(const std::list<bolt::Server>& servers);

	virtual const char* CALL_TYPE getMWInfo();

	virtual void CALL_TYPE mwConnect();

	virtual void CALL_TYPE mwDisconnect();

	virtual void CALL_TYPE mwSubscribe(const char* subject, const gmsec::api::Config& config);

	virtual void CALL_TYPE mwUnsubscribe(const char* subject);

	virtual void CALL_TYPE mwPublish(const gmsec::api::Message& message, const gmsec::api::Config& config);

	virtual void CALL_TYPE mwRequest(const gmsec::api::Message& message, std::string& uniqueID);

	virtual void CALL_TYPE mwReply(const gmsec::api::Message& message, const gmsec::api::Message& reply);

	virtual void CALL_TYPE mwReceive(gmsec::api::Message*& message, GMSEC_I32 timeout);

	virtual void handlePacket(bolt::SharedPacket &packet);

private:

	typedef std::list<bolt::Server>      Servers;
	typedef std::list<bolt::Connection*> Connections;

	typedef Servers::iterator            ServerIt;
	typedef Connections::iterator        ConnIt;

	Servers     servers;
	Connections connections;

	std::list<bolt::SharedPacket> incoming;

	gmsec::api::util::Condition condvar;

	gmsec::api::internal::UniqueFilter filter;

	std::string mwInfo;
};


} // namespace gmsec_bolt


#endif /* gmsec_BoltConnection_h */
