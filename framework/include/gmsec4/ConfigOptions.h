/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


//
// Note: This file is best viewed if tabstop is set to 8 (spaces).
//

#ifndef GMSEC_CONFIG_OPTIONS_H
#define GMSEC_CONFIG_OPTIONS_H

namespace gmsec{
namespace api{

const char* const GMSEC_MIDDLEWARE_ID			= "MW-ID";
const char* const GMSEC_CONN_TYPE			= "ConnectionType";

const char* const GMSEC_MSG_CONTENT_VALIDATE		= "GMSEC-MSG-CONTENT-VALIDATE";      //deprecated
const char* const GMSEC_MSG_CONTENT_VALIDATE_ALL	= "GMSEC-MSG-CONTENT-VALIDATE-ALL";
const char* const GMSEC_MSG_CONTENT_VALIDATE_SEND	= "GMSEC-MSG-CONTENT-VALIDATE-SEND";
const char* const GMSEC_MSG_CONTENT_VALIDATE_RECV	= "GMSEC-MSG-CONTENT-VALIDATE-RECV";
const char* const GMSEC_MESSAGE_SPEC_VERSION		= "GMSEC-SPECIFICATION-VERSION";
const char* const GMSEC_SCHEMA_PATH			= "GMSEC-SCHEMA-PATH";
const char* const GMSEC_VALIDATION_LEVEL		= "GMSEC-VALIDATION-LEVEL";
const char* const GMSEC_SCHEMA_LEVEL			= "GMSEC-SCHEMA-LEVEL";
const char* const GMSEC_LEGACY_SCHEMA_FILES		= "GMSEC-LEGACY-SCHEMA-FILES";


const char* const LOG_LEVEL				= "LOGLEVEL";
const char* const LOG_FILE				= "LOGFILE";
const char* const LOG_STDOUT				= "STDOUT";
const char* const LOG_STDERR				= "STDERR";


const char* const GMSEC_TRACKING			= "TRACKING";
const char* const GMSEC_TRACKING_NODE			= "TRACKING-NODE";
const char* const GMSEC_TRACKING_PROCESS_ID		= "TRACKING-PROCESS-ID";
const char* const GMSEC_TRACKING_USERNAME		= "TRACKING-USER-NAME";
const char* const GMSEC_TRACKING_CONNECTION_ID		= "TRACKING-CONNECTION-ID";
const char* const GMSEC_TRACKING_PUBLISH_TIME		= "TRACKING-PUBLISH-TIME";
const char* const GMSEC_TRACKING_UNIQUE_ID		= "TRACKING-UNIQUE-ID";
const char* const GMSEC_TRACKING_MW_INFO		= "TRACKING-MW-INFO";
const char* const GMSEC_TRACKING_ACTIVE_SUBSCRIPTIONS	= "TRACKING-ACTIVE-SUBSCRIPTIONS";
const char* const GMSEC_TRACKING_CONNECTION_ENDPOINT	= "TRACKING-CONNECTION-ENDPOINT";

const char* const GMSEC_REPLY_SUBJECT         		= "MW-REPLY-STRING";
const char* const GMSEC_EXPOSE_RESP            		= "MW-EXPOSE-RESP";
const char* const OPT_MULTI_RESP               		= "MW-MULTI-RESP";
const char* const GMSEC_REQ_RESP_BEHAVIOR      		= "GMSEC-REQ-RESP";
const char* const GMSEC_REQ_REPUBLISH_MS		= "REPUBLISH-MS";
const char* const GMSEC_REPLY_SUBJECT_FIELD    		= "GMSEC_REPLY_SUBJECT";
const char* const GMSEC_DISABLE_RR             		= "GMSEC-DISABLE-RR";

const char* const GMSEC_LOG_PERFORMANCE        		= "GMSEC-LOG-PERFORMANCE";

const char* const OPT_MAX_CONN_RETRIES              	= "MW-MAX-CONN-RETRIES";
const char* const OPT_CONN_RETRY_INTERVAL           	= "MW-CONN-RETRY-INTERVAL";

const char* const GMSEC_ASYNC_PUBLISH               	= "GMSEC-ASYNC-PUBLISH";
const char* const GMSEC_ASYNC_PUBLISH_QUEUE_DEPTH   	= "GMSEC-ASYNC-PUBLISH-QUEUE-DEPTH";
const char* const GMSEC_ASYNC_PUBLISH_TEARDOWN_WAIT	= "GMSEC-ASYNC-PUBLISH-TEARDOWN-WAIT";

const char* const GMSEC_USE_MSG_BINS           		= "GMSEC-USE-MSG-BINS";
const char* const GMSEC_MSG_BIN_SIZE           		= "GMSEC-MSG-BIN-SIZE";
const char* const GMSEC_MSG_BIN_TIMEOUT        		= "GMSEC-MSG-BIN-TIMEOUT";

const char* const GMSEC_MSG_FLD_STORAGE_TYPE		= "GMSEC-MSGFLD-STORE-TYPE";
const char* const GMSEC_MSG_FLD_STORAGE_SIZE		= "GMSEC-MSGFLD-STORE-SIZE";
const char* const GMSEC_SORT_MSG_FIELDS			= "GMSEC-SORT-MSG-FIELDS";
const char* const GMSEC_CONVERT_FIELD_NAMES		= "GMSEC-CONVERT-FIELD-NAMES";


namespace secure
{
const char* const GMSEC_RANDOM_PATH		        = "SEC-RANDOM-PATH";
const char* const GMSEC_CIPHER_DELTA		        = "SEC-CIPHER-DELTA";
const char* const GMSEC_RANDOM_SEED_NUMBER	        = "SEC-RANDOM-SEED-NUMBER";
const char* const GMSEC_RANDOM_SEED_STRING	        = "SEC-RANDOM-SEED-STRING";
const char* const GMSEC_RANDOM_SEED_CRYPT	        = "SEC-RANDOM-SEED-CRYPT";
const char* const GMSEC_VALIDATE_SUBJECT	        = "SEC-VAL-SUB";
const char* const GMSEC_VALIDATE_SUBJECT_LENIENT	= "SEC-VAL-SUB-LENIENT";
const char* const GMSEC_POLICY	                        = "SEC-POLICY";
const char* const GMSEC_POLICY_FULL_PATH	        = "SEC-POLICY-PATH-QUALIFIED";
const char* const GMSEC_POLICY_ACCESS	                = "SEC-AUTH";
const char* const GMSEC_POLICY_CIPHER	                = "SEC-CIPHER";
const char* const GMSEC_POLICY_SIGNER	                = "SEC-SIGNER";

const char* const POLICY_RANDOM      			= "SEC-RANDOM";
const char* const FIELD_CONTENT      			= "SEC-CONTENT";

const char* const ENCODE_HEADER              		= "SEC-ENCODE-HEADER"; 
const char* const ENCODE_XML                 		= "SEC-ENCODE-XML"; 

const char* const GMSEC_ENCODE_XML           		= "GMSEC-ENCODE-XML";
const char* const GMSEC_ENCODE_JSON          		= "GMSEC-ENCODE-JSON";
	
const char* const POL_COMPRESS 				= "POL-COMPRESS";   

const char* const PROP_EMBED 				= "SEC-EMBED";
const char* const C2_AUTH_NAME         			= "SEC-AUTH-NAME";
const char* const AUTH_KEY           			= "SEC-AUTH-KEY";
const char* const C2_AUTH_TOPIC        			= "SEC-AUTH-TOPIC";
const char* const DEFAULT_AUTH_TOPIC 			= "GMSEC.AUTH.CONN";
const char* const AUTH_NONE          			= "SEC-AUTH-NONE";
const char* const PRESERVE_DIGEST    			= "SEC-PRESERVE-DIGEST";
const char* const DIGEST_NONE        			= "SEC-DIGEST-NONE";
const char* const C2_SIGNER_KEY        			= "SEC-SIGNER-KEY";
const char* const C2_COMMON_KEY        			= "SEC-COMMON-KEY";
const char* const ACCEPT_ENCRYPTED   			= "SEC-ACCEPT-ENCRYPTED";
const char* const FIELD_ENCRYPT      			= "SEC-ENCRYPT";
const char* const C2_KEY_ID	       			= "SEC-KEY-ID";
const char* const FIELD_DIGEST       			= "SEC-DIGEST";
const char* const FIELD_SIGNER       			= "SEC-SIGNER";
const char* const EMBED_MESSAGE      			= "SEC-EMBED-MESSAGE";
const char* const EXPOSE_HEADER      			= "SEC-EXPOSE-HEADER";
const char* const C2_TEST_KEY_ID			= "SEC-TEST-KEY-ID";

const char* const GMSEC_VAL_DIGEST			= "SEC-VAL-DIGEST";
const char* const GMSEC_TEST_DEBUG_PACKAGING		= "SEC-TEST-DEBUG-PACKAGING";
const char* const GMSEC_TEST_DEBUG_ENCODING		= "SEC-TEST-DEBUG-ENCODING";
const char* const GMSEC_RANDOM_CRYPT			= "SEC-RANDOM-CRYPT";

const char* const C2_NSS_DIR           			= "SEC-NSS-DIR";
const char* const C2_NSS_KEY           			= "SEC-NSS-KEY";
const char* const C2_SYM_PREFIX     			= "SEC-NSS-SYM-PREFIX";
const char* const NSS_NONE           			= "SEC-NSS-NONE";

}   // end namespace secure


//MB
const char* const MB_MAX_MSG_LIFE			= "GMSEC-MB-MAX_MSG_LIFE.s";
const char* const MB_LOOPBACK				= "loopback";
const char* const MB_AUTOSTART				= "autostart";
const char* const MB_COMPRESS				= "compress";
const char* const MB_EXITABLE				= "exitable";


//ActiveMQ
const char* const AMQ_DURABLE_CLIENT_ID			= "durable-client-id";
const char* const AMQ_DURABLE_MSG_SELECTOR	        = "durable-message-selector";
const char* const AMQ_DURABLE_PRIORITY      	        = "durable-priority";
const char* const AMQ_DURABLE_TTL		        = "durable-time-to-live";
const char* const AMQ_DURABLE_SUBSCRIBE			= "durable-subscribe";
const char* const AMQ_DURABLE_PUBLISH			= "durable-publish";


//WebSphere
const char* const OPT_MAX_TOPIC_HANDLES              	= "MW-MAX-TOPIC-HANDLES";
const char* const OPT_ASYNC_PUBLISH                  	= "MW-ASYNC-PUBLISH";
const char* const OPT_ASYNC_STATUS_CHECK             	= "MW-ASYNC-STATUS-CHECK";
const char* const OPT_ASYNC_STATUS_CHECK_MESSAGE_INTERVAL = "MW-ASYNC-STATUS-CHECK-MESSAGE-INTERVAL";
const char* const OPT_MESSAGE_WAIT_PERIOD               = "MW-MESSAGE-WAIT-PERIOD";


//ZeroMQ
const char* const ZEROMQ_SOCKET_SETTLETIME    		= "SOCKET-SETTLETIME";
const char* const ZEROMQ_SUBSCRIBE_ENDPOINT   		= "MW-SUB-ENDPOINT";
const char* const ZEROMQ_PUBLISH_ENDPOINT     		= "MW-PUB-ENDPOINT";
const char* const ZEROMQ_REPLYLISTEN_ENDPOINT 		= "MW-REPLISTEN-ENDPOINT";

const char* const BROKER_USAGE        			= "ZMQBroker publish-endpoint=<protocol>://<url>:<port> subscribe-endpoint=<protocol>://<url>:<port>";
const char* const ZEROMQ_PUB_ENDPOINT 			= "publish-endpoint";
const char* const ZEROMQ_SUB_ENDPOINT 			= "subscribe-endpoint";


//BoltConnection
const char* const BOLT_HIDE_MWGET			= "MW-HIDE-MWGET";
const char* const BOLT_LOG_FILTER			= "MW-LOG-FILTER";


//Generic JMS
const char* const JMS_PERSISTENT			= "MW-PERSISTENT";
const char* const JMS_JVM_IGNORE_UNRECOGNIZED		= "MW-JVM-IGNORE-UNRECOGNIZED";

}   // end namespace api
}   // end namespace gmsec

#endif 
