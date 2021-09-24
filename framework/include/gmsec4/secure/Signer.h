/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file gmsec4/security/Signer.h
	Holds declaration of AbstractSigner interface.
*/

#ifndef GMSEC_API_SECURE_SIGNER_H
#define GMSEC_API_SECURE_SIGNER_H


#include <gmsec4/Config.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec {
namespace api {
namespace secure {


using gmsec::api::Status;
using gmsec::api::Config;
using gmsec::api::util::DataBuffer;


class AbstractSigner;


class GMSEC_API Signer
{
public:

	Signer ();
	~Signer ();

	bool isValid () const;
	const char * getID () const;
	bool isMiddleware () const;

	/** @method initialize (const Config &config)
		Initializes the signer according to the config parameters:
			SEC-SIGNER
			e.g., "MD5"
		and algorithm specific parameters:
			SEC-SIGNER-<PARAM>
		sign/validate are only invoked if !isMiddleware.
	*/
	Status initialize (const Config &config);
	Status sign (const DataBuffer &in, DataBuffer &out);
	Status validate (const DataBuffer &in, const DataBuffer &digest);

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

	virtual bool isValid () const = 0;
	virtual const char * getID () const = 0;
	virtual bool isMiddleware () const = 0;

	virtual Status initialize (const Config &config) = 0;
	virtual Status sign (const DataBuffer &in, DataBuffer &out) = 0;

	virtual Status validate (const DataBuffer &in, const DataBuffer &digest) = 0;

	void setExternal (Signer &signer);
};


class GMSEC_API BaseSigner : public AbstractSigner
{
public:

	virtual bool isValid () const;
	virtual bool isMiddleware () const;
	virtual bool test (const DataBuffer &in, const DataBuffer &digest);
	virtual Status validate (const DataBuffer &in, const DataBuffer &digest);

protected:
	Status status;
};


class GMSEC_API NullSigner : public BaseSigner
{
public:
	NullSigner ();

	virtual const char * getID () const;
	virtual Status initialize (const Config &config);
	virtual Status sign (const DataBuffer &in, DataBuffer &out);

};


class GMSEC_API DisabledSigner : public BaseSigner
{
public:
	DisabledSigner ();
	~DisabledSigner ();

	virtual const char * getID () const;
	virtual Status initialize (const Config &config);
	virtual Status sign (const DataBuffer &in, DataBuffer &out);
};


// Sum32Signer is a silly implementation for testing

class GMSEC_API Sum32Signer : public BaseSigner
{
public:
	Sum32Signer ();

	virtual const char * getID () const;
	virtual Status initialize (const Config &config);
	virtual Status sign (const DataBuffer &in, DataBuffer &out);

};


} // namespace secure
} // namespace api
} // namespace gmsec

#endif

