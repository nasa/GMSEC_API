/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Cipher.cpp
	Holds trivial Cipher implementations.
*/

#include <gmsec4/secure/Cipher.h>

#include <gmsec4/util/Log.h>

#include <gmsec4/internal/StringUtil.h>


namespace gmsec {
namespace api {
namespace secure {

using gmsec::api::Status;
using gmsec::api::Config;
using namespace gmsec::api::util;


Cipher::Cipher()
	:
	ptr(new NullCipher())
{
}


Cipher::~Cipher()
{
	delete ptr;
	ptr = 0;
}


bool Cipher::isValid () const
{
	return ptr->isValid();
}


const char * Cipher::getID () const
{
	return ptr->getID();
}


Status Cipher::initialize (const Config &config)
{
	return ptr->initialize(config);
}


Status Cipher::encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta)
{
	return ptr->encrypt(in, out, meta);
}


Status Cipher::decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &meta)
{
	return ptr->decrypt(in, out, meta);
}


/*
 * AbstractCipher
 */

AbstractCipher::~AbstractCipher()
{

}


void AbstractCipher::setExternal (Cipher &cipher)
{
	delete cipher.ptr;
	cipher.ptr = this;
}


/*
 * NullCipher
 */
NullCipher::NullCipher ()
{
}


bool NullCipher::isValid () const
{
	return true;
}


const char * NullCipher::getID () const
{
	return "NULL";
}


Status NullCipher::initialize (const Config &)
{
	return status;
}


Status NullCipher::encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &)
{
	out = in;
	return status;
}


Status NullCipher::decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &)
{
	out = in;
	return status;
}


/*
 * DisabledCipher indicates an error if any operation is attempted
 */
DisabledCipher::DisabledCipher ()
{
}


DisabledCipher::~DisabledCipher ()
{
}


bool DisabledCipher::isValid () const
{
	return true;
}


const char * DisabledCipher::getID () const
{
	return "DISABLED";
}


Status DisabledCipher::initialize (const Config &)
{
	Status status;
	GMSEC_VERBOSE << "Encryption disabled";
	return status;
}


Status DisabledCipher::encrypt (const DataBuffer &, DataBuffer &, ValueMap &)
{
	Status status(POLICY_ERROR, UNINITIALIZED_OBJECT, "Not configured to encrypt messages");
	return status;
}


Status DisabledCipher::decrypt (const DataBuffer &, DataBuffer &, ValueMap &)
{
	Status status(POLICY_ERROR, UNINITIALIZED_OBJECT, "Not configured to decrypt messages");
	return status;
}



/*
 * RotateCipher: move to a library for demonstration.
 */
RotateCipher::RotateCipher ()
		:
		ready(false),
		delta(0)
{
}


bool RotateCipher::isValid () const
{
	return !status.isError();
}


const char * RotateCipher::getID () const
{
	return "ROTATE";
}


Status RotateCipher::initialize (const Config &config)
{
	status.reset();

	const char* value = config.getValue(GMSEC_CIPHER_DELTA);

	if (value)
	{
		if (StringUtil::stringParseI32(value, delta))
		{
			ready = true;
		}
		else
		{
			status.set(POLICY_ERROR, INVALID_CONFIG_VALUE, "Bad SEC-CIPHER-DELTA / SEC-KEY-ID");
		}
	}

	return status;
}


Status RotateCipher::encrypt (const DataBuffer &in, DataBuffer &out, ValueMap &)
{
	if (status.isError())
	{
		return status;
	}
	if (!ready)
	{
		status.set(POLICY_ERROR, UNINITIALIZED_OBJECT, "RotateCipher not initialized");
		GMSEC_WARNING << "RotateCipher::encrypt: " << status.get();
		return status;
	}

	out.resize(in.size());
	const DataBuffer::data_t *pi = in.get();
	DataBuffer::data_t *po = out.raw();
	for (size_t i = 0; i < in.size(); ++i)
	{
		po[i] = (DataBuffer::data_t)(pi[i] + delta);
	}

	return status;
}


Status RotateCipher::decrypt (const DataBuffer &in, DataBuffer &out, ValueMap &)
{
	if (status.isError())
	{
		return status;
	}

	out.resize(in.size());
	const DataBuffer::data_t *pi = in.get();
	DataBuffer::data_t *po = out.raw();
	for (size_t i = 0; i < in.size(); ++i)
	{
		po[i] = (DataBuffer::data_t)(pi[i] - delta);
	}

	return status;
}


} // namespace secure
} // namespace api
} // namespace gmsec
