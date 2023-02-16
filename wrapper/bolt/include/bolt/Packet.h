/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/*
 * @file Packet.h
 * bolt uses Packets internally to talk to the server:
 *	CLIENT <-> Message <-> BOLT <-> Packet <-> SERVER
 */

#ifndef bolt_Packet_h
#define bolt_Packet_h

#include <bolt/Header.h>
#include <bolt/Meta.h>

#include <gmsec5/util/StdUniquePtr.h>


namespace bolt {

class Packet
{
public:
	Packet(PacketType type);
	Packet(Header * header);
	virtual ~Packet();

	void setMeta(const Meta &meta);
	void setBody(const ByteBuffer &body);

	const Header *getHeader () const;
	const Meta *getMeta () const;
	const ByteBuffer *getBody () const;

	Header *getHeader ();
	Meta *getMeta ();
	ByteBuffer *getBody ();

	bool put(ByteBuffer &buffer) const;
	bool getContent(const ByteBuffer &buffer);

	std::ostream &put(std::ostream &os) const;


private:

	gmsec::api5::util::StdUniquePtr<Header> header;
	gmsec::api5::util::StdUniquePtr<Meta> meta;
	gmsec::api5::util::StdUniquePtr<ByteBuffer> body;
};

} // namespace bolt


#endif /* bolt_Packet_h */
