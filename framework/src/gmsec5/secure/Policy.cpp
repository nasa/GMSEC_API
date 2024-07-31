/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <gmsec5/secure/Policy.h>

#include <gmsec5/internal/DynamicFactory.h>
#include <gmsec5/internal/Encoder.h>
#include <gmsec5/internal/MessageBuddy.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/Subject.h>

#include <gmsec5/util/Log.h>


// Define DEFAULT_POLICY in config/default for a different policy.
#ifndef DEFAULT_POLICY
	#define DEFAULT_POLICY APIPolicy
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::secure;
using namespace gmsec::api5::util;


Policy::Policy()
	: policy(new DEFAULT_POLICY())
{
}


Policy::~Policy()
{
}


Status Policy::initialize(const Config& config)
{
	return policy->initialize(config);
}


Access& Policy::getAccess()
{
	return policy->getAccess();
}


Signer& Policy::getSigner()
{
	return policy->getSigner();
}


Cipher& Policy::getCipher()
{
	return policy->getCipher();
}


Random& Policy::getRandom()
{
	return policy->getRandom();
}



bool Policy::isValidSubject(const char* subject)
{
	return policy->isValidSubject(subject);
}


bool Policy::isValidSubscription(const char* topic)
{
	return policy->isValidSubscription(topic);
}


Status Policy::authenticate(Connection& connection)
{
	return policy->authenticate(connection);
}


Status Policy::checkSubscribe(const char* topic)
{
	return policy->checkSubscribe(topic);
}


Status Policy::checkSend(const char* subject)
{
	return policy->checkSend(subject);
}


Status Policy::encode(Message& message, DataBuffer& content)
{
	return policy->encode(message, content);
}


Status Policy::decode(Message& message, const DataBuffer& in)
{
	return policy->decode(message, in);
}


Status Policy::package(Message& message, DataBuffer& content, ValueMap& meta)
{
	return policy->package(message, content, meta);
}


Status Policy::unpackage(Message& message, const DataBuffer& in, ValueMap& meta)
{
	return policy->unpackage(message, in, meta);
}


/*
 * AbstractPolicy.  Base Policy implementation.
 */

void AbstractPolicy::setExternal(Policy& policy)
{
	policy.policy.reset(this);
}


AbstractPolicy::~AbstractPolicy()
{
}


Access& AbstractPolicy::getAccess()
{
	return access;
}


Cipher& AbstractPolicy::getCipher()
{
	return cipher;
}


Random& AbstractPolicy::getRandom()
{
	return random;
}


Signer& AbstractPolicy::getSigner()
{
	return signer;
}


Status AbstractPolicy::baseInitialize(const Config& localConfig)
{
	Status status;

	// I see a pattern here...
	status = DynamicFactory::initialize(getAccess(), localConfig);
	if (status.hasError())
		return status;

	status = DynamicFactory::initialize(getCipher(), localConfig);
	if (status.hasError())
		return status;

	status = DynamicFactory::initialize(getRandom(), localConfig);
	if (status.hasError())
		return status;

	status = DynamicFactory::initialize(getSigner(), localConfig);
	if (status.hasError())
		return status;

	return status;
}


/*
 * APIPolicy.
 */

APIPolicy::APIPolicy()
	: m_encodeHeader(true),
	  m_encodeXml(false),
	  m_encodeJson(false),
	  m_compress(false)
{
}


Status APIPolicy::initialize(const Config& localConfig)
{
	m_encodeHeader = localConfig.getBooleanValue(C2_ENCODE_HEADER, true);

	//MEH: ER 3924 - keep SEC-ENCODE-XML as an option, even though it hasn't been
	//documented, but normalize documented syntax to GMSEC-ENCODE-XML to conform with
	//JSON's GMSEC-ENCODE-JSON
	m_encodeXml = localConfig.getBooleanValue(C2_ENCODE_XML, false) || 
	              localConfig.getBooleanValue(GMSEC_ENCODE_XML, false);

	m_encodeJson = localConfig.getBooleanValue(GMSEC_ENCODE_JSON, false);

	if (m_encodeXml && m_encodeJson)
	{
		m_status.set(POLICY_ERROR, INVALID_CONFIG,
					 "Cannot specify both XML and JSON encoding. XML encoding will take priority.");
	}

	m_compress = localConfig.getBooleanValue(POL_COMPRESS, false);

	return m_status;
}


