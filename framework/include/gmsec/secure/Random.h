
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file gmsec/secure/Random.h
	Interface to random number generation.
*/

#ifndef gmsec_secure_Random_h
#define gmsec_secure_Random_h

/*
 * OS provided random numbers.
 */
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <wincrypt.h>
#else
#include <string>
#endif

#include <gmsec/Config.h>
#include <gmsec/util/Buffer.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/sysutil.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec {
namespace secure {


class AbstractRandom;


using gmsec::Status;
using gmsec::Config;
using gmsec::util::DataBuffer;


class GMSEC_API Random
{
public:

	GMSEC_DEPRECATED Random ();
	~Random ();

	bool isValid () const;
	const char * getID () const;

	Status initialize (const Config &config);

	/** @method fill (DataBuffer &out)
	 * Generate random data and fill the buffer.
	 */
	bool fill (DataBuffer &buffer);

	/** @method nextU32 (GMSEC_U32 &out)
	 * Generate a random number on [ 0, 2^32 ).
	 */
	bool nextU32 (GMSEC_U32 &out);

	/** @method nextRange (GMSEC_U32 range, GMSEC_U32 &out)
	 * Generate a random number on [ 0, range ).
	 */
	bool nextRange (GMSEC_U32 range, GMSEC_U32 &out);

	/** @method nextBits (int bits, GMSEC_U32 &out)
	 * Generate the requested number of random bits.
	 */
	bool nextBits (int bits, GMSEC_U32 &out);

	/** @method nextF64 (GMSEC_F64 &out)
	 * Generate an IEEE 64 bit float.
	 */
	bool nextF64 (GMSEC_F64 &out);

private:

	// Declared, but not implemented.
	Random (const Random &);
	Random &operator= (const Random &);

	friend class AbstractRandom;

	AbstractRandom * ptr;
};



class GMSEC_API AbstractRandom
{
public:

	virtual ~AbstractRandom ();

	virtual bool isValid () const = 0;
	virtual const char * getID () const = 0;

	virtual Status initialize (const Config &config) = 0;

	virtual bool fill (DataBuffer &buffer) = 0;
	virtual bool nextU32 (GMSEC_U32 &out) = 0;
	virtual bool nextRange (GMSEC_U32 range, GMSEC_U32 &out) = 0;
	virtual bool nextBits (int bits, GMSEC_U32 &out) = 0;
	virtual bool nextF64 (GMSEC_F64 &out) = 0;

	void setExternal (Random &random);
};


class GMSEC_API BaseRandom : public AbstractRandom
{
public:

	// provide default implementations of these in terms of nextU32
	virtual bool fill (DataBuffer &buffer);
	virtual bool nextRange (GMSEC_U32 range, GMSEC_U32 &out);
	virtual bool nextBits (int bits, GMSEC_U32 &out);
	virtual bool nextF64 (GMSEC_F64 &out);

};



class GMSEC_API SystemRandom : public BaseRandom
{
public:

	GMSEC_DEPRECATED SystemRandom ();
	virtual ~SystemRandom ();

	virtual bool isValid () const;
	virtual const char * getID () const;

	virtual Status initialize (const Config &config);

	virtual bool fill (DataBuffer &buffer);
	virtual bool nextU32 (GMSEC_U32 &out);

private:

	// Declared, but not implemented.
	SystemRandom(const SystemRandom &);
	SystemRandom &operator=(const SystemRandom &);

	Status status;

#ifdef WIN32
	HCRYPTPROV handle;
#else
	std::string path;
#endif

};


/*
 * A good pseudo random number generator.
 */
class GMSEC_API MersenneTwister : public BaseRandom
{
public:

	GMSEC_DEPRECATED MersenneTwister ();
	virtual ~MersenneTwister ();

	virtual bool isValid () const;
	virtual const char * getID () const;

	virtual Status initialize (const Config &config);

	virtual bool nextU32 (GMSEC_U32 &out);

private:

	// Declared, but not implemented.
	MersenneTwister(const MersenneTwister &);
	MersenneTwister &operator=(const MersenneTwister &);

	Status setSeed (GMSEC_U32 seed);
	Status setSeed (const DataBuffer &buffer);

	static const int MERSENNE_N = 624;

	Status status;
	GMSEC_U32 mt[MERSENNE_N];
	int mti;

};


} // namespace secure
} // namespace gmsec

#endif /* gmsec_secure_Random_h */
