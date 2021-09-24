/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API_SECURE_POLICY_H
#define GMSEC_API_SECURE_POLICY_H

#include <gmsec4/ConfigOptions.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/secure/Access.h>
#include <gmsec4/secure/Cipher.h>
#include <gmsec4/secure/Signer.h>
#include <gmsec4/secure/Random.h>

#include <gmsec4/util/Buffer.h>
#include <gmsec4/util/wdllexp.h>


namespace gmsec {
namespace api {

class Connection;
class Message;

namespace secure {


class AbstractPolicy;


class GMSEC_API Policy
{
public:

	/**
	parameters:
		SEC-AUTH
		SEC-AUTH-ID
		SEC-AUTH-KEY

		SEC-AUTH-<PARAM>
		SEC-AUTH-KEYDIR?

		SEC-CIPHER
		SEC-CIPHER-<PARAM>

		SEC-SIGNER
		SEC-SIGNER-<PARAM>
	*/

	Policy ();
	~Policy ();

	Status initialize (const Config &config);

	Access &getAccess ();
	Cipher &getCipher ();
	Random &getRandom ();
	Signer &getSigner ();


	/** @method isValidSubject (const char *subject)
	*/
	bool isValidSubject (const char *subject);

	/** @method isValidSubscription (const char *subject)
	*/
	bool isValidSubscription (const char *subject);


	// authenticate/checkSubscribe/checkSend use the GMSEC-provided
	// security service.  If the middleware-specific version is being
	// employed, they will return

	/** @method authenticate
		Authenticate the connection.
	*/
	Status authenticate (Connection &conn);

	/** @method checkSubscribe
		Validate permission to subscribe to subject.
		If enabled, this will first validate the subscription.
		If access is not middleware controlled, Access.canSubscribe
	*/
	Status checkSubscribe (const char *subject);

	/** @method checkSend (const char *subject);
		Validate permission to send (publish/request/reply) to subject.
		If enabled, this will first validate the subject.
		If access is not middleware controlled, Access.canSend
	*/
	Status checkSend (const char *subject);

	/** @fn encode
	 */
	Status encode (Message &message, DataBuffer &content);

	/** @fn decode(...)
	 */
	Status decode(Message &message, const DataBuffer &in);

	/** @fn package 
	 * Applies encoding/compression/encryption/signing according to policy.
	 */
	Status package (Message &message, DataBuffer &content, ValueMap &meta);

	/** @fn unpackage(...)
	 * Inverts package.
	 */
	Status unpackage (Message &message, const DataBuffer &in, ValueMap &meta);


private:

	// Declared, but not implemented.
	Policy (const Policy &other);
	Policy &operator= (const Policy &other);

	friend class AbstractPolicy;

	AbstractPolicy * ptr;

};


// TODO: hide

class GMSEC_API AbstractPolicy
{
public:

	void setExternal (Policy &policy);

	virtual ~AbstractPolicy();

	virtual Status initialize (const Config &config) = 0;

	virtual Access &getAccess ();
	virtual Cipher &getCipher ();
	virtual Random &getRandom ();
	virtual Signer &getSigner ();

	virtual bool isValidSubject (const char *subject) = 0;
	virtual bool isValidSubscription (const char *subject) = 0;

	virtual Status authenticate (Connection &conn) = 0;
	virtual Status checkSubscribe (const char *subject) = 0;
	virtual Status checkSend (const char *subject) = 0;

	virtual Status encode (Message &message, DataBuffer &content) = 0;
	virtual Status decode(Message &message, const DataBuffer &in) = 0;

	virtual Status package (Message &message, DataBuffer &content, ValueMap &meta) = 0;
	virtual Status unpackage (Message &message, const DataBuffer &in, ValueMap &meta) = 0;

protected:

	Status baseInitialize (const Config &localConfig);

private:

	Config config;

	Access access;
	Cipher cipher;
	Random random;
	Signer signer;

};


class GMSEC_API API3Policy : public AbstractPolicy
{
public:

	API3Policy ();

	virtual Status initialize (const Config &localConfig);

	virtual bool isValidSubject (const char *subject);
	virtual bool isValidSubscription (const char *subject);

	virtual Status authenticate (Connection &connection);
	virtual Status checkSubscribe (const char *subject);
	virtual Status checkSend (const char *subject);

	virtual Status encode (Message &message, DataBuffer &content);
	virtual Status decode(Message &message, const DataBuffer &in);

	virtual Status package (Message &message, DataBuffer &content, ValueMap &meta);
	virtual Status unpackage (Message &message, const DataBuffer &in, ValueMap &meta);

	bool useCompression() const;

private:

	// Declared, but not implemented.
	API3Policy(const API3Policy &);
	API3Policy &operator=(const API3Policy &);

	Status fStatus;
	bool fValidateSubjects;
	bool fLenientSubjects;
	bool fEncodeHeader;
	bool fEncodeXml;
	bool fEncodeJson;
	bool fCompress;

};


class GMSEC_API API4Policy : public API3Policy
{
public:

	API4Policy ();

	virtual Status initialize (const Config &localConfig);

private:
	// Declared, but not implemented.
	API4Policy(const API4Policy&);
	API4Policy& operator=(const API4Policy&);
};


/*
	Refuses to proceed on any operation.
*/
class GMSEC_API InvalidPolicy : public AbstractPolicy
{
public:

	InvalidPolicy (const Status &status);

	virtual Status initialize (const Config &localConfig);

	virtual bool isValidSubject (const char *subject);
	virtual bool isValidSubscription (const char *subject);

	virtual Status authenticate (Connection &connection);
	virtual Status checkSubscribe (const char *subject);
	virtual Status checkSend (const char *subject);

	virtual Status encode (Message &message, DataBuffer &out);
	virtual Status decode (Message &message, const DataBuffer &encoded);

	virtual Status package (Message &message, DataBuffer &content, ValueMap &meta);
	virtual Status unpackage (Message &message, const DataBuffer &in, ValueMap &meta);

private:

	// Declared, but not implemented.
	InvalidPolicy(const InvalidPolicy &);
	InvalidPolicy &operator=(const InvalidPolicy &);

	Status fStatus;

};


} // namespace secure
} // namespace api
} // namespace gmsec


#endif