bool APIPolicy::isValidSubject(const char* subject)
{
	return Subject::isValid(subject, false).empty();
}


bool APIPolicy::isValidSubscription(const char* topic)
{
	return Subject::isValidSubscription(topic, false).empty();
}


Status APIPolicy::authenticate(Connection&)
{
	return Status();
}


Status APIPolicy::checkSubscribe(const char*)
{
	return Status();
}


Status APIPolicy::checkSend(const char*)
{
	return Status();
}


Status APIPolicy::package(Message& message, DataBuffer& content, ValueMap& meta)
{
	Status result = encode(message, content);

	if (!result.hasError() && useCompression())
	{
		DataBuffer compressed;

		result = StringUtil::gmsec_compress(content, compressed);

		if (!result.hasError())
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


Status APIPolicy::unpackage(Message& message, const DataBuffer& in, ValueMap& meta)
{
	Status result;

	const DataBuffer* content = &in;
	DataBuffer uncompressed;

	bool isCompressed = false;
	meta.getBoolean("ZIP", isCompressed, &isCompressed);

	if (isCompressed)
	{
		result = StringUtil::gmsec_uncompress(in, uncompressed);

		if (!result.hasError())
		{
			content = &uncompressed;
		}
	}

	if (!result.hasError())
	{
		result = decode(message, *content);
	}

	if (!result.hasError() && isCompressed)
	{
		message.clearField("ZIP");
	}

	return result;
}


Status APIPolicy::encode(Message& message, DataBuffer& out)
{
	Status status;

	DataBuffer tmp;

	if (m_encodeXml)
	{
		const char* xml = message.toXML();
		tmp.copy(reinterpret_cast<const GMSEC_U8*>(xml), StringUtil::stringLength(xml) + 1);
	}
	else if(m_encodeJson)
	{
		const char* json = message.toJSON();
		tmp.copy(reinterpret_cast<const GMSEC_U8*>(json), StringUtil::stringLength(json) + 1);
	}
	else
	{
		MessageEncoder encoder;

		try
		{
			encoder.encode(message, tmp);
		}
		catch (const GmsecException& e)
		{
			status = Status(e);
		}
	}

	if (!status.hasError())
	{
		tmp.swap(out);
	}

	return status;
}


Status APIPolicy::decode(Message& message, const DataBuffer& encoded)
{
	Status status;

	if (m_encodeXml || m_encodeJson)
	{
		const char* data = reinterpret_cast<const char*>(encoded.get());

		if (data == NULL || *data == '\0')
		{
			status.set(POLICY_ERROR, NO_MSG_AVAILABLE, "No XML or JSON data associated with message");
		}
		else
		{
			try {
				MessageBuddy::getInternal(message).processData(data, (m_encodeXml ? DataType::XML_DATA : DataType::JSON_DATA));
			}
			catch (const GmsecException& e) {
				status.set(POLICY_ERROR, e.getErrorCode(), e.getErrorMessage());
			}
		}
	}
	else
	{
		MessageDecoder decoder;

		try {
			decoder.decode(message, encoded);
		}
		catch (const GmsecException& e) {
			status = Status(e);
		}
	}

	return status;
}


bool APIPolicy::useCompression() const
{
	return m_compress;
}


/*
 * InvalidPolicy. Returns failure for all operations.
 */

InvalidPolicy::InvalidPolicy(const Status& status)
	: m_status(status)
{
}


Status InvalidPolicy::initialize(const Config&)
{
	GMSEC_WARNING << "InvalidPolicy: " << m_status.get();
	return m_status;
}


bool InvalidPolicy::isValidSubject(const char *)
{
	return false;
}


bool InvalidPolicy::isValidSubscription(const char*)
{
	return false;
}


Status InvalidPolicy::authenticate(Connection&)
{
	return m_status;
}


Status InvalidPolicy::checkSubscribe(const char*)
{
	return m_status;
}


Status InvalidPolicy::checkSend(const char*)
{
	return m_status;
}


Status InvalidPolicy::encode(Message&, DataBuffer&)
{
	return m_status;
}


Status InvalidPolicy::decode(Message&, const DataBuffer&)
{
	return m_status;
}


Status InvalidPolicy::package(Message&, DataBuffer&, ValueMap&)
{
	return m_status;
}


Status InvalidPolicy::unpackage(Message&, const DataBuffer&, ValueMap&)
{
	return m_status;
}
