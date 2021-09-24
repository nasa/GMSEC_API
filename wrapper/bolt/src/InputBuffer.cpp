
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Buffers.h>
#include <bolt/Decoder.h>
#include <bolt/Exception.h>

namespace bolt {


InputBuffer::InputBuffer(const ByteBuffer &buffer)
	:
	decoder(Decoder::get()),
	count(buffer.size()),
	start(buffer.raw()),
	current(buffer.raw())
{
}


InputBuffer::InputBuffer(const u8 *raw, i32 count)
	:
	decoder(Decoder::get()),
	count(count),
	start(raw),
	current(raw)
{
}


InputBuffer::~InputBuffer()
{
}


i32 InputBuffer::get1()
{
	if (available() < 1)
		throw IOException("InputBuffer::get1: insufficient data");
	i32 tmp = decoder.get1(current);
	current += 1;
	return tmp;
}


i32 InputBuffer::get2()
{
	if (available() < 2)
		throw IOException("InputBuffer::get2: insufficient data");
	i32 tmp = decoder.get2(current);
	current += 2;
	return tmp;
}


i32 InputBuffer::get3()
{
	if (available() < 3)
		throw IOException("InputBuffer::get3: insufficient data");
	i32 tmp = decoder.get3(current);
	current += 3;
	return tmp;
}


i32 InputBuffer::get4()
{
	if (available() < 4)
		throw IOException("InputBuffer::get4: insufficient data");
	i32 tmp = decoder.get4(current);
	current += 4;
	return tmp;
}


bool InputBuffer::get(u8 *dest, int localCount)
{
	if (available() < localCount)
		throw IOException("InputBuffer::get(array): insufficient data");
	decoder.get(current, localCount, dest);
	current += localCount;
	return true;
}


f64 InputBuffer::getr8()
{
	if (available() < 8)
		throw IOException("InputBuffer::getr8: insufficient data");
	f64 tmp = decoder.getr8(current);
	current += 8;
	return tmp;
}


void InputBuffer::advance(i32 n)
{
	if (available() < n)
		throw IOException("InputBuffer::advance: insufficient data");
	current += n;
}


} // namespace bolt

