/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec4/secure/Random.h
 *	Interface to random number generation.
 */

#ifndef GMSEC_API5_SECURE_RANDOM_H
#define GMSEC_API5_SECURE_RANDOM_H


#include <gmsec5/Config.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/wdllexp.h>


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

#include <stddef.h>  // for size_t


namespace gmsec {
namespace api5 {
namespace secure {


class AbstractRandom;


using gmsec::api5::Status;
using gmsec::api5::Config;
using gmsec::api5::util::DataBuffer;


class GMSEC_API Random
{
public:

	Random ();
	~Random ();

	bool CALL_TYPE isValid () const;
	const char * CALL_TYPE getID () const;

	Status CALL_TYPE initialize (const Config &config);

	/** @method fill (DataBuffer &out)
	 * Generate random data and fill the buffer.
	 */
	bool CALL_TYPE fill (DataBuffer &buffer);

	/** @method nextU32 (GMSEC_U32 &out)
	 * Generate a random number on [ 0, 2^32 ).
	 */
	bool CALL_TYPE nextU32 (GMSEC_U32 &out);

	/** @method nextRange (GMSEC_U32 range, GMSEC_U32 &out)
	 * Generate a random number on [ 0, range ).
	 */
	bool CALL_TYPE nextRange (GMSEC_U32 range, GMSEC_U32 &out);

	/** @method nextBits (int bits, GMSEC_U32 &out)
	 * Generate the requested number of random bits.
	 */
	bool CALL_TYPE nextBits (int bits, GMSEC_U32 &out);

	/** @method nextF64 (GMSEC_F64 &out)
	 * Generate an IEEE 64 bit float.
	 */
	bool CALL_TYPE nextF64 (GMSEC_F64 &out);

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

	virtual bool CALL_TYPE isValid () const = 0;
	virtual const char * CALL_TYPE getID () const = 0;

	virtual Status CALL_TYPE initialize (const Config &config) = 0;

	virtual bool CALL_TYPE fill (DataBuffer &buffer) = 0;
	virtual bool CALL_TYPE nextU32 (GMSEC_U32 &out) = 0;
	virtual bool CALL_TYPE nextRange (GMSEC_U32 range, GMSEC_U32 &out) = 0;
	virtual bool CALL_TYPE nextBits (int bits, GMSEC_U32 &out) = 0;
	virtual bool CALL_TYPE nextF64 (GMSEC_F64 &out) = 0;

	void CALL_TYPE setExternal (Random &random);
};


class GMSEC_API BaseRandom : public AbstractRandom
{
public:
	// provide default implementations of these in terms of nextU32
	virtual bool CALL_TYPE fill (DataBuffer &buffer);
	virtual bool CALL_TYPE nextRange (GMSEC_U32 range, GMSEC_U32 &out);
	virtual bool CALL_TYPE nextBits (int bits, GMSEC_U32 &out);
	virtual bool CALL_TYPE nextF64 (GMSEC_F64 &out);
};


class GMSEC_API SystemRandom : public BaseRandom
{
public:
	SystemRandom ();
	virtual ~SystemRandom ();

	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual bool CALL_TYPE fill (DataBuffer &buffer);
	virtual bool CALL_TYPE nextU32 (GMSEC_U32 &out);

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
	MersenneTwister ();
	virtual ~MersenneTwister ();

	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual bool CALL_TYPE nextU32 (GMSEC_U32 &out);

private:
	// Declared, but not implemented.
	MersenneTwister(const MersenneTwister &);
	MersenneTwister &operator=(const MersenneTwister &);

	Status CALL_TYPE setSeed (GMSEC_U32 seed);
	Status CALL_TYPE setSeed (const DataBuffer &buffer);

	static const size_t MERSENNE_N = 624;

	Status    status;
	GMSEC_U32 mt[MERSENNE_N];
	size_t    mti;
};


} // namespace secure
} // namespace api5
} // namespace gmsec

#endif
