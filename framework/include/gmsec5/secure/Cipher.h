/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Cipher.h
 * @brief Holds declaration of AbstractCipher interface.
*/

#ifndef GMSEC_API5_SECURE_CIPHER_H
#define GMSEC_API5_SECURE_CIPHER_H

#include <gmsec5/Config.h>

#include <gmsec5/ConfigOptions.h>

#include <gmsec5/internal/Value.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec {
namespace api5 {
namespace secure {

using gmsec::api5::Config;
using gmsec::api5::util::DataBuffer;
using gmsec::api5::internal::ValueMap;


class AbstractCipher;


class GMSEC_API Cipher
{
public:

	/** @constructor
		Instantiates a null cipher.
	*/
	Cipher ();
	~Cipher ();

	bool CALL_TYPE isValid () const;
	const char * CALL_TYPE getID () const;

	/** @method initialize (const Config &config)
		Initializes the cipher according to the config parameters:
			SEC-CIPHER
			e.g., "RSA", "ROTATE"
		and algorithm specific parameters:
			SEC-CIPHER-<PARAM>
	*/
	Status CALL_TYPE initialize (const Config &config);

	Status CALL_TYPE encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	Status CALL_TYPE decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

private:

	// Declared, but not implemented.
	Cipher (const Cipher &other);
	Cipher &operator= (const Cipher &other);

	friend class AbstractCipher;

	AbstractCipher *ptr;
};



class GMSEC_API AbstractCipher
{
public:
	virtual ~AbstractCipher ();

	virtual bool CALL_TYPE isValid () const = 0;
	virtual const char * CALL_TYPE getID () const = 0;

	virtual Status CALL_TYPE initialize (const Config &config) = 0;
	virtual Status CALL_TYPE encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta) = 0;
	virtual Status CALL_TYPE decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta) = 0;

	void CALL_TYPE setExternal (Cipher &object);
};



// NullCipher

class GMSEC_API NullCipher : public AbstractCipher
{
public:
	NullCipher ();

	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual Status CALL_TYPE encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	virtual Status CALL_TYPE decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

private:
	// Declared, but not implemented.
	NullCipher(const NullCipher &);
	NullCipher &operator=(const NullCipher &);

	Status status;
};


class GMSEC_API DisabledCipher : public AbstractCipher
{
public:
	DisabledCipher ();
	virtual ~DisabledCipher ();

	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual Status CALL_TYPE encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	virtual Status CALL_TYPE decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

private:
	// Declared, but not implemented.
	DisabledCipher(const DisabledCipher &);
	DisabledCipher &operator=(const DisabledCipher &);
};


// RotateCipher is for demonstration purposes.

class GMSEC_API RotateCipher : public AbstractCipher
{
public:
	RotateCipher ();

	virtual bool CALL_TYPE isValid () const;
	virtual const char * CALL_TYPE getID () const;

	virtual Status CALL_TYPE initialize (const Config &config);

	virtual Status CALL_TYPE encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	virtual Status CALL_TYPE decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

private:
	// Declared, but not implemented.
	RotateCipher(const RotateCipher &);
	RotateCipher &operator=(const RotateCipher &);

	Status status;
	bool ready;
	GMSEC_I32 delta;
};


} // namespace secure
} // namespace api5
} // namespace gmsec

#endif
