/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "TCPSocketClientArray.h"
#include "MBResourceData.h"
#include "MBWire.h"

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Message.h>

#include <gmsec4/util/Log.h>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include <sstream>

using namespace gmsec::api;
using namespace gmsec::api::util;

using namespace gmsec_messagebus;


#define STUPID_HUMAN_TRICK 12


TCPSocketClientArray::TCPSocketClientArray()
	:
	m_nsocks(0),
	m_sock(NULL),
	m_shouldBeConnected(false),
	m_maxMsgLife_s(60),	// 1 minute
	currentServer(""),
	m_isDebug(false)
{
}


TCPSocketClientArray::~TCPSocketClientArray()
{
	disconnect();
}


void TCPSocketClientArray::setDebug(bool isDebug)
{
	m_isDebug = isDebug;

	if (m_sock == NULL) return;

	for (int i = 0; i < m_nsocks; ++i)
	{
		m_sock[i]->setDebug(isDebug);
	}
}


Status TCPSocketClientArray::connect(int port)
{
	Status status;
	status = connect(port, "localhost");
	return status;
}


Status TCPSocketClientArray::connect(int port, const char* servers)
{
	{
		::AutoMutex hold(m_config_data_mutex);
		m_config_data.clear();
	}
	std::string servers_cp = servers;

	m_nsocks = 0;
	std::vector<GMSEC_I32> ports;

	// Expecting the server string to be something like:
	//      hostnameA:9000, hostnameB:9001
	// or   hostnameA, hostnameB
	std::vector<std::string> serversSplit = StringUtil::split(servers_cp, ',');

	for (size_t i = 0; i < serversSplit.size(); ++i)
	{
		if (serversSplit[i].empty())
		{
			continue;
		}

		std::vector<std::string> serverAndPort = StringUtil::split(serversSplit[i], ':');

		std::string serverName = StringUtil::trim(serverAndPort[0]);

		if (serverName.empty())
		{
			continue;
		}

		serverNames.push_back(serverName);

		++m_nsocks;

		if (serverAndPort.size() > 1)
		{
			std::string portString = StringUtil::trim(serverAndPort[1]);

			if (!portString.empty())
			{
				GMSEC_I32 gport = (GMSEC_I32) port;
				StringUtil::stringParseI32(portString.c_str(), gport);
				ports.push_back(gport);
				continue;
			}
		}

		ports.push_back((GMSEC_I32) port);
	}


// For HP-UX and Solaris there can not be a redundant MBServer at this time
#if defined(__hpux) || defined(__sun)
	if (m_nsocks > 1)
	{
		Status eresult;
		eresult.set(CONNECTION_ERROR, INVALID_CONNECTION , "Redundant servers not supported");
		return eresult;
	}
#endif

	m_sock = new TCPSocketClientReconnector* [m_nsocks];

	for (int i = 0; i < m_nsocks; ++i)
	{
		m_sock[i] = new TCPSocketClientReconnector(m_nsocks, &m_config_data, &m_config_data_mutex);
	}

	int failures = 0;
	Status result;

	std::ostringstream endpoints;

	for (int i = 0; i < m_nsocks; ++i)
	{
		result = m_sock[i]->connect(ports[i], serverNames[i].c_str());
		m_sock[i]->setDebug(m_isDebug);
		if (result.isError())
		{
			++failures;
		}
		else
		{
			endpoints << (endpoints.str().empty() ? "" : ",") << serverNames[i];
		}
	}

	m_shouldBeConnected.set(true);

	if (failures < m_nsocks)
	{
		currentServer = endpoints.str();

		Status good_result;
		return good_result;
	}
	else
	{
		disconnect();
		return result;
	}
}


