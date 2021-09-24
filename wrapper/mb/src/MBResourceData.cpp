/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "MBResourceData.h"


using namespace gmsec::api::util;

using namespace gmsec_messagebus;


MBResourceData::MBResourceData()
	:
	m_bytes_rcvd(0),
	m_bytes_sent(0),
	m_msgs_rcvd(0),
	m_msgs_sent(0)
{
}


MBResourceData::~MBResourceData()
{
}


void MBResourceData::setBytesRcvd(GMSEC_U32 bytes)
{
	AutoMutex hold(m_bytes_rcvd_mutex);
	m_bytes_rcvd += bytes;
}


void MBResourceData::setBytesSent(GMSEC_U32 bytes)
{
	AutoMutex hold(m_bytes_sent_mutex);
	m_bytes_sent += bytes;
}


void MBResourceData::setMsgsRcvd(GMSEC_U32 msgs)
{
	AutoMutex hold(m_bytes_rcvd_mutex);
	m_msgs_rcvd += msgs;
}


void MBResourceData::setMsgsSent(GMSEC_U32 msgs)
{
	AutoMutex hold(m_bytes_sent_mutex);
	m_msgs_sent += msgs;
}


GMSEC_U32 MBResourceData::getBytesRcvd()
{
	AutoMutex hold(m_bytes_rcvd_mutex);
	return m_bytes_rcvd;
}


GMSEC_U32 MBResourceData::getBytesSent()
{
	AutoMutex hold(m_bytes_sent_mutex);
	return m_bytes_sent;
}


GMSEC_U32 MBResourceData::getMsgsRcvd()
{
	AutoMutex hold(m_bytes_rcvd_mutex);
	return m_msgs_rcvd;
}


GMSEC_U32 MBResourceData::getMsgsSent()
{
	AutoMutex hold(m_bytes_sent_mutex);
	return m_msgs_sent;
}
