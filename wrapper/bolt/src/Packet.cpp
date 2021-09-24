
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Packet.h>
#include <bolt/Log.h>
#include <bolt/Exception.h>


using std::ostream;


namespace bolt {


Packet::Packet(Header *h)
	:
	header(h)
{
}


Packet::Packet(PacketType t)
	:
	header(new Header(t))
{
}


Packet::~Packet()
{
}


bool Packet::getContent(const ByteBuffer &buffer)
{
	bool ok = false;

	InputBuffer in(buffer);

	try
	{
		ok = true;

		if (header->hasMeta())
		{
			meta.reset(new Meta());
			ok = meta->parse(in);
		}

		if (header->hasBody())
		{
			body.reset(new ByteBuffer());
			body->copy(in.at(), in.available());
		}
	}
	catch (Exception &e)
	{
		GMSEC_WARNING << "Packet.getContent: " << e.getText().c_str();
	}

	return ok;
}



bool Packet::put(ByteBuffer &buffer) const
{
	i32 size = 0;
	if (meta.get())
		size += meta->determineSize();

#if 0
i32 metaSize = size;
#endif

	if (body.get())
		size += body->size();

	buffer.resize(Header::SIZE+size);

	OutputBuffer out(buffer);

	{
		int flags = 0;
		if (meta.get())
			flags |= Header::F_META;
		if (body.get())
			flags |= Header::F_BODY;

		header->put(out, size, flags);
	}

	if (meta.get())
		meta->put(out);

	if (body.get())
		out.put(body->raw(), body->size());

#if 0
std::cout << "the encoded packet: metaSize=" << metaSize
		<< " bodySize=" << body->size()
		<< " total=" << buffer.size()
		<< std::endl;
buffer.put(std::cout);
#endif

	return true;
}



void Packet::setMeta(const Meta &in)
{
	meta.reset(new Meta(in));
}


void Packet::setBody(const ByteBuffer &in)
{
	body.reset(new ByteBuffer(in));
}


Header *Packet::getHeader()
{
	return header.get();
}


Meta *Packet::getMeta()
{
	return meta.get();
}


ByteBuffer *Packet::getBody()
{
	return body.get();
}


const Header *Packet::getHeader() const
{
	return header.get();
}


const Meta *Packet::getMeta() const
{
	return meta.get();
}


const ByteBuffer *Packet::getBody() const
{
	return body.get();
}


ostream &Packet::put(ostream &os) const
{
	os << "Packet[" << this << ']' << '\n';
	if (getHeader())
		getHeader()->put(os, "\t");
	if (getMeta())
		getMeta()->put(os, "\t");
	if (getBody())
	{
		os << "\tBody[size=" << getBody()->size() << ']' << '\n';
		getBody()->put(os, "\t");
	}
	os << "--- end Packet ---\n";
	return os;
}


} // namespace bolt

