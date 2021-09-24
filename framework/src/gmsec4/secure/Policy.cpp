/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec4/secure/Policy.h>

#include <gmsec4/internal/DynamicFactory.h>
#include <gmsec4/internal/Encoder.h>
#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/Subject.h>

#include <gmsec4/MessageFieldIterator.h>

#include <gmsec4/util/Log.h>




namespace gmsec {
namespace api {
namespace secure {

// Define DEFAULT_POLICY in config/default for a different policy.
#ifndef DEFAULT_POLICY
#define DEFAULT_POLICY API4Policy
#endif


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


Policy::Policy ()
	:
	ptr(new DEFAULT_POLICY())
{
}


Policy::~Policy ()
{
	delete ptr;
	ptr = 0;
}


Status Policy::initialize (const Config &config)
{
	return ptr->initialize(config);
}


Access &Policy::getAccess ()
{
	return ptr->getAccess();
}


Signer &Policy::getSigner ()
{
	return ptr->getSigner();
}


Cipher &Policy::getCipher ()
{
	return ptr->getCipher();
}


Random &Policy::getRandom ()
{
	return ptr->getRandom();
}



bool Policy::isValidSubject (const char * subject)
{
	return ptr->isValidSubject(subject);
}


bool Policy::isValidSubscription (const char * subject)
{
	return ptr->isValidSubscription(subject);
}


Status Policy::authenticate (Connection &connection)
{
	return ptr->authenticate(connection);
}



Status Policy::checkSubscribe (const char *subject)
{
	return ptr->checkSubscribe(subject);
}


Status Policy::checkSend (const char *subject)
{
	return ptr->checkSend(subject);
}


Status Policy::encode (Message &message, DataBuffer &content)
{
	return ptr->encode(message, content);
}


Status Policy::decode (Message &message, const DataBuffer &in)
{
	return ptr->decode(message, in);
}


Status Policy::package (Message &message, DataBuffer &content, ValueMap &meta)
{
	return ptr->package(message, content, meta);
}


Status Policy::unpackage (Message &message, const DataBuffer &in, ValueMap &meta)
{
	return ptr->unpackage(message, in, meta);
}


/*
 * AbstractPolicy.  Base Policy implementation.
 */


void AbstractPolicy::setExternal (Policy &policy)
{
	delete policy.ptr;
	policy.ptr = this;
}


AbstractPolicy::~AbstractPolicy()
{
}


Access &AbstractPolicy::getAccess ()
{
	return access;
}


Cipher &AbstractPolicy::getCipher ()
{
	return cipher;
}


Random &AbstractPolicy::getRandom ()
{
	return random;
}


Signer &AbstractPolicy::getSigner ()
{
	return signer;
}


Status AbstractPolicy::baseInitialize (const Config &localConfig)
{
	Status status;

	// I see a pattern here...
	status = DynamicFactory::initialize(getAccess(), localConfig);
	if (status.isError())
		return status;

	status = DynamicFactory::initialize(getCipher(), localConfig);
	if (status.isError())
		return status;

	status = DynamicFactory::initialize(getRandom(), localConfig);
	if (status.isError())
		return status;

	status = DynamicFactory::initialize(getSigner(), localConfig);
	if (status.isError())
		return status;

	return status;
}


/*
 * API3Policy.  This maintains backward compatibility with GMSEC API 3
 */

API3Policy::API3Policy ()
	:
	fValidateSubjects(false),
	fLenientSubjects(true),
	fEncodeHeader(true),
	fEncodeXml(false),
	fCompress(false)
{
}


Status API3Policy::initialize (const Config &localConfig)
{
	// Should we validate subjects?  We did not in the past, so default false.
	fValidateSubjects = localConfig.getBooleanValue(GMSEC_VALIDATE_SUBJECT, false);

	// If we are validating subjects, should we be lenient?  We have been through 3.1
	fLenientSubjects = localConfig.getBooleanValue(GMSEC_VALIDATE_SUBJECT_LENIENT, true);

	fEncodeHeader = localConfig.getBooleanValue(ENCODE_HEADER, true);

	//MEH: ER 3924 - keep SEC-ENCODE-XML as an option, even though it hasn't been
	//documented, but normalize documented syntax to GMSEC-ENCODE-XML to conform with
	//JSON's GMSEC-ENCODE-JSON
	fEncodeXml = localConfig.getBooleanValue(ENCODE_XML, false) || 
		localConfig.getBooleanValue(GMSEC_ENCODE_XML, false);

	fEncodeJson = localConfig.getBooleanValue(GMSEC_ENCODE_JSON, false);

	if(fEncodeXml && fEncodeJson)
	{
		fStatus.set(POLICY_ERROR, INVALID_CONFIG,
					"Cannot specify both XML and JSON encoding. XML encoding flag will take priority.");
	}

	fCompress = localConfig.getBooleanValue(POL_COMPRESS, false);

	// fStatus = baseInitialize(localConfig);

	return fStatus;
}


bool API3Policy::isValidSubject (const char *subject)
{
	bool allow = true;
	if (fValidateSubjects)
	{
		allow = Subject::isValid(subject, fLenientSubjects);
	}
	return allow;
}


bool API3Policy::isValidSubscription (const char *subject)
{
	bool allow = true;
	if (fValidateSubjects)
	{
		allow = Subject::isValidSubscription(subject, fLenientSubjects);
	}
	return allow;
}


Status API3Policy::authenticate (Connection &)
{
	Status status;
	return status;
}


Status API3Policy::checkSubscribe (const char *)
{
	Status status;
	return status;
}


Status API3Policy::checkSend (const char *)
{
	Status status;
	return status;
}


Status API3Policy::package (Message &message, DataBuffer &content, ValueMap &meta)
{
	Status result = encode(message, content);

	if (!result.isError() && useCompression())
	{
		DataBuffer compressed;

		result = StringUtil::gmsec_compress(content, compressed);

		if (!result.isError())
		{
			if (result.getCode() == NO_ERROR_CODE)
			{
				meta.setBoolean("ZIP", true);
			}
			content.copy(compressed.get(), compressed.size());
		}
	}

	return result;
}


Status API3Policy::unpackage (Message &message, const DataBuffer &in, ValueMap &meta)
{
	Status result;

	const DataBuffer* content = &in;
	DataBuffer uncompressed;

	bool isCompressed = false;
	meta.getBoolean("ZIP", isCompressed, &isCompressed);

	if (isCompressed)
	{
		result = StringUtil::gmsec_uncompress(in, uncompressed);

		if (!result.isError())
		{
			content = &uncompressed;
		}
	}

	if (!result.isError())
	{
		result = decode(message, *content);
	}

	return result;
}


Status API3Policy::encode (Message &message, DataBuffer &out)
{
	Status status;

	DataBuffer tmp;

	if (fEncodeXml)
	{
		const char* xml = message.toXML();
		tmp.copy((GMSEC_U8*) xml, StringUtil::stringLength(xml) + 1);
	}
	else if(fEncodeJson)
	{
		const char* json = message.toJSON();
		tmp.copy((GMSEC_U8*) json, StringUtil::stringLength(json) + 1);
	}
	else
	{
		MessageEncoder encoder;

		if (!fEncodeHeader)
		{
			encoder.setSelector(MessageFieldIterator::NON_HEADER_FIELDS);
		}
		try
		{
			encoder.encode(message, tmp);
		}
		catch (const Exception& e)
		{
			status = Status(e);
		}
	}

	if (!status.isError())
	{
		tmp.swap(out);
	}

	return status;
}


Status API3Policy::decode(Message &message, const DataBuffer &encoded)
{
	Status status;

	if (fEncodeXml || fEncodeJson)
	{
		const char* data = (const char*) encoded.get();

		try {
			Message tmpMsg(data);

			tmpMsg.copyFields(message);
		}
		catch (Exception& e) {
			status.set(POLICY_ERROR, e.getErrorCode(), e.getErrorMessage());
		}
	}
	else
	{
		MessageDecoder decoder;

		try
		{
			decoder.decode(message, encoded);
		}
		catch (const Exception& e)
		{
			status = Status(e);
		}
	}

	return status;
}


bool API3Policy::useCompression() const
{
	return fCompress;
}


/*
 * API4Policy.  This maintains backward compatibility with GMSEC API 3
 */

API4Policy::API4Policy ()
	: API3Policy()
{
}


Status API4Policy::initialize(const Config& localConfig)
{
	// Make copy of config
	Config config = localConfig;

	const char* value = config.getValue(GMSEC_VALIDATE_SUBJECT);
	if (!value)
	{
		config.addValue(GMSEC_VALIDATE_SUBJECT, "true");
	}

	value = config.getValue(GMSEC_VALIDATE_SUBJECT_LENIENT);
	if (!value)
	{
		config.addValue(GMSEC_VALIDATE_SUBJECT_LENIENT, "false");
	}

	return API3Policy::initialize(config);
}


/*
 * InvalidPolicy.  Returns failure for all operations.
 */

InvalidPolicy::InvalidPolicy (const Status &status)
	:
	fStatus(status)
{
}


Status InvalidPolicy::initialize (const Config &)
{
	GMSEC_WARNING << "InvalidPolicy: " << fStatus.get();
	return fStatus;
}


bool InvalidPolicy::isValidSubject (const char *)
{
	bool allow = false;
	return allow;
}


bool InvalidPolicy::isValidSubscription (const char *)
{
	bool allow = false;
	return allow;
}


Status InvalidPolicy::authenticate (Connection &)
{
	return fStatus;
}


Status InvalidPolicy::checkSubscribe (const char *)
{
	return fStatus;
}


Status InvalidPolicy::checkSend (const char *)
{
	return fStatus;
}


Status InvalidPolicy::encode (Message &, DataBuffer &)
{
	return fStatus;
}


Status InvalidPolicy::decode (Message &, const DataBuffer &)
{
	return fStatus;
}


Status InvalidPolicy::package (Message &, DataBuffer &, ValueMap &)
{
	return fStatus;
}


Status InvalidPolicy::unpackage (Message &, const DataBuffer &, ValueMap &)
{
	return fStatus;
}


} // namespace secure
} // namespace api
} // namespace gmsec

