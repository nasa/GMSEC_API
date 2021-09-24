
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file gmsec/security/Cipher.h
	Holds declaration of AbstractCipher interface.
*/

#ifndef gmsec_secure_Cipher_h
#define gmsec_secure_Cipher_h

#include <gmsec/Config.h>
#include <gmsec/util/Buffer.h>
#include <gmsec/internal/Value.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>

namespace gmsec {
namespace secure {

using gmsec::Config;
using gmsec::util::DataBuffer;
using gmsec::internal::ValueMap;


class AbstractCipher;


class GMSEC_API Cipher
{
public:

	/** @constructor
		Instantiates a null cipher.
	*/

	GMSEC_DEPRECATED Cipher ();
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
	GMSEC_DEPRECATED NullCipher ();

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

	GMSEC_DEPRECATED DisabledCipher ();
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
const char KEY_DELTA[] = "SEC-CIPHER-DELTA";

class GMSEC_API RotateCipher : public AbstractCipher
{
public:

	GMSEC_DEPRECATED RotateCipher ();

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
} // namespace gmsec

#endif /* gmsec_secure_Cipher_h */

