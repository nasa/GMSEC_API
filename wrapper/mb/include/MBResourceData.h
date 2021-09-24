/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_RESOURCE_DATA_H
#define MB_RESOURCE_DATA_H

#include <gmsec4_defs.h>

#include <gmsec4/util/Mutex.h>


#define MB_RESOURCE_DATA_SUBJECT           "GMSEC.MSG.C2CX.MB.RSRC"


namespace gmsec_messagebus
{

/**
 * @brief This is a container class for the MBServer resource data.
 */
class MBResourceData
{
public:
	MBResourceData();

	~MBResourceData();

	// Set functions for data elements.
	void setBytesRcvd(GMSEC_U32 bytes);
	void setBytesSent(GMSEC_U32 bytes);
	void setMsgsRcvd(GMSEC_U32 msgs);
	void setMsgsSent(GMSEC_U32 msgs);

	// Get functions for data elements.
	GMSEC_U32 getBytesRcvd();
	GMSEC_U32 getBytesSent();
	GMSEC_U32 getMsgsRcvd();
	GMSEC_U32 getMsgsSent();


private:
	// Mutex's for accessing the various data elements.
	gmsec::api::util::Mutex m_bytes_rcvd_mutex;
	gmsec::api::util::Mutex m_bytes_sent_mutex;
	gmsec::api::util::Mutex m_msgs_rcvd_mutex;
	gmsec::api::util::Mutex m_msgs_sent_mutex;

	GMSEC_U32               m_bytes_rcvd;	// Total bytes received by MBServer
	GMSEC_U32               m_bytes_sent;	// Total bytes sent by MBServer
	GMSEC_U32               m_msgs_rcvd;	// Total messages received by MBServer
	GMSEC_U32               m_msgs_sent;	// Total messages sent by MBServer
};

}  // end namespace gmsec_messagebus

#endif
