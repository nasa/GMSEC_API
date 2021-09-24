/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_secure_Policy_h
#define gmsec_secure_Policy_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>

#include <gmsec/secure/Access.h>
#include <gmsec/secure/Cipher.h>
#include <gmsec/secure/Signer.h>
#include <gmsec/secure/Random.h>


namespace gmsec {

class Connection;

namespace secure {

using gmsec::Status;
using gmsec::Config;
using gmsec::Connection;


const char SEC_POLICY[]		= "SEC-POLICY";
const char POLICY_ACCESS[]	= "SEC-AUTH";
const char POLICY_CIPHER[]	= "SEC-CIPHER";
const char POLICY_SIGNER[]	= "SEC-SIGNER";
const char POLICY_RANDOM[]	= "SEC-RANDOM";

const char FULLY_QUALIFIED_POLICY[]	= "SEC-POLICY-PATH-QUALIFIED";

const char FIELD_CONTENT[]	= "SEC-CONTENT";

const char VAL_SUB[]			= "SEC-VAL-SUB";	// validate topics?
const char VAL_SUB_LENIENT[]	= "SEC-VAL-SUB-LENIENT"; // go easy on 'em ?
const char ENCODE_HEADER[]		= "SEC-ENCODE-HEADER"; // encode header fields?
const char ENCODE_XML[]			= "SEC-ENCODE-XML"; //MEH - legacy parameter for encode as xml

const char GMSEC_ENCODE_XML[]		= "GMSEC-ENCODE-XML";
const char GMSEC_ENCODE_JSON[]		= "GMSEC-ENCODE-JSON";

const char POL_COMPRESS[]   = "POL-COMPRESS";   // use data compression?



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

	GMSEC_DEPRECATED Policy ();
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

	GMSEC_DEPRECATED API3Policy ();

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


/*
	Refuses to proceed on any operation.
*/
class GMSEC_API InvalidPolicy : public AbstractPolicy
{
public:

	GMSEC_DEPRECATED InvalidPolicy (const Status &status);

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
} // namespace gmsec


#endif /* gmsec_secure_Policy_h */

