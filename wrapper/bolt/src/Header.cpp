
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <cassert>

#include <bolt/Header.h>
#include <bolt/Buffers.h>
#include <bolt/Log.h>
#include <bolt/Exception.h>
#include <bolt/util.h>


using std::ostream;


namespace bolt {


Header::Header(PacketType t)
	:
	size(-1),
	type(t),
	flags(0)
{
}


Header::Header(const Header &other)
	:
	size(other.size),
	type(other.type),
	flags(other.flags)
{
}


Header::~Header()
{
}


Header& Header::operator=(const Header& other)
{
	if (this != &other)
	{
		size  = other.size;
		type  = other.type;
		flags = other.flags;
	}
	return *this;
}



class TypeValidater
{
public:
	TypeValidater() {
		PacketType known[] = {
			PACKET_WELCOME, PACKET_GOODBYE, PACKET_ECHO,
			PACKET_ERROR, PACKET_ACK,
			PACKET_NEGOTIATE, PACKET_STATISTICS,
			PACKET_SUBSCRIBE, PACKET_UNSUBSCRIBE,
			PACKET_PUBLISH, PACKET_REQUEST, PACKET_REPLY
		};

		for (int i = 0; i < PACKET_END; ++i)
			table[i] = PACKET_VOID;

		for (size_t i = 0; i < sizeof(known)/sizeof(known[0]); ++i) {
			int z = known[i];
			if (z < PACKET_END) {
				table[z] = z;
			}
			else {
				GMSEC_WARNING << "TypeValidater: unable to initialize table using index: " << z;
			}
		}
	}

	PacketType fix(int in) const {
		PacketType fixed = PACKET_VOID;
		if (in > 0 && in < PACKET_END)
			fixed = (PacketType) table[in];
		return fixed;
	}
private:
	int table[PACKET_END];
};


PacketType validateType(int type)
{
	static const TypeValidater validater;
	return validater.fix(type);
}



Header *Header::parse(const ByteBuffer &buffer0)
{
	assert(buffer0.size() == SIZE);

	try
	{
		Header header(PACKET_VOID);

		InputBuffer buffer(buffer0);

		header.parseAux(buffer);

		return new Header(header);
	}
	catch (IOException &e)
	{
		GMSEC_WARNING << "Header.parse: " << e.getText().c_str();
	}

	return 0;
}



bool Header::parseAux(InputBuffer &buffer)
{
	size = buffer.get4();
	if (size < 0)
		throw IOException("invalid size=" + util::toString(size));

	i32 tmp = buffer.get1();
	type = validateType(tmp);
	if (type == PACKET_VOID)
		throw IOException("invalid type=" + util::toString(tmp));

	tmp = buffer.get1();
	if (tmp != SYNC)
		throw IOException("invalid sync=" + util::toString(tmp));

	flags = buffer.get1();

	return true;
}



bool Header::put(OutputBuffer &buffer, i32 osize, int oflags) const
{
	assert(buffer.size() == osize + Header::SIZE);
	assert(buffer.available() == buffer.size());

	buffer.put4(osize);

	buffer.put1(type);
	buffer.put1(SYNC);
	buffer.put1(oflags);
	buffer.put1(0);

	return true;
}


ostream &Header::put(ostream &os, const char *prefix) const
{
	os << prefix << "Header[" << this
		<< "] type=" << util::toString(type)
		<< " size=" << size << " flags=" << flags
		<< '\n';
	return os;
}


} // namespace bolt

