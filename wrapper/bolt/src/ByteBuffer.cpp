
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/ByteBuffer.h>

#include <iomanip>


namespace bolt {


ByteBuffer::ByteBuffer()
	:
	_owned(false), _buffer(0), _size(0)
{
}


ByteBuffer::ByteBuffer(const ByteBuffer &in)
	:
	_owned(false), _buffer(0), _size(0)
{
	copy(in._buffer, in._size);
}


ByteBuffer::ByteBuffer(u8 *p, i32 count, bool ownership)
	:
	_owned(false), _buffer(0), _size(0)
{
	set(p, count, ownership);
}


ByteBuffer::~ByteBuffer()
{
	clear();
}


void ByteBuffer::set(u8 *p, i32 count, bool ownership)
{
	if (_owned)
		if (p != _buffer)
			delete[] _buffer;
	_buffer = p;
	_size = count;
	_owned = ownership;
}


void ByteBuffer::copy(const u8 *p, i32 count)
{
	u8 *q = new u8[count];
	for (i32 i = 0; i < count; ++i)
		q[i] = p[i];
	set(q, count, true);
}


void ByteBuffer::resize(i32 count)
{
	set(new u8[count], count, true);
}


void ByteBuffer::clear()
{
	if (_owned)
		delete[] _buffer;
	_buffer = 0;
	_size = 0;
	_owned = false;
}


std::ostream &ByteBuffer::put(std::ostream &os, const char *prefix) const
{
using namespace std;
	const u8 * raw = (u8*) _buffer;

	ios::fmtflags oflags = os.flags();
	streamsize owidth = os.width();
	char ofill = os.fill('0');

	os << prefix << hex;

#define BB_LIMIT 1024
	i32 end = _size < BB_LIMIT ? _size : BB_LIMIT;
	for (i32 i = 0; i < end; ++i)
	{
		os << setw(2) << int(raw[i]) << ' ';
		if (i % 16 == 15)
			os << '\n' << prefix;
	}

	if (_size >= BB_LIMIT)
		os << "...";

	os.flags(oflags);
	os.width(owidth);
	os.fill(ofill);

	os << '\n';

	return os;
}


} // namespace bolt