Status TCPSocketClientArray::read(char*& buffer, int& len)
{
	Status result;

	AutoMutex hold(m_read_mutex);

	for (;;)
	{
		fd_set err_fds;
		fd_set in_fds;
		FD_ZERO(&err_fds);
		FD_ZERO(&in_fds);

		bool has_highest_socket = false;
		TCPSocket::Socket highest_socket = (TCPSocket::Socket) 0;

		for (int i = 0; i < m_nsocks; i++)
		{
			if (m_sock[i]->isConnected())
			{
				FD_SET(m_sock[i]->m_sock, &err_fds);
				FD_SET(m_sock[i]->m_sock, &in_fds);

				if (!has_highest_socket || (m_sock[i]->m_sock > highest_socket))
				{
					highest_socket = m_sock[i]->m_sock;
					has_highest_socket = true;
				}
			}
		}

		if (!has_highest_socket)
		{
			result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost [1]");
			return result;
		}

		struct timeval timeout = {0, 100000};
		int ret = select(highest_socket+1, &in_fds, NULL, &err_fds, &timeout);

		if (ret <= 0)
		{
			if (!m_shouldBeConnected.get())
			{
				result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost [2]");
				return result;
			}
			continue;
		}

		int isock;

		for (isock = 0; isock < m_nsocks; ++isock)
		{
			if (FD_ISSET(m_sock[isock]->m_sock, &err_fds))
			{
				break;
			}
		}

		if (isock != m_nsocks)
		{
			if (!m_shouldBeConnected.get())
			{
				result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost [3]");
				return result;
			}
			m_sock[isock]->reconnect();
			continue;
		}

		for (isock = 0; isock < m_nsocks; ++isock)
		{
			if (FD_ISSET(m_sock[isock]->m_sock, &in_fds))
			{
				break;
			}
		}

		result = m_sock[isock]->read(buffer, len);

		if (buffer == NULL || len == 0)
		{
			if (!m_shouldBeConnected.get())
			{
				result.set(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the server was lost [4]");
				return result;
			}
			m_sock[isock]->reconnect();
			continue;
		}

		if (result.getClass() != NO_ERROR_CLASS)
		{
			if (!m_shouldBeConnected.get())
			{
				return result;
			}
			m_sock[isock]->reconnect();
			continue;
		}

		// Check to see if the message read is a MB Resource message.  If it
		// is, just return, don't bother adding the unique_id to the unique_id
		// list which blocks redundant messages since EVERY Resource message
		// needs to get through.
		//
		char* strBuffer = new char[len + 1];
		StringUtil::copyBytes(strBuffer, buffer, len);
		strBuffer[len] = '\0';

		std::string tmp(strBuffer);

		delete [] strBuffer;

		if (tmp.find(MB_RESOURCE_DATA_SUBJECT, 0) != std::string::npos)
		{
			GMSEC_DEBUG << "Found GMSEC.MSG.C2CX.MB.RSRC";
			return result;
		}

		/* This is quite ugly.  There are messages which
		 * have unique IDs and those which do not.  This assumes that
		 * messages with length < 12 do not have unique IDs
		 */
		if (len < STUPID_HUMAN_TRICK)
		{
			GMSEC_DEBUG << "TSCA: not checking unique ID of short message length=" << len;
			return result;
		}

		len = checkDuplicate(buffer, len);
		if (len < 0)
		{
			GMSEC_VERBOSE << "TSCA: checkDuplicate<0 => deleting " << (void*) buffer;

			// toast the duplicate and try again
			delete [] buffer;

			buffer = NULL;
		}
		else
		{
			return result;
		}
	}

	return result;
}



int TCPSocketClientArray::checkDuplicate(const char* buffer, int len)
{
	if (len < STUPID_HUMAN_TRICK)
	{
		GMSEC_WARNING << "CTCPSocketClientArray::checkDuplicate: logic error";
		return len;
	}

	/* If there is only 1 server, there is no possibility of receiving
	 * duplicates, but we still need to remove the unique id.
	 */
	if (m_nsocks < 2)
	{
		return len;
	}

	Message* msg = NULL;
	Config   msgConfig;
	MBWire::deserialize(buffer, len, msg, msgConfig);

	if (msg == NULL)
	{
		return -1;
	}

	std::string unique_id;
	try
	{
		unique_id = msg->getStringField("UNIQUE-ID").getValue();
	}
	catch (...)
	{
		GMSEC_DEBUG << "Message does not have UNIQUE-ID";
	}
	delete msg;

	if (unique_id.empty())
	{
		return -1;
	}

	// Look for the unique_id in the m_msgs_set and set the iterator to it.
	//
	MsgsSetInter mmi = m_msgs_set.find(unique_id);

	// If the iterator did not hit the end of the set, then it was found,
	// so don't add it, delete it.
	//
	if (mmi != m_msgs_set.end())
	{
		len = -1;
	}
	else
	{
		// Create a structure of 'unique_id' and 'when' it was received.
		TTMsg ttmsg;
		ttmsg.when = time(NULL);
		ttmsg.msg = unique_id;

		// Insert the unique_id into the unique_id set.
		m_msgs_set.insert(unique_id);

		// Store the 'unique_id' and 'when' it was received.
		m_prev_msg.push_back(ttmsg);
	}

	time_t now = time(NULL);

	// Iterate over the unique_id list to see if their time has expired.
	//
	for (TTMsgsIter mi = m_prev_msg.begin(); mi != m_prev_msg.end(); ++mi)
	{
		// If the time has expired.
		if (now - mi->when >= m_maxMsgLife_s)
		{
			// Find the unique_id in the unique_id set.
			MsgsSetInter mmi2 = m_msgs_set.find(mi->msg);

			// If the unique_id is found in the set, erase it.
			if (mmi2 != m_msgs_set.end())
			{
				m_msgs_set.erase(mmi2);
			}

			// Erase the structure from the list.
			mi = m_prev_msg.erase(mi);

			// after erasing, it's possible that the ++mi in the loop statement
			// will dereference an invalid iterator
			if (mi == m_prev_msg.end())
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return len;
}



Status TCPSocketClientArray::write(const char *buffer, int len, const char *unique_id)
{
	AutoMutex hold(m_write_mutex);

	const char* buf2;
	int         len2;

	if (unique_id == NULL)
	{
		buf2 = buffer;
		len2 = len;
	}
	else
	{
		int uidlen = StringUtil::stringLength(unique_id);
		len2 = len + 1 + uidlen + 1;
		char *tmp = new char[len2];
		StringUtil::copyBytes(tmp, buffer, len);
		tmp[len] = '\0';
		StringUtil::copyBytes(tmp + len + 1, unique_id, uidlen);
		tmp[len2 - 1] = '\0';
		buf2 = tmp;
	}
	currentServer = "";

	bool is_good_result = false;
	Status result;
	// if (unique_id != NULL) printf("Wrote unique_id = '%s'\n", buf2 + len + 1);
	for (int i = 0; i < m_nsocks; i++)
	{
		if (!m_sock[i]->isConnected())
		{
			result = Status(CONNECTION_ERROR, CONNECTION_LOST, "Connection to the broker was lost");
			is_good_result = false;
			continue;
		}

		result = m_sock[i]->write(buf2, len2);

		if (result.isError())
		{
			m_sock[i]->reconnect();
		}
		else
		{
			if(currentServer.length() == 0)
			{
				currentServer.append(serverNames[i].c_str());
			}
			else
			{
				currentServer.append(",").append(serverNames[i].c_str());
			}
		}

		is_good_result = (result.getClass() == NO_ERROR_CLASS);
	}

	if (unique_id != NULL)
	{
		delete [] buf2;
	}

	if (is_good_result)
	{
		Status good_result;
		return good_result;
	}

	return result;
}


Status TCPSocketClientArray::disconnect()
{
	Status result;
	bool   is_good_result = true;

	m_shouldBeConnected.set(false);

	{
		AutoMutex hold(m_config_data_mutex);
		m_config_data.clear();
	}

	for (int i = 0; i < m_nsocks; ++i)
	{
		if (m_sock[i]->isConnected())
		{
			is_good_result = false;

			result = m_sock[i]->disconnect();

			is_good_result |= (result.getClass() == NO_ERROR_CLASS);

			if (result.getClass() == NO_ERROR_CLASS)
			{
				currentServer = "";
			}
		}
	}

	{
		AutoMutex holdRead(m_read_mutex);
		AutoMutex holdWrite(m_write_mutex);

		for (int i = 0; i < m_nsocks; ++i)
		{
			delete m_sock[i];
			m_sock[i] = NULL;
		}

		delete [] m_sock;
		m_sock = NULL;

		m_nsocks = 0;
	}

	if (is_good_result)
	{
		Status good_result;
		return good_result;
	}

	return result;
}


void TCPSocketClientArray::addConfig(const char* config_data)
{
	AutoMutex hold(m_config_data_mutex);

	m_config_data[std::string(config_data)] = 1;
}


void TCPSocketClientArray::removeConfig(const char* config_data)
{
	AutoMutex hold(m_config_data_mutex);

	m_config_data.erase(std::string(config_data));
}


void TCPSocketClientArray::setConfig(const Config &config)
{
	const char* value = config.getValue("GMSEC-MB-MAX_MSG_LIFE.s");

	if (value)
	{
		GMSEC_I32 tmp = 0;

		if (StringUtil::stringParseI32(value, tmp))
		{
			m_maxMsgLife_s = tmp;
		}
	}

	GMSEC_DEBUG << "MB:TSCA: using maxMsgLife_s=" << m_maxMsgLife_s;
}

