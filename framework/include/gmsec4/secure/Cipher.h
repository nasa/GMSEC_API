/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec4/security/Cipher.h
	Holds declaration of AbstractCipher interface.
*/

#ifndef GMSEC_API_SECURE_CIPHER_H
#define GMSEC_API_SECURE_CIPHER_H

#include <gmsec4/Config.h>

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/internal/Value.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec {
namespace api {
namespace secure {

using gmsec::api::Config;
using gmsec::api::util::DataBuffer;
using gmsec::api::internal::ValueMap;


class AbstractCipher;


class GMSEC_API Cipher
{
public:

	/** @constructor
		Instantiates a null cipher.
	*/

	Cipher ();
	~Cipher ();

	bool isValid () const;
	const char * getID () const;

	/** @method initialize (const Config &config)
		Initializes the cipher according to the config parameters:
			SEC-CIPHER
			e.g., "RSA", "ROTATE"
		and algorithm specific parameters:
			SEC-CIPHER-<PARAM>
	*/
	Status initialize (const Config &config);

	Status encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	Status decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

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

	virtual bool isValid () const = 0;
	virtual const char * getID () const = 0;

	virtual Status initialize (const Config &config) = 0;
	virtual Status encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta) = 0;
	virtual Status decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta) = 0;

	void setExternal (Cipher &object);

};



// NullCipher

class GMSEC_API NullCipher : public AbstractCipher
{
public:
	NullCipher ();

	virtual bool isValid () const;
	virtual const char * getID () const;

	virtual Status initialize (const Config &config);

	virtual Status encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	virtual Status decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

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

	virtual bool isValid () const;
	virtual const char * getID () const;

	virtual Status initialize (const Config &config);

	virtual Status encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	virtual Status decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

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

	virtual bool isValid () const;
	virtual const char * getID () const;

	virtual Status initialize (const Config &config);

	virtual Status encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);
	virtual Status decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta);

private:

	// Declared, but not implemented.
	RotateCipher(const RotateCipher &);
	RotateCipher &operator=(const RotateCipher &);

	Status status;
	bool ready;
	GMSEC_I32 delta;

};


} // namespace secure
} // namespace api
} // namespace gmsec

#endif

