/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


//
// Note: This file is best viewed if tabstop is set to 8 (spaces).
//

#ifndef GMSEC_API5_CONFIG_OPTIONS_H
#define GMSEC_API5_CONFIG_OPTIONS_H

namespace gmsec
{
namespace api5
{

const char* const GMSEC_MIDDLEWARE_ID			= "mw-id";
const char* const GMSEC_MIDDLEWARE_SERVER               = "mw-server";

const char* const GMSEC_CONN_TYPE			= "ConnectionType";                   // deprecated -- use mw-id
const char* const GMSEC_SERVER                          = "server";                           // deprecated -- use mw-server

const char* const GMSEC_MSG_CONTENT_VALIDATE		= "GMSEC-MSG-CONTENT-VALIDATE";       // synonymous with GMSEC-MSG-CONTENT-VALIDATE-SEND
const char* const GMSEC_MSG_CONTENT_VALIDATE_ALL	= "GMSEC-MSG-CONTENT-VALIDATE-ALL";
const char* const GMSEC_MSG_CONTENT_VALIDATE_SEND	= "GMSEC-MSG-CONTENT-VALIDATE-SEND";
const char* const GMSEC_MSG_CONTENT_VALIDATE_RECV	= "GMSEC-MSG-CONTENT-VALIDATE-RECV";
const char* const GMSEC_MESSAGE_SPEC_VERSION		= "GMSEC-SPECIFICATION-VERSION";
const char* const GMSEC_SCHEMA_PATH			= "GMSEC-SCHEMA-PATH";
const char* const GMSEC_SCHEMA_LEVEL			= "GMSEC-SCHEMA-LEVEL";
const char* const GMSEC_LEGACY_SCHEMA_FILES		= "GMSEC-LEGACY-SCHEMA-FILES";


const char* const LOG_LEVEL				= "LOGLEVEL";
const char* const LOG_FILE				= "LOGFILE";
const char* const LOG_STDOUT				= "STDOUT";
const char* const LOG_STDERR				= "STDERR";


const char* const GMSEC_REMOVE_TRACKING_FIELDS          = "GMSEC-REMOVE-TRACKING-FIELDS";
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

const char* const OPT_MULTI_RESP               		= "MW-MULTI-RESP";
const char* const OPT_SUBSCRIBE_FOR_RESPONSE		= "MW-SUBSCRIBE-FOR-RESP";
const char* const GMSEC_REQ_REPUBLISH_MS		= "MW-REPUBLISH-MS";

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
const char* const GMSEC_SHOW_MSG_CONFIG			= "GMSEC-SHOW-MSG-CONFIG";
const char* const GMSEC_IDENTIFY_HEADER_FIELDS		= "GMSEC-IDENTIFY-HEADER-FIELDS";


namespace secure
{
const char* const GMSEC_RANDOM_PATH		        = "SEC-RANDOM-PATH";           //Not documented
const char* const GMSEC_CIPHER_DELTA		        = "SEC-CIPHER-DELTA";          //Not documented
const char* const GMSEC_RANDOM_SEED_NUMBER	        = "SEC-RANDOM-SEED-NUMBER";    //Not documented
const char* const GMSEC_RANDOM_SEED_STRING	        = "SEC-RANDOM-SEED-STRING";    //Not documented
const char* const GMSEC_RANDOM_SEED_CRYPT	        = "SEC-RANDOM-SEED-CRYPT";     //Not documented
const char* const GMSEC_POLICY	                        = "SEC-POLICY";
const char* const GMSEC_POLICY_FULL_PATH	        = "SEC-POLICY-PATH-QUALIFIED"; //Not documented
const char* const GMSEC_POLICY_ACCESS	                = "SEC-AUTH";
const char* const GMSEC_POLICY_CIPHER	                = "SEC-CIPHER";
const char* const GMSEC_POLICY_SIGNER	                = "SEC-SIGNER";

const char* const POLICY_RANDOM      			= "SEC-RANDOM";                //Not documented

const char* const GMSEC_ENCODE_XML           		= "GMSEC-ENCODE-XML";          //Default is false
const char* const GMSEC_ENCODE_JSON          		= "GMSEC-ENCODE-JSON";         //Default is false
	
const char* const POL_COMPRESS 				= "POL-COMPRESS";              //Default is false

const char* const C2_ENCODE_HEADER             		= "SEC-ENCODE-HEADER";         //Default is true. Not documented 
const char* const C2_ENCODE_XML                		= "SEC-ENCODE-XML";            //Default is false. Not documented 

const char* const C2_AUTH_NAME         			= "SEC-AUTH-NAME";
const char* const C2_AUTH_TOPIC        			= "SEC-AUTH-TOPIC";
const char* const C2_SEC_ENCRYPT			= "SEC-ENCRYPT";
const char* const C2_PRESERVE_DIGEST    		= "SEC-PRESERVE-DIGEST";       //Default is false
const char* const C2_KEY_ID	       			= "SEC-KEY-ID";
const char* const C2_EXPOSE_HEADER      		= "SEC-EXPOSE-HEADER";         //Default is true; not documented

const char* const C2_VAL_DIGEST				= "SEC-VAL-DIGEST";            //Default is false; not documented
const char* const C2_RANDOM_CRYPT			= "SEC-RANDOM-CRYPT";          //Default is false; not documented

const char* const C2_NSS_NONE          			= "SEC-NSS-NONE";              //Default is false
const char* const C2_AUTH_NONE         			= "SEC-AUTH-NONE";             //Default is true
const char* const C2_DIGEST_NONE       			= "SEC-DIGEST-NONE";           //Default is true; not documented
const char* const C2_NSS_DIR           			= "SEC-NSS-DIR";
const char* const C2_NSS_KEY           			= "SEC-NSS-KEY";
const char* const C2_COMMON_KEY        			= "SEC-COMMON-KEY";            //Not documented; alternate for SEC-NSS-KEY?
const char* const C2_SYM_PREFIX     			= "SEC-NSS-SYM-PREFIX";
const char* const C2_SIGNER_KEY        			= "SEC-SIGNER-KEY";
const char* const C2_ACCEPT_ENCRYPTED   		= "SEC-ACCEPT-ENCRYPTED";      //Default is false

const char* const C2_TEST_KEY_ID			= "SEC-TEST-KEY-ID";           //Not documented. For internal use only
const char* const C2_TEST_DEBUG_PACKAGING		= "SEC-TEST-DEBUG-PACKAGING";  //Default is false. Not documented. For internal use only
const char* const C2_TEST_DEBUG_ENCODING		= "SEC-TEST-DEBUG-ENCODING";   //Default is false. Not documented. For internal use only

}   // end namespace secure


//MB
const char* const MB_MAX_MSG_LIFE			= "MW-GMSEC-MB-MAX_MSG_LIFE.s";
const char* const MB_LOOPBACK				= "mw-loopback";
const char* const MB_AUTOSTART				= "mw-autostart";
const char* const MB_COMPRESS				= "mw-compress";
const char* const MB_EXITABLE				= "mw-exitable";


//ActiveMQ/Artemis
const char* const AMQ_CONNECTION_CLIENT_ID		= "mw-connection-client-id";
const char* const AMQ_DURABLE_CLIENT_ID			= "mw-durable-client-id";
const char* const AMQ_DURABLE_MSG_SELECTOR	        = "mw-durable-message-selector";
const char* const AMQ_DURABLE_PRIORITY      	        = "mw-durable-priority";
const char* const AMQ_DURABLE_TTL		        = "mw-durable-time-to-live";
const char* const AMQ_DURABLE_SUBSCRIBE			= "mw-durable-subscribe";
const char* const AMQ_DURABLE_PUBLISH			= "mw-durable-publish";
const char* const AMQ_FILTER_DUPLICATE_MSGS		= "mw-filter-dups";
const char* const AMQ_USERNAME				= "mw-username";
const char* const AMQ_PASSWORD				= "mw-password";
const char* const AMQ_SSL_KEYSTORE			= "mw-keystore";
const char* const AMQ_SSL_KEYSTORE_PW			= "mw-keystore-password";
const char* const AMQ_SSL_TRUSTSTORE			= "mw-truststore";
const char* const AMQ_SSL_TRUSTSTORE_PW			= "mw-truststore-password";
const char* const AMQ_ACK_MODE				= "mw-ack-mode";


//IBM MQ
const char* const IBM_CHANNEL				= "mw-channel";
const char* const IBM_QUEUE_MANAGER			= "mw-queuemanager";
const char* const IBM_KEYSTORE				= "mw-keystore";
const char* const IBM_CIPHER				= "mw-cipher";
const char* const IBM_FILTER_DUPLICATE_MSGS		= "mw-filter-dups";
const char* const IBM_ASYNC_PUBLISH                  	= "mw-async-publish";
const char* const IBM_ASYNC_STATUS_CHECK             	= "mw-async-status-check";
const char* const IBM_ASYNC_STATUS_CHECK_MSG_INTERVAL	= "mw-async-status-check-message-interval";
const char* const IBM_MESSAGE_WAIT_PERIOD               = "mw-message-wait-period";
const char* const IBM_MQINQMP_SPACE			= "mw-mqinqmp-space";
const char* const IBM_MAX_TOPIC_HANDLES              	= "mw-max-topic-handles";
const char* const IBM_DEBUG_PROP			= "mw-debug-prop";


// Apache Kafka
const char* const KAFKA_GROUP_ID			= "mw-group-id";


//ZeroMQ
const char* const ZEROMQ_SOCKET_SETTLETIME    		= "MW-SOCKET-SETTLETIME";
const char* const ZEROMQ_SUBSCRIBE_ENDPOINT   		= "MW-SUB-ENDPOINT";
const char* const ZEROMQ_PUBLISH_ENDPOINT     		= "MW-PUB-ENDPOINT";
const char* const ZEROMQ_REPLYLISTEN_ENDPOINT 		= "MW-REPLISTEN-ENDPOINT";
const char* const ZEROMQ_PUB_ENDPOINT 			= "MW-PUBLISH-ENDPOINT";
const char* const ZEROMQ_SUB_ENDPOINT 			= "MW-SUBSCRIBE-ENDPOINT";


//BoltConnection
const char* const BOLT_HIDE_MWGET			= "MW-HIDE-MWGET";
const char* const BOLT_LOG_FILTER			= "MW-LOG-FILTER";


//Generic JMS
const char* const JMS_PERSISTENT			= "MW-PERSISTENT";
const char* const JMS_JVM_IGNORE_UNRECOGNIZED		= "MW-JVM-IGNORE-UNRECOGNIZED";

}   // end namespace api5
}   // end namespace gmsec

#endif 
