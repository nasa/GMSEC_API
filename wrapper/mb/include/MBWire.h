/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef MB_WIRE_H
#define MB_WIRE_H

namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Message;
	class MessageFactory;
}
}

#include <gmsec5_defs.h>


// Message Bus message types
#define CMD_OK       'O'
#define CMD_SUB      'S'
#define CMD_DISC     'D'
#define CMD_PUB      'P'
#define CMD_USUB     'U'
#define CMD_NLOOP    'N'

#define CMD_ACKSUB   'A'
#define CMD_ACKCONN  'G'

#define CMD_EXIT     'X'


namespace gmsec_messagebus
{

/** @brief This file contains declarations for the Message Bus wire protocol.
*/
class MBWire
{
public:
	/**
	 * @fn serialize(const gmsec::api5::Message& message, char*& data, GMSEC_I32& size)
	 * serializes the message into a binary format returning ownership of the <size> bytes at <data>
	 */
	static void serialize(const gmsec::api5::Message& message, char*& data, GMSEC_I32& size);


	/**
	 * @fn deserialize(const char* data, int size, gmsec::api5::Message*& message)
	 * populates the message using the format generated by Serialize()
	 */
	static gmsec::api5::Message* deserialize(const char* data, GMSEC_I32 size, gmsec::api5::MessageFactory& msgFactory, const gmsec::api5::Config& msgConfig);


	static void setCompress(bool isCompress);


	static bool isCompress();


private:
	static bool m_isCompress;
};

}  // end namespace gmsec_messagebus

#endif
