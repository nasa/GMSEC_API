/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_MB_TCP_SOCKET_CLIENT_ARRAY_H
#define GMSEC_MB_TCP_SOCKET_CLIENT_ARRAY_H

#include "TCPSocketClientReconnector.h"

#include <gmsec5_defs.h>

#include <gmsec5/Config.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/Atomics.h>
#include <gmsec5/util/Mutex.h>

#include <time.h>
#include <list>
#include <set>
#include <string>
#include <vector>



namespace gmsec_messagebus
{

/** @brief This class manages the client incoming and outgoing messages coming
to and from the MBServer.
**/
class TCPSocketClientArray
{
private:
	int checkDuplicate(const char* buffer, int len);

	struct TTMsg
	{
		time_t when;
		std::string msg;
	};

	typedef std::set<std::string>  MsgsSet;
	typedef MsgsSet::iterator      MsgsSetInter;
	typedef std::list<TTMsg>       TTMsgs;
	typedef TTMsgs::iterator       TTMsgsIter;

	gmsec::api5::MessageFactory&             m_msgFactory;
	gmsec::api5::util::Mutex                 m_read_mutex;
	gmsec::api5::util::Mutex                 m_write_mutex;
	int                                      m_nsocks;
	TCPSocketClientReconnector**             m_sock;
	std::vector<std::string>                 serverNames;
	MsgsSet                                  m_msgs_set;

	TTMsgs                                   m_prev_msg;
	gmsec::api5::util::Mutex                 m_config_data_mutex;
	TCPSocketClientReconnector::ConfigData   m_config_data;
	gmsec::api5::util::AtomicBoolean         m_shouldBeConnected;
	GMSEC_I32                                m_maxMsgLife_s;
	std::string                              currentServer;

protected:
	bool m_isDebug;

public:
	TCPSocketClientArray(gmsec::api5::MessageFactory& msgFactory);

	~TCPSocketClientArray();

	void setDebug(bool);

	gmsec::api5::Status connect(const std::string& servers, int defaultPort);

	gmsec::api5::Status read(char*& buffer, GMSEC_I32& len);

	gmsec::api5::Status write(const char* buffer, GMSEC_I32 len, const char* unique_id = NULL);

	gmsec::api5::Status disconnect();

	void addConfig(const char* config_data);

	void removeConfig(const char* config_data);

	void setConfig(const gmsec::api5::Config& config);

	std::string getCurrentServer()
	{
		return currentServer;
	}
};

}  // end namespace gmsec_messagebus

#endif
