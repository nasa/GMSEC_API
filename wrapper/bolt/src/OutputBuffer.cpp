
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Buffers.h>
#include <bolt/Encoder.h>
#include <bolt/Exception.h>

using namespace std;


namespace bolt {


OutputBuffer::OutputBuffer(ByteBuffer &buffer)
	:
	encoder(Encoder::get()),
	count(buffer.size()),
	start(buffer.raw()),
	current(buffer.raw())
{
}


OutputBuffer::OutputBuffer(u8 *p, i32 count)
	:
	encoder(Encoder::get()),
	count(count),
	start(p),
	current(p)
{
}


OutputBuffer::~OutputBuffer()
{
}


bool OutputBuffer::put1(i32 i)
{
// cout << "put1:" << i << endl;
	if (available() < 1)
		throw IOException("OutputBuffer::put1: insufficient space");

	encoder.put1(i, current);
	current += 1;
	return true;
}


bool OutputBuffer::put2(i32 i)
{
// cout << "put2:" << i << endl;
	if (available() < 2)
		throw IOException("OutputBuffer::put2: insufficient space");

	encoder.put2(i, current);
	current += 2;
	return true;
}


bool OutputBuffer::put3(i32 i)
{
// cout << "put3:" << i << endl;
	if (available() < 3)
		throw IOException("OutputBuffer::put3: insufficient space");

	encoder.put3(i, current);
	current += 3;
	return true;
}


bool OutputBuffer::put4(i32 i)
{
// cout << "put4:" << i << endl;
	if (available() < 4)
		throw IOException("OutputBuffer::put4: insufficient space");

	encoder.put4(i, current);
	current += 4;
	return true;
}


bool OutputBuffer::put(const u8 *p, int localCount)
{
// cout << "put:" << p << " [" << localCount << ']' << endl;
	if (available() < localCount)
		throw IOException("OutputBuffer::put(u8*): insufficient space");

	encoder.put(p, localCount, current);
	current += localCount;
	return true;
}


bool OutputBuffer::putr8(f64 x)
{
// cout << "putr8:" << x << endl;
	if (available() < 8)
		throw IOException("OutputBuffer::put(f64): insufficient space");

	encoder.putr8(x, current);
	current += 8;
	return true;
}


} // namespace bolt

