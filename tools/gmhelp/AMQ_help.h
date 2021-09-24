/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef AMQ_HELP_H
#define AMQ_HELP_H

#include "gmhelp.h"

// Apache ActiveMQ Client Options
//
ConfigOption amq_client_opt1 =
{
" *  mw-id=<string>",

"        <string> must be activemq39. [required]",

"        Used to specify the type of connection object to set up. For ActiveMQ\n"
"        clients, this must be set to activemq39.\n",

"        Example:\n"
"            mw-id=activemq39\n"
"        \n"
"        Note: For information regarding using the GMSEC Generic JMS wrapper\n"
"        with ActiveMQ, run 'gmhelp generic_jms activemq'."
};

ConfigOption amq_client_opt2 =
{
" *  mw-filter-dups=<yes | no>",

"        Filter duplicate messages. [optional]", 

"        This option can be used to specify whether to filter duplicate\n"
"        messages or not. The default is 'yes'.",

"        Examples:\n"
"            mw-filter-dups=yes\n"
"            mw-filter-dups=no"
};

ConfigOption amq_client_opt3 =
{
" *  server=<string>",

"        Comma-delimited list of server(s) and server options. [optional]",

"        This option can be used to specify the addresses and ports\n"
"        of ActiveMQ Server that the client should connect to. If not\n"
"        specified, localhost and port 61616 are assumed."
"        \n\n"
"        The option is used in the form of:\n\n"
"            server=tcp://<addr>:<port>[?<AMQopt=value>[&<AMQopt=value>&...]]"
"        \n"
"        Some useful ActiveMQ server options include:\n"
"        \n"
"          *  connection.SendTimeout=<milliseconds>\n"
"                Time to wait on message sends for a response.\n"
"        \n"
"          *  wireFormat.MaxInactivityDuration=<milliseconds>\n"
"                The maximum inactivity period before which the socket is\n"
"                considered dead.\n"
"        \n"
"          *  wireFormat.MaxInactivityDurationInitalDelay=<milliseconds>\n"
"                The initial delay in starting the maximum inactivity checks\n"
"                (note this misspelling of 'Inital').\n"
"        \n"
"          *  soConnectTimeout=<microseconds>\n"
"                Socket connection timeout in microseconds.\n"
"        \n"
"        Additional ActiveMQ connection options for clients are described at:\n"
"            http://activemq.apache.org/cms/configuring.html\n"
"        \n"
"        For other additional information, please refer to the GMSEC API\n"
"        'Installation and Configuration Guide'.",

"        Examples:\n"
"            For a single server:\n"
"                server=tcp://10.1.123:61616\n"
"                server=tcp://10.1.123:61616&connection.SendTimeout=2000\n"
"            \n"
"            For multiple servers employing failover:\n"
"                server=failover:(tcp://<primary>:61616?wireFormat.MaxInactivityDuration=3000&wireFormat.MaxInactivityDurationInitalDelay=2000&soConnectTimeout=1500,tcp://<backup>:61616?wireFormat.MaxInactivityDuration=3000&wireFormat.MaxInactivityDurationInitalDelay=2000&soConnectTimeout=1500)?useExponentialBackOff=false&initialReconnectDelay=5000\n"
"            \n"
"            where <primary> and <backup> are the primary and backup middleware\n"
"            server IP addresses or domain names.\n"
"            \n"
"            Note the (mis)spelling of \"Inital\" in MaxInactivityDurationInital\n"
"            Delay.\n"
"            \n"
"            Also note that when a server string is to be embedded in XML, the\n"
"            XML special characters will need to be escaped (e.g. every &\n"
"            (ampersand) will need to be replaced with &amp;)."
};


// Apache ActiveMQ Server Options
//
ConfigOption amq_server_opt1 =
{
" *  Not applicable.",

"        Refer to the GMSEC API 'Installation and Configuration Guide' regarding\n"
"        helpful suggestions for perfomance enhancing configuration options for\n"
"        the Apache ActiveMQ Server.",
"",
""
};

#endif
