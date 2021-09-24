/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/internal/MathUtil.h>

#include <gmsec4/internal/Encoder.h>
#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Exception.h>

#include <limits>
#include <cmath>

#include <assert.h>


using namespace gmsec::api::util;


namespace gmsec
{
namespace api
{
namespace internal
{

static const float SIMPLE = 1.5f;


/**
 * @fn isLittleEndian()
 * @brief Determines if this is a little endian platform.
 */
static bool isLittleEndian()
{
	int x = 1;
	char* p = reinterpret_cast<char*>(&x);
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
	char* q = reinterpret_cast<char*>(f);

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
	char* q = reinterpret_cast<char*>(d);

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


static unsigned char lookupHexDigit(char digit)
{
	switch (digit)
	{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			return (unsigned char)((digit - 'a') + 10);
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return (unsigned char)((digit - 'A') + 10);
		default:
			return (unsigned char)(digit - '0');
	}
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


bool MathUtil::floatEquals(GMSEC_F32 f1, GMSEC_F32 f2)
{
    static GMSEC_F32 epsilon = std::numeric_limits<GMSEC_F32>::epsilon();

    return fabsf(f1 - f2) < epsilon;
}


bool MathUtil::doubleEquals(GMSEC_F64 d1, GMSEC_F64 d2)
{
    static GMSEC_F64 epsilon = std::numeric_limits<GMSEC_F64>::epsilon();

    return fabs(d1 - d2) < epsilon;
}


GMSEC_F32 MathUtil::extractF32(const char* hexstr)
{
	if (StringUtil::stringLength(hexstr) != 8)
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD_VALUE,
			"Field F32 BITS must be 8 hex digits");
	}

    GMSEC_U8 raw[4];
    for (int i = 0; i < 4; ++i)
    {
        int upper = lookupHexDigit(hexstr[i*2]);
        int lower = lookupHexDigit(hexstr[i*2 + 1]);
        if (upper < 0 || lower < 0)
        {
			throw Exception(FIELD_ERROR, INVALID_FIELD_VALUE,
                    "Field F32 BITS must be 32 hex digits");
        }
        raw[i] = upper*16 + lower;
    }

    const char* pi = reinterpret_cast<char *>(&raw[0]);
	GMSEC_F32   x;

    Decoder::getDecoder()->getF32(pi, &x);

	return x;
}


GMSEC_F64 MathUtil::extractF64(const char* hexstr)
{
	if (StringUtil::stringLength(hexstr) != 16)
	{
		throw Exception(FIELD_ERROR, INVALID_FIELD_VALUE,
			"Field F64 BITS must be 16 hex digits");
	}

	GMSEC_U8 raw[8];
	for (int i = 0; i < 8; ++i)
	{
		int upper = lookupHexDigit(hexstr[i*2]);
		int lower = lookupHexDigit(hexstr[i*2 + 1]);
		if (upper < 0 || lower < 0)
		{
			throw Exception(FIELD_ERROR, INVALID_FIELD_VALUE,
				"Field F64 BITS must be 16 hex digits");
		}
		raw[i] = upper*16 + lower;
	}

	const char* pi = reinterpret_cast<char *>(&raw[0]);
	GMSEC_F64   x;

    Decoder::getDecoder()->getF64(pi, &x);

	return x;
}


double MathUtil::getInfinity()
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


float MathUtil::getInfinityf()
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


double MathUtil::getNaN()
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


float MathUtil::getNaNf()
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


const char* MathUtil::realStringConstant(double x)
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


float MathUtil::stringRealConstant(const char* p)
{
	if (StringUtil::stringEquals(p, "NAN"))
		return getNaNf();

	if (StringUtil::stringEquals(p, "INF") || StringUtil::stringEquals(p, "+INF"))
		return getInfinityf();

	if (StringUtil::stringEquals(p, "-INF"))
		return -getInfinityf();

	return SIMPLE;
}


} // namespace internal
} // namespace api
} // namespace gmsec
