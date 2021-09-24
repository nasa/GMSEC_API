
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <gmsec/internal/mathutil.h>
#include <gmsec/internal/strutil.h>

#include <cassert>
#include <sstream>

using std::size_t;

/**
 * \file mathutil.cpp
 * \brief Platform independent(?) support for floating point constants.
 *
 * There is an assumption that the platform uses IEEE 754.
 *
 * See http://en.wikipedia.org/wiki/Single_precision and Double_precision
 * for representation of infinity, NaN.
 */


namespace gmsec
{
namespace util
{


/**
* \fn isLittleEndian ()
* Determines if this is a little endian platform.
*/
static bool isLittleEndian()
{
	int x = 1;
	char *p = reinterpret_cast<char*>(&x);
	return p[0] == 1;
}


static void setBit(GMSEC_U8 *p, int bit)
{
	int byte = bit / 8;
	int subbit = bit % 8;
	p[byte] |= (0x80 >> subbit);
}


static void storeFloat(float *f, const GMSEC_U8 *p)
{
	char *q = reinterpret_cast<char*>(f);

	if (isLittleEndian())
	{
		q[0] = p[3];
		q[1] = p[2];
		q[2] = p[1];
		q[3] = p[0];
	}
	else
	{
		q[0] = p[0];
		q[1] = p[1];
		q[2] = p[2];
		q[3] = p[3];
	}
}


static void storeDouble(double *d, const GMSEC_U8 *p)
{
	char *q = reinterpret_cast<char*>(d);

	if (isLittleEndian())
	{
		q[0] = p[7];
		q[1] = p[6];
		q[2] = p[5];
		q[3] = p[4];
		q[4] = p[3];
		q[5] = p[2];
		q[6] = p[1];
		q[7] = p[0];
	}
	else
	{
		q[0] = p[0];
		q[1] = p[1];
		q[2] = p[2];
		q[3] = p[3];
		q[4] = p[4];
		q[5] = p[5];
		q[6] = p[6];
		q[7] = p[7];
	}
}


double getInfinity()
{
	static int initialized = 0;
	static double infinity;
	if (!initialized)
	{
		assert(sizeof(double) == 8);
		initialized = 1;
		GMSEC_U8 raw[8] = { 0 };
		for (int i = 1; i <= 11; ++i)
			setBit(raw, i);
		storeDouble(&infinity, raw);
	}
	return infinity;
}


float getInfinityf()
{
	static int initialized = 0;
	static float infinity;
	if (!initialized)
	{
		initialized = 1;
		GMSEC_U8 raw[4] = { 0 };
		for (int i = 1; i <= 8; ++i)
			setBit(raw, i);
		storeFloat(&infinity, raw);
	}
	return infinity;
}


double getNaN()
{
	static int initialized = 0;
	static double nan;
	if (!initialized)
	{
		assert(sizeof(double) == 8);
		initialized = 1;
		GMSEC_U8 raw[8];
		for (size_t i = 0; i < sizeof(raw); ++i)
			raw[i] = 0xff;
		storeDouble(&nan, raw);
	}
	return nan;
}


float getNaNf()
{
	static int initialized = 0;
	static float nan;
	if (!initialized)
	{
		assert(sizeof(float) == 4);
		initialized = 1;
		GMSEC_U8 raw[4];
		for (size_t i = 0; i < sizeof(raw); ++i)
			raw[i] = 0xff;
		storeFloat(&nan, raw);
	}
	return nan;
}


static bool isInfinity(double x)
{
	unsigned char *p = reinterpret_cast<unsigned char*>(&x);
	int exponent;

	if (isLittleEndian())
		exponent = (p[6] >> 4) + 16 * (p[7] & 0x7f);
	else
		exponent = (p[1] >> 4) + 16 * (p[0] & 0x7f);

	return exponent == 0x7ff;
}


const char *realStringConstant(double x)
{
	if (x != x)
		return "NAN";
	else if (isInfinity(x))
	{
		if (x < 0)
			return "-INF";
		else
			return "+INF";
	}

	return 0;
}


float stringRealConstant(const char *p)
{
	if (stringEquals(p, "NAN"))
		return getNaNf();

	if (stringEquals(p, "INF") || stringEquals(p, "+INF"))
		return getInfinityf();

	if (stringEquals(p, "-INF"))
		return -getInfinityf();

	return SIMPLE;
}


#ifdef GMSEC_VC6

std::ostream& operator<<(std::ostream& os, GMSEC_I64 i)
{
	char buf[32];
	sprintf(buf, "%I64d", i);
	os << buf;
	return os;
}

std::istream& operator>>(std::istream& in, GMSEC_I64 &o)
{
	char buf[32];
	int got = 0;
	in >> buf;
	if (in)
		got = sscanf(buf, "%I64d", &o);
	if (got == 0)
		o = 0;
	return in;
}

#endif /* GMSEC_VC6 */


GMSEC_API GMSEC_I64 CALL_TYPE stringToI64(const char *str)
{
	GMSEC_I64 ret = 0;
	std::istringstream i(str);
	i >> ret;
	return ret;
}

GMSEC_API char * CALL_TYPE I64toString(GMSEC_I64 val, char *buf, size_t destSize)
{
	std::ostringstream o;
	o << val;
	stringCopy(buf, o.str().c_str(), destSize);
	return buf;
}


} // namespace util
} // namespace gmsec

