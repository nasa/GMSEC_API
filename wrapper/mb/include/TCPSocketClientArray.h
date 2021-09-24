/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_TCP_SOCKET_CLIENT_ARRAY_H
#define MB_TCP_SOCKET_CLIENT_ARRAY_H

#include "TCPSocketClientReconnector.h"

#include <gmsec4_defs.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Atomics.h>
#include <gmsec4/util/Mutex.h>

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

	gmsec::api::util::Mutex                 m_read_mutex;
	gmsec::api::util::Mutex                 m_write_mutex;
	int                                     m_nsocks;
	TCPSocketClientReconnector**            m_sock;
	std::vector<std::string>                serverNames;
	MsgsSet                                 m_msgs_set;

	TTMsgs                                  m_prev_msg;
	gmsec::api::util::Mutex                 m_config_data_mutex;
	TCPSocketClientReconnector::ConfigData  m_config_data;
	gmsec::api::util::AtomicBoolean         m_shouldBeConnected;
	GMSEC_I32                               m_maxMsgLife_s;
	std::string                             currentServer;

protected:
	bool m_isDebug;

public:
	TCPSocketClientArray();

	~TCPSocketClientArray();

	void setDebug(bool);

	gmsec::api::Status connect(int port);

	gmsec::api::Status connect(int port, const char* servers);

	gmsec::api::Status read(char*& buffer, int& len);

	gmsec::api::Status write(const char* buffer, int len, const char* unique_id = NULL);

	gmsec::api::Status disconnect();

	void addConfig(const char* config_data);

	void removeConfig(const char* config_data);

	void setConfig(const gmsec::api::Config& config);

	std::string getCurrentServer()
	{
		return currentServer;
	}
};

}  // end namespace gmsec_messagebus

#endif
