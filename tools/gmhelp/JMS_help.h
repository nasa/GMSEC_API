/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef JMS_HELP_H
#define JMS_HELP_H

#include "gmhelp.h"


// JMS ActiveMQ Client Options
//
ConfigOption jms_amq_client_opt1 =
{
" *  connectionType=<string>",

"        Must be gmsec_generic_jms. [required]",

"        Used to specify the type of connection object to set up.  For Generic\n"
"        JMS clients, this must be set to gmsec_generic_jms.",

"        Example:\n"
"            connectionType=gmsec_generic_jms"
};

ConfigOption jms_amq_client_opt2 =
{
" *  mw-initial-context-factory=<string>",

"        ActiveMQ initial context factory. [required]",

"        No additional details available.",

"        Example:\n"
"            mw-initial-context-factory=org.apache.activemq.jndi.ActiveMQInitial\n"
"            ContextFactory"
};


ConfigOption jms_amq_client_opt3 =
{
" *  mw-provider-url=<string>",

"        The transport URI. [required]",

"        Transport URIs are detailed in:\n"
"        http://activemq.apache.org/configuring-transports.html\n\n"
"        Both the TCP and failover transport reference on that page are\n"
"        likely of interest.",

"        Example:\n"
"            mw-provider-url=tcp://10.1.2.123:61616"
};

ConfigOption jms_amq_client_opt4 =
{
" *  mw-lookup-connection-factory=<string>",

"        The name of the ActiveMQ connection factory. [required]",

"        The name of the connection factory as defined in the JNDI definition.",

"        Example:\n"
"            mw-lookup-connection-factory=ConnectionFactory\n"
};

ConfigOption jms_amq_client_opt5 =
{
" *  mw-lookup-topic=<string>",

"        The name of the JMS topic. [optional]",

"        The name of the JMS topic as defined in the JNDI definition.",

"        Example:\n"
"            mw-lookup-topic=dynamicTopics/AnyTopic"
};


// JMS WebLogic Client Options
//
ConfigOption jms_wl_client_opt1 =
{
" *  connectionType=<string>",

"        Must be gmsec_generic_jms. [required]",

"        Used to specify the type of connection object to set up.  For Generic\n"
"        JMS clients, this must be set to gmsec_generic_jms.",

"        Example:\n"
"            connectionType=gmsec_generic_jms"
};

ConfigOption jms_wl_client_opt2 =
{
" *  mw-initial-context-factory=<string>",

"        WebLogic initial context factory. [required]",

"        No additional details available.",

"        Example:\n"
"            mw-initial-context-factory=weblogic.jndi.WLInitialContextFactory"
};

ConfigOption jms_wl_client_opt3 =
{
" *  mw-provider-url=<string>",

"        The server URL using T3S protocol. [required]",

"        No additional details available.",

"        Example:\n"
"            mw-provider-url=t3s://10.1.2.123:7002"
};

ConfigOption jms_wl_client_opt4 =
{
" *  mw-lookup-connection-factory=<string>",

"        The name of the WebLogic connection factory. [required]",

"        No additional details available.",

"        Example:\n"
"            mw-lookup-connection-factory=weblogic.jms.ConnectionFactory"
};

ConfigOption jms_wl_client_opt5 =
{
" *  mw-lookup-topic=<string>",

"        The name of the JMS topic. [optional]",

"        The name of the JMS topic as defined in the JNDI definition.",

"        Example:\n"
"            mw-lookup-topic=GMSEC/TEST/PUBLISH"
};

ConfigOption jms_wl_client_opt6 =
{
" *  weblogic.security.CustomTrustKeyStoreFileName=<string>",

"        The path (file name) to the Trust Key Store file. [optional]",

"        No additional details available.",

"        Example:\n"
"            weblogic.security.CustomTrustKeyStoreFileName=MyKeys.ts"
};

ConfigOption jms_wl_client_opt7 =
{
" *  weblogic.security.CustomTrustKeyStoreType=<string>",

"        The Custom Trust Key Store type. [optional]",

"        No additional details available.",

"        Example:\n"
"            <none available>"
};

ConfigOption jms_wl_client_opt8 =
{
" *  weblogic.security.CustomTrustKeyStorePassPhrase=<string>",

"        The Custom Trust Key Store passphrase. [optional]",

"        No additional details available.",

"        Example:\n"
"            <none available>"
};


// JMS WebSphere Client Options
//
ConfigOption jms_ws_client_opt1 =
{
" *  connectionType=<string>",

"        Must be gmsec_generic_jms. [required]",

"        Used to specify the type of connection object to set up.  For Generic\n"
"        JMS clients, this must be set to gmsec_generic_jms.",

"        Example:\n"
"            connectionType=gmsec_generic_jms"
};

ConfigOption jms_ws_client_opt2 =
{
" *  mw-initial-context-factory=<string>",

"        WebSphere initial context factory. [required]",

"        No additional details available.",

"        Example:\n"
"           mw-initial-context-factory=com.sun.jndi.fscontext.RefFSContextFactory"
};


ConfigOption jms_ws_client_opt3 =
{
" *  mw-provider-url=<string>",

"        Location of the bindings file. [required]",

"        This is the location of the bindings file, which may be on the\n"
"        file system or located remotely on another system, depending on the\n"
"        middleware chosen.",

"        Example:\n"
"            mw-provider-url=file:/C:/jndi"
};

ConfigOption jms_ws_client_opt4 =
{
" *  mw-lookup-connection-factory=<string>",

"        The name of the WebSphere connection factory. [required]",

"        The name of the connection factory as defined in the JNDI definition.",

"        Example:\n"
"            mw-lookup-connection-factory=GMSECTESTFACTORY"
};

ConfigOption jms_ws_client_opt5 =
{
" *  mw-lookup-topic=<string>",

"        The name of the JMS topic. [optional]",

"        The name of the JMS topic as defined in the JNDI definition.",

"        Example:\n"
"            mw-lookup-topic=GMSECTESTTOPIC"
};

#endif
