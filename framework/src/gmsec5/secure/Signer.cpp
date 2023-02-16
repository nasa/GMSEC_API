/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Signer.h
 * @brief Signer implementation.
 */


#include <gmsec5/secure/Signer.h>

#include <gmsec5/internal/Encoder.h>

#include <gmsec5/util/Log.h>


using namespace gmsec::api5::internal;
using namespace gmsec::api5::secure;
using namespace gmsec::api5::util;


Signer::Signer()
	:
	ptr(new NullSigner())
{
}


Signer::~Signer()
{
	delete ptr;
	ptr = 0;
}


bool Signer::isValid () const
{
	return ptr->isValid();
}


const char * Signer::getID () const
{
	return ptr->getID();
}


bool Signer::isMiddleware () const
{
	return ptr->isMiddleware();
}


Status Signer::initialize (const Config &config)
{
	return ptr->initialize(config);
}


Status Signer::sign (const DataBuffer &in, DataBuffer &out)
{
	return ptr->sign(in, out);
}


Status Signer::validate (const DataBuffer &in, const DataBuffer &digest)
{
	return ptr->validate(in, digest);
}



/*
 * AbstractSigner
 */

AbstractSigner::~AbstractSigner()
{

}


void AbstractSigner::setExternal (Signer &signer)
{
	delete signer.ptr;
	signer.ptr = this;
}


/*
 * BaseSigner
 */

bool BaseSigner::isValid () const
{
	return !status.hasError();
}


bool BaseSigner::isMiddleware () const
{
	return false;
}


Status BaseSigner::validate (const DataBuffer &in, const DataBuffer &digest)
{
	if (status.hasError())
		return status;

	DataBuffer tmp;

	status = sign(in, tmp);
	if (status.hasError())
	{
		GMSEC_WARNING << status.get();
		return status;
	}

	// a Status for this comparison
	Status result;
	if (!digest.equals(tmp))
	{
		result.set(POLICY_ERROR, INVALID_SIGNATURE, "Signature mismatch");
	}

	return result;
}


bool BaseSigner::test (const DataBuffer &in, const DataBuffer &digest)
{
	Status result = validate(in, digest);
	return !result.hasError();
}



/*
 * NullSigner
 */

NullSigner::NullSigner()
{
}


const char * NullSigner::getID () const
{
	return "NULL";
}


Status NullSigner::initialize (const Config &)
{
	status.reset();
	return status;
}


Status NullSigner::sign (const DataBuffer &, DataBuffer &digest)
{
	if (status.hasError())
		return status;

	digest.resize(0);

	return status;
}


DisabledSigner::DisabledSigner ()
{
}


DisabledSigner::~DisabledSigner ()
{
}


const char * DisabledSigner::getID () const
{
	return "DISABLED";
}


Status DisabledSigner::initialize (const Config &)
{
	Status localStatus;
	GMSEC_VERBOSE << "Signatures disabled";
	return localStatus;
}


Status DisabledSigner::sign (const DataBuffer &, DataBuffer &)
{
	Status localStatus(POLICY_ERROR, UNINITIALIZED_OBJECT, "Not configured to sign messages");
	return localStatus;
}

/*
 * Sum32Signer
 */
Sum32Signer::Sum32Signer()
{
}



const char * Sum32Signer::getID () const
{
	return "SUM32";
}


Status Sum32Signer::initialize (const Config &)
{
	status.reset();
	return status;
}


Status Sum32Signer::sign (const DataBuffer &in, DataBuffer &digest)
{
	if (status.hasError())
		return status;

	GMSEC_U32 tmp = 0;

	const DataBuffer::data_t *pi = in.get();
	for (size_t i = 0; i < in.size(); ++i)
	{
		tmp += pi[i];
	}

	digest.resize(4);
	char * raw = reinterpret_cast<char *>(digest.raw());

	Encoder *encoder = Encoder::getEncoder();
	encoder->putU32(&tmp, raw);

	return status;
}
