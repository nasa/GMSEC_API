/*
 * Copyright 2007-2025 United States Government as represented by the
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

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/UniqueFilter.h>

#include <gmsec5/util/StdUniquePtr.h>

#include <list>


namespace gmsec_bolt {


struct LogInfo
{
	bool mwGet;
};

class GMSEC_MW_API BoltConnection : public gmsec::api5::internal::ConnectionInterface,
                                    public bolt::EventListener
{
public:
	BoltConnection(const gmsec::api5::Config& config);

	virtual ~BoltConnection();

	virtual void setServers(const std::list<bolt::Server> &servers) = 0;

	virtual const char* getLibraryRootName();
	virtual const char* getLibraryVersion();

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

	void makePacket(const gmsec::api5::Message& message, bolt::SharedPacket& packet, bolt::PacketType type, const std::string* preID = 0);

	void fromPacket(bolt::SharedPacket& packet, gmsec::api5::Message*& message);

	void mwPublishAux(const gmsec::api5::Message& message, bolt::SharedPacket& packet);

	void mwRequestAux(gmsec::api5::Message& message, bolt::SharedPacket& packet, const std::string& uniqueID);


	bolt::PacketHandler* myDefaultHandler;
	bolt::PacketHandler* myReplyHandler;
	std::string          myReplyTo;
	std::string          myTag;
	LogInfo              logInfo;
};


class GMSEC_MW_API SingleConnection : public BoltConnection
{
public:
	SingleConnection(const gmsec::api5::Config& config);

	virtual ~SingleConnection();

	virtual void setServers(const std::list<bolt::Server>& servers);

	virtual const char* getMWInfo();

	virtual void mwConnect();

	virtual void mwDisconnect();

	virtual void mwSubscribe(const char* subject, const gmsec::api5::Config& config);

	virtual void mwUnsubscribe(const char* subject);

	virtual void mwPublish(const gmsec::api5::Message& message, const gmsec::api5::Config& config);

	virtual void mwRequest(const gmsec::api5::Message& message, const std::string& uniqueID);

	virtual void mwReceive(gmsec::api5::Message*& message, GMSEC_I32 timeout);

	virtual std::string mwGetUniqueID();

private:

	bolt::Server server;
	gmsec::api5::util::StdUniquePtr<bolt::Connection> connection;

	std::string mwInfo;
};



class GMSEC_MW_API MultipleConnection : public BoltConnection
{
public:
	MultipleConnection(const gmsec::api5::Config& config);

	virtual ~MultipleConnection();

	virtual void setServers(const std::list<bolt::Server>& servers);

	virtual const char* getMWInfo();

	virtual void mwConnect();

	virtual void mwDisconnect();

	virtual void mwSubscribe(const char* subject, const gmsec::api5::Config& config);

	virtual void mwUnsubscribe(const char* subject);

	virtual void mwPublish(const gmsec::api5::Message& message, const gmsec::api5::Config& config);

	virtual void mwRequest(const gmsec::api5::Message& message, const std::string& uniqueID);

	virtual void mwReceive(gmsec::api5::Message*& message, GMSEC_I32 timeout);

	virtual std::string mwGetUniqueID();

private:

	virtual void handlePacket(bolt::SharedPacket &packet);

	typedef std::list<bolt::Server>      Servers;
	typedef std::list<bolt::Connection*> Connections;

	typedef Servers::iterator            ServerIt;
	typedef Connections::iterator        ConnIt;

	Servers     servers;
	Connections connections;

	std::list<bolt::SharedPacket> incoming;

	gmsec::api5::util::Condition condvar;

	gmsec::api5::internal::UniqueFilter filter;

	std::string mwInfo;
};


} // namespace gmsec_bolt


#endif /* gmsec_BoltConnection_h */
