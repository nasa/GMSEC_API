/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file gmsec/util/MersenneTwister.cpp
	Implementation of default random number generator.
 */


/* This is derived from mt19937ar.c

   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/


#include <gmsec/secure/Random.h>

#include <gmsec/util/sysutil.h>
#include <gmsec/util/timeutil.h>
#include <gmsec/internal/strutil.h>
#include <gmsec/internal/Log.h>


namespace gmsec {
namespace secure {

using namespace gmsec::util;

/* Period parameters */
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */


static GMSEC_U32 taste;



MersenneTwister::MersenneTwister()
	:
	mti(MERSENNE_N + 1)
{
}


MersenneTwister::~MersenneTwister()
{
}


bool MersenneTwister::isValid() const
{
	return !status.isError();
}


const char * MersenneTwister::getID() const
{
	return "Mersenne";
}


Status MersenneTwister::initialize(const Config &config)
{
	Status localStatus;
	DataBuffer seed;

	const char * value = 0;
	Status test;
	test = config.GetValue("SEC-RANDOM-SEED-NUMBER", value);
	if (!test.isError() && value)
	{
		GMSEC_I32 tmp = 0;
		if (stringParseI32(value, tmp))
			return setSeed(GMSEC_U32(tmp));
		else
		{
			localStatus.Set(GMSEC_STATUS_OTHER_ERROR,
				GMSEC_INVALID_CONFIG_VALUE,
				"Invalid seed number");
			return localStatus;
		}
	}

	test = config.GetValue("SEC-RANDOM-SEED-STRING", value);
	if (!test.isError() && value)
	{
		seed.reset((const DataBuffer::data_t *) value, (int) stringLength(value), false);
		return setSeed(seed);
	}

	test = config.GetValue("SEC-RANDOM-SEED-CRYPT", value);
	if (!test.isError() && value)
	{
		GMSEC_I32 bytes = 0;
		if (!stringParseI32(value, bytes) || bytes < 1)
		{
			bytes = 4;
			LOG_WARNING << "MersenneTwister provided invalid crypt count " << value;
		}

		seed.resize(bytes);
		SystemRandom random;
		if (!random.fill(seed))
		{
			LOG_WARNING << "MT: unable to get fill " << bytes;
		}
		return setSeed(seed);
	}
	else
	{
		// select the random bytes ourselves
		double t = gmsec::util::getTime_s();
		int pid = getProcessID();
		std::string name;
		getHostName(name);
		union
		{
			GMSEC_F64 f64;
			GMSEC_U32 u32[2];
			GMSEC_U8 u8[8];
		} tmp;
		tmp.f64 = GMSEC_F64(t);
		for (int i = 0; i < 4; ++i)
			tmp.u8[i] ^= tmp.u8[7-i];
		tmp.u32[0] ^= taste;
		tmp.u32[0] ^= GMSEC_U32(pid);
		for (size_t i = 0; i < name.length(); ++i)
			tmp.u8[i%4] ^= GMSEC_U8(name[i]);

		localStatus = setSeed(tmp.u32[0]);

		nextU32(taste);
	}

	return localStatus;
}


Status MersenneTwister::setSeed(const DataBuffer &buffer)
{
	/* initialize by an array with array-length */
	/* init_key is the array for initializing keys */
	/* key_length is its length */
	/* slight change for C++, 2004/2/26 */
	GMSEC_U32 *init_key = (GMSEC_U32 *) buffer.get();
	int key_length = buffer.size() / 4;

	int i, j, k;
	setSeed(19650218UL);
	i = 1;
	j = 0;
	k = (MERSENNE_N > key_length ? MERSENNE_N : key_length);

	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
				+ init_key[j] + j; /* non linear */
		mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
		i++;
		j++;
		if (i >= MERSENNE_N) {
			mt[0] = mt[MERSENNE_N - 1];
			i = 1;
		}
		if (j >= key_length)
			j = 0;
	}

	for (k = MERSENNE_N - 1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL)) - i; /* non linear */
		mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
		i++;
		if (i >= MERSENNE_N) {
			mt[0] = mt[MERSENNE_N - 1];
			i = 1;
		}
	}

	mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */

	return status;
}


Status MersenneTwister::setSeed(GMSEC_U32 seed)
{
	mt[0] = seed & 0xffffffffUL;
	for (mti = 1; mti < MERSENNE_N; mti++) {
		mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}

	return status;
}


bool MersenneTwister::nextU32(GMSEC_U32 &out)
{
	GMSEC_U32 y;
	static GMSEC_U32 mag01[2] = { 0x0UL, MATRIX_A };
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= MERSENNE_N) { /* generate MERSENNE_N words at one time */
		int kk;

		if (mti == MERSENNE_N + 1) /* if init_genrand() has not been called, */
			setSeed(5489UL); /* a default initial seed is used */

		for (kk = 0; kk < MERSENNE_N - M; kk++) {
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (; kk < MERSENNE_N - 1; kk++) {
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + (M - MERSENNE_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[MERSENNE_N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[MERSENNE_N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	out = y;

	return isValid();
}


} // namespace util
} // namespace gmsec

