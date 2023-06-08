/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_SECURE_POLICY_H
#define GMSEC_API5_SECURE_POLICY_H

#include <gmsec5/ConfigOptions.h>

#include <gmsec5/Config.h>
#include <gmsec5/Status.h>

#include <gmsec5/secure/Access.h>
#include <gmsec5/secure/Cipher.h>
#include <gmsec5/secure/Signer.h>
#include <gmsec5/secure/Random.h>

#include <gmsec5/util/Buffer.h>
#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/wdllexp.h>


namespace gmsec {
namespace api5 {

	// Forward declaration(s)
	class Connection;
	class Message;

namespace secure {

	// Forward declaration(s)
	class AbstractPolicy;


class GMSEC_API Policy
{
public:
	Policy ();


	~Policy ();


	Status CALL_TYPE initialize(const Config& config);


	Access& CALL_TYPE getAccess();
	Cipher& CALL_TYPE getCipher();
	Random& CALL_TYPE getRandom();
	Signer& CALL_TYPE getSigner();


	/**
	 * @method isValidSubject (const char* subject)
	 */
	bool CALL_TYPE isValidSubject(const char* subject);


	/**
	 * @method isValidSubscription(const char* subject)
	 */
	bool CALL_TYPE isValidSubscription(const char* subject);


	// authenticate/checkSubscribe/checkSend use the GMSEC-provided
	// security service. If the middleware-specific version is being
	// employed, they will return

	/**
	 * @method authenticate
	 *
	 * Authenticate the connection.
	 */
	Status CALL_TYPE authenticate(Connection& conn);


	/**
	 * @method checkSubscribe(const char* topic)
	 *
	 * Validate permission to subscribe to subject.
	 * If enabled, this will first validate the subscription.
	 * If access is not middleware controlled, Access.canSubscribe
	 */
	Status CALL_TYPE checkSubscribe(const char* topic);


	/**
	 * @method checkSend (const char* subject);
	 *
	 * Validate permission to send (publish/request/reply) to subject.
	 * If enabled, this will first validate the subject.
	 * If access is not middleware controlled, Access.canSend
	 */
	Status CALL_TYPE checkSend(const char* subject);


	/**
	 * @method encode(const Message& message, DataBuffer& content)
	 */
	Status CALL_TYPE encode(Message& message, DataBuffer& content);


	/**
	 * @method decode(Message& message, const DataBuffer& in)
	 */
	Status CALL_TYPE decode(Message& message, const DataBuffer& in);


	/**
	 * @fn package(const Message& message, DataBuffer& content, ValueMap& meta)
	 *
	 * Applies encoding/compression/encryption/signing according to policy.
	 */
	Status CALL_TYPE package(Message& message, DataBuffer& content, ValueMap& meta);


	/**
	 * @fn unpackage
	 *
	 * Inverts package.
	 */
	Status CALL_TYPE unpackage(Message& message, const DataBuffer& in, ValueMap& meta);


private:
	// Declared, but not implemented.
	Policy(const Policy& other);
	Policy& operator=(const Policy& other);

	friend class AbstractPolicy;

	gmsec::api5::util::StdUniquePtr<AbstractPolicy> policy;
};


// TODO: hide

class GMSEC_API AbstractPolicy
{
public:
	void CALL_TYPE setExternal(Policy &policy);

	virtual ~AbstractPolicy();

	virtual Status CALL_TYPE initialize(const Config& config) = 0;

	virtual Access& CALL_TYPE getAccess();
	virtual Cipher& CALL_TYPE getCipher();
	virtual Random& CALL_TYPE getRandom();
	virtual Signer& CALL_TYPE getSigner();

	virtual bool CALL_TYPE isValidSubject(const char* subject) = 0;
	virtual bool CALL_TYPE isValidSubscription(const char* subject) = 0;

	virtual Status CALL_TYPE authenticate(Connection& conn) = 0;
	virtual Status CALL_TYPE checkSubscribe(const char* subject) = 0;
	virtual Status CALL_TYPE checkSend(const char* subject) = 0;

	virtual Status CALL_TYPE encode(Message& message, DataBuffer& content) = 0;
	virtual Status CALL_TYPE decode(Message& message, const DataBuffer& in) = 0;

	virtual Status CALL_TYPE package(Message& message, DataBuffer& content, ValueMap& meta) = 0;
	virtual Status CALL_TYPE unpackage(Message& message, const DataBuffer& in, ValueMap& meta) = 0;

protected:
	Status CALL_TYPE baseInitialize(const Config& localConfig);

private:
	Config config;

	Access access;
	Cipher cipher;
	Random random;
	Signer signer;
};


class GMSEC_API APIPolicy : public AbstractPolicy
{
public:
	APIPolicy ();

	virtual Status CALL_TYPE initialize(const Config& localConfig);

	virtual bool CALL_TYPE isValidSubject(const char* subject);
	virtual bool CALL_TYPE isValidSubscription(const char* topic);

	virtual Status CALL_TYPE authenticate(Connection& connection);
	virtual Status CALL_TYPE checkSubscribe(const char* topic);
	virtual Status CALL_TYPE checkSend(const char* subject);

	virtual Status CALL_TYPE encode(Message& message, DataBuffer& content);
	virtual Status CALL_TYPE decode(Message& message, const DataBuffer& in);

	virtual Status CALL_TYPE package(Message& message, DataBuffer& content, ValueMap& meta);
	virtual Status CALL_TYPE unpackage(Message& message, const DataBuffer& in, ValueMap& meta);

	bool CALL_TYPE useCompression() const;

private:
	// Declared, but not implemented.
	APIPolicy(const APIPolicy&);
	APIPolicy& operator=(const APIPolicy&);

	Status m_status;
	bool   m_encodeHeader;
	bool   m_encodeXml;
	bool   m_encodeJson;
	bool   m_compress;
};


/*
 * InvalidPolicy that refuses to proceed on any operation.
 */
class GMSEC_API InvalidPolicy : public AbstractPolicy
{
public:
	InvalidPolicy(const Status& status);

	virtual Status CALL_TYPE initialize(const Config& localConfig);

	virtual bool CALL_TYPE isValidSubject(const char* subject);
	virtual bool CALL_TYPE isValidSubscription(const char* topic);

	virtual Status CALL_TYPE authenticate(Connection& connection);
	virtual Status CALL_TYPE checkSubscribe(const char* topic);
	virtual Status CALL_TYPE checkSend(const char* subject);

	virtual Status CALL_TYPE encode(Message& message, DataBuffer& out);
	virtual Status CALL_TYPE decode(Message& message, const DataBuffer& encoded);

	virtual Status CALL_TYPE package(Message& message, DataBuffer& content, ValueMap& meta);
	virtual Status CALL_TYPE unpackage(Message& message, const DataBuffer& in, ValueMap& meta);

private:
	// Declared, but not implemented.
	InvalidPolicy(const InvalidPolicy&);
	InvalidPolicy& operator=(const InvalidPolicy&);

	Status m_status;
};


} // namespace secure
} // namespace api5
} // namespace gmsec


#endif
