/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file gmsec/secure/Random.cpp
	Implementation of default random number generators.
 */

#ifndef WIN32
#include <ios>
#include <fstream>
#endif

#include <gmsec/secure/Random.h>

#include <gmsec/internal/Log.h>


namespace gmsec {
namespace secure {


using namespace gmsec::util;


Random::Random ()
	:
	ptr(new SystemRandom())
{
}


Random::~Random()
{
	delete ptr;
	ptr = 0;
}


bool Random::isValid() const
{
	return ptr->isValid();
}


const char * Random::getID() const
{
	return ptr->getID();
}


Status Random::initialize(const Config &config)
{
	return ptr->initialize(config);
}


bool Random::fill(DataBuffer &buffer)
{
	return ptr->fill(buffer);
}


bool Random::nextU32(GMSEC_U32 &out)
{
	return ptr->nextU32(out);
}


bool Random::nextRange(GMSEC_U32 range, GMSEC_U32 &out)
{
	return ptr->nextRange(range, out);
}


bool Random::nextBits(int bits, GMSEC_U32 &out)
{
	return ptr->nextBits(bits, out);
}


bool Random::nextF64(GMSEC_F64 &out)
{
	return ptr->nextF64(out);
}




AbstractRandom::~AbstractRandom()
{
}


void AbstractRandom::setExternal (Random &random)
{
	delete random.ptr;
	random.ptr = this;
}

/*
 * BaseRandom implementation
 */

bool BaseRandom::fill(DataBuffer &buffer)
{
	GMSEC_U32 tmp = 0;
	DataBuffer::data_t * p = buffer.raw();
	DataBuffer::data_t * q = 0;
	DataBuffer::data_t * q0 = reinterpret_cast<DataBuffer::data_t*>(&tmp);

	for (int i = 0; i < buffer.size(); ++i)
	{
		if ((i % 4) == 0)
		{
			if (!nextU32(tmp))
				return false;
			q = q0;
		}

		p[i] = *q++;
	}

	return true;
}


bool BaseRandom::nextRange(GMSEC_U32 range, GMSEC_U32 &out)
{
	if (range < 2)
		return false;

	bool ok = false;

	GMSEC_U32 tmp = 0;

#if 1

	// this approach is lop-sided (especially for large values of range)
	ok = nextU32(tmp);
	out = tmp % range;

#else

	// this is too much work?
	int bits = 0;
	int x = range - 1;
	while (x > 0)
	{
		x /= 2;
		++bits;
	}

	while (!ok)
	{
		if (!nextBits(bits, tmp))
			return false;
		if (tmp < range)
		{
			out = tmp;
			ok = true;
		}
	}

#endif

	return ok;
}


bool BaseRandom::nextBits(int bits, GMSEC_U32 &out)
{
	if (bits < 1)
		return false;

	GMSEC_U32 tmp;
	nextU32(tmp);

	out = tmp >> (32 - bits);

	return true;
}


/* generates a random number on [0,1) with 53-bit resolution*/
bool BaseRandom::nextF64(GMSEC_F64 &out)
{
	GMSEC_U32 a, b;
	if (!nextU32(a) || !nextU32(b))
		return false;
	a >>= 5;
	b >>= 6;

	out = (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);

	return true;
}



/*
 * SystemRandom implementation
 */

#ifdef WIN32
#include <wincrypt.h>
#endif
#include <string>

using std::string;

SystemRandom::SystemRandom()
		:
#ifdef WIN32
		handle(0)
#else
		path("/dev/urandom")
#endif
{
}


SystemRandom::~SystemRandom()
{
	string errstr;

#ifdef WIN32
	if (handle)
	{
		if (!CryptReleaseContext(handle, 0))
			getErrorString(GetLastError(), errstr);

		handle = 0;
	}
#endif

	if (errstr.size())
	{
		LOG_WARNING << "~SystemRandom: " << errstr;
	}
}


bool SystemRandom::isValid () const
{
	return !status.isError();
}


const char * SystemRandom::getID() const
{
	return "SystemRandom";
}


Status SystemRandom::initialize(const Config &config)
{
	status.ReSet();

#ifdef WIN32

	DWORD type = PROV_RSA_AES;
	// Update to use configuration parameter SEC-RANDOM-PROV?

	if (!CryptAcquireContext(&handle, NULL, NULL, type, CRYPT_VERIFYCONTEXT))
	{
		string errstr;
		getErrorString(GetLastError(), errstr);
		string full = "Unable to acquire CryptContext: " + errstr;
		status.Set(GMSEC_STATUS_POLICY_ERROR, GMSEC_UNINITIALIZED_OBJECT,
				full.c_str());
	}

#else /* not WIN32 */

	const char * value = 0;
	Status test = config.GetValue("SEC-RANDOM-PATH", value);
	if (!test.isError() && value)
		path = value;

	std::ifstream stream(path.c_str());
	if (!stream)
	{
		status.Set(GMSEC_STATUS_POLICY_ERROR, GMSEC_UNINITIALIZED_OBJECT,
				"unable to open random source");
		LOG_WARNING << "Random::initialize: " << status.Get();
	}
#if GMSEC_TEST_DEV_RANDOM
	else
	{
		GMSEC_U32 tmp = 0;
		for (int i = 0; i < 5; ++i)
		{
			if (!nextU32(tmp))
				LOG_WARNING << "SR::i: nextU32 error";
			LOG_DEBUG << "SR::i: nextU32 => " << tmp;
			
		}
		DataBuffer tb(8);
		if (!fill(tb))
			LOG_WARNING << "SR::i: fill error";
		for (int i = 0; i < tb.size(); ++i)
			LOG_DEBUG << "SR::i: fill => " << int(tb.get()[i]);
	}
#endif

#endif /* WIN32 */

	return status;
}



bool SystemRandom::fill(DataBuffer &out)
{
	if (status.isError())
		return false;

	bool ok = false;

#ifdef WIN32
	BYTE * p = reinterpret_cast<BYTE *>(out.raw());
	if (CryptGenRandom(handle, out.size(), p))
		ok = true;
	else
	{
		string errstr;
		getErrorString(GetLastError(), errstr);
		LOG_WARNING << "SystemRandom::fill: unable to generate " << out.size() << ": " << errstr;
	}

#else
	std::ifstream stream(path.c_str());
	if (!stream)
	{
		LOG_WARNING << "SystemRandom::fill: unable to open " << path;
		return false;
	}
	stream.read(reinterpret_cast<char*>(out.raw()), out.size());
	int n = stream.gcount();
	if (n == out.size())
		ok = true;
	else
		LOG_WARNING << "SystemRandom::fill: unable to read " << out.size() << ": got " << n;

#endif /* WIN32 */

	return ok;
}


bool SystemRandom::nextU32(GMSEC_U32 &out)
{
	if (status.isError())
		return false;

	bool ok = false;

#ifdef WIN32

	BYTE * p = reinterpret_cast<BYTE*>(&out);
	if (CryptGenRandom(handle, sizeof(out), p))
		ok = true;
	else
	{
		string errstr;
		getErrorString(GetLastError(), errstr);
		LOG_WARNING << "SystemRandom::nextU32: unable to generate random: " << errstr;
	}

#else

	std::ifstream stream(path.c_str());
	if (!stream)
	{
		LOG_WARNING << "SystemRandom::nextU32: unable to open " << path;
		return false;
	}
	stream.read(reinterpret_cast<char*>(&out), sizeof(out));
	int count = stream.gcount();
	if (count == sizeof(out))
		ok = true;
	else
		LOG_WARNING << "SystemRandom::nextU32: only read " << count;

#endif /* WIN32 */

	return ok;
}


} // namespace util
} // namespace gmsec

