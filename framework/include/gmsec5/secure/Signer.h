/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec4/security/Signer.h
	Holds declaration of AbstractSigner interface.
*/

#ifndef GMSEC_API5_SECURE_SIGNER_H
#define GMSEC_API5_SECURE_SIGNER_H


#include <gmsec5/Config.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec {
namespace api5 {
namespace secure {


using gmsec::api5::Status;
using gmsec::api5::Config;
using gmsec::api5::util::DataBuffer;


class AbstractSigner;


class GMSEC_API Signer
{
public:
	Signer ();
	~Signer ();

	bool CALL_TYPE isValid () const;
	const char * CALL_TYPE getID () const;
	bool CALL_TYPE isMiddleware () const;

	/** @method initialize (const Config &config)
		Initializes the signer according to the config parameters:
			SEC-SIGNER
			e.g., "MD5"
		and algorithm specific parameters:
			SEC-SIGNER-<PARAM>
		sign/validate are only invoked if !isMiddleware.
	*/
	Status CALL_TYPE initialize (const Config &config);
	Status CALL_TYPE sign (const DataBuffer &in, DataBuffer &out);
	Status CALL_TYPE validate (const DataBuffer &in, const DataBuffer &digest);

private:
	// Declared, but not implemented.
	Signer (const Signer &other);
	Signer &operator= (const Signer &other);

	friend class AbstractSigner;
	AbstractSigner *ptr;
};



class GMSEC_API AbstractSigner
{
public:
	virtual ~AbstractSigner ();

	virtual bool CALL_TYPE isValid () const = 0;
	virtual const char * CALL_TYPE getID () const = 0;
	virtual bool CALL_TYPE isMiddleware () const = 0;

	virtual Status CALL_TYPE initialize (const Config &config) = 0;
	virtual Status CALL_TYPE sign (const DataBuffer &in, DataBuffer &out) = 0;

	virtual Status CALL_TYPE validate (const DataBuffer &in, const DataBuffer &digest) = 0;

	void CALL_TYPE setExternal (Signer &signer);
};


class GMSEC_API BaseSigner : public AbstractSigner
{
public:
	virtual bool CALL_TYPE isValid () const;
	virtual bool CALL_TYPE isMiddleware () const;
	virtual bool CALL_TYPE test (const DataBuffer &in, const DataBuffer &digest);
	virtual Status CALL_TYPE validate (const DataBuffer &in, const DataBuffer &digest);

protected:
	Status status;
};


class GMSEC_API NullSigner : public BaseSigner
{
public:
	NullSigner ();

	virtual const char * CALL_TYPE getID () const;
	virtual Status CALL_TYPE initialize (const Config &config);
	virtual Status CALL_TYPE sign (const DataBuffer &in, DataBuffer &out);
};


class GMSEC_API DisabledSigner : public BaseSigner
{
public:
	DisabledSigner ();
	~DisabledSigner ();

	virtual const char * CALL_TYPE getID () const;
	virtual Status CALL_TYPE initialize (const Config &config);
	virtual Status CALL_TYPE sign (const DataBuffer &in, DataBuffer &out);
};


// Sum32Signer is a silly implementation for testing

class GMSEC_API Sum32Signer : public BaseSigner
{
public:
	Sum32Signer ();

	virtual const char * CALL_TYPE getID () const;
	virtual Status CALL_TYPE initialize (const Config &config);
	virtual Status CALL_TYPE sign (const DataBuffer &in, DataBuffer &out);
};


} // namespace secure
} // namespace api5
} // namespace gmsec

#endif
