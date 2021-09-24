/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef IBM_MQ_HELP_H
#define IBM_MQ_HELP_H

#include "gmhelp.h"

// IBM MQ Client Options
//
ConfigOption ibmmq_client_opt1 =
{
" *  mw-id=<string>",

"        <string> must be ibmmq90. [required]",

"        Used to specify the type of connection object to set up. For IBM MQ\n"
"        clients, this must be set to ibmmq90.",

"        Examples:\n"
"            mw-id=ibmmq90\n"
"        \n"
"        Note: For information regarding using the GMSEC Generic JMS wrapper\n"
"        with IBM MQ, run 'gmhelp generic_jms ibmmq'."
};

ConfigOption ibmmq_client_opt2 =
{
" *  server=<string>",

"        The server name or IP address and optional port number. [optional]",

"        Comma-delimited list of server names or IP addresses. The default\n"
"        default server is 'localhost'. The default port is 1414.",

"        Examples:\n"
"            server=10.1.2.123\n"
"            server=10.1.2.123(8008)\n"
"            server=10.1.2.123,10.1.2.321\n"
"            server=10.1.2.123(8008),10.1.2.321(9009)"
};

ConfigOption ibmmq_client_opt3 =
{
" *  mw-channel=<string>",

"        The channel name string. [required]",

"        Used to specify the channel the client will use to connect to the\n"
"        server. This value is required for network communication.",

"        Example:\n"
"            mw-channel=GMSEC.TEST.CHANNEL"
};

ConfigOption ibmmq_client_opt4 =
{
" *  queueManager=<string>",

"        Queue manager name string. [optional]",

"        Used to specify the queue manager to which the user desires to\n"
"        connect. The server's default queue manager is automatically used\n"
"        in the event that this value is not specified.",

"        Example:\n"
"            queueManager=TEST_QM"
};

ConfigOption ibmmq_client_opt5 =
{
" *  persistent=<yes | no>",

"        Retain queue manager after the client has disconnected. [optional]",

"        Used to specify whether the queue manager should be retained after\n"
"        the client has disconnected from the server. The default setting is\n"
"        to not retain the queue manager.",

"        Examples:\n"
"            persistent=yes\n"
"            persistent=no"
};

ConfigOption ibmmq_client_opt6 =
{
" *  mw-filter-dups=<yes | no>",

"        Filter duplicate messages. [optional]",

"        This option can be used to specify whether to filter duplicate\n"
"        messages or not. The default is 'yes'.",

"        Examples:\n"
"            mw-filter-dups=yes\n"
"            mw-filter-dups=no"
};

// IBM MQ Server Options
//
ConfigOption ibmmq_server_opt1 =
{
" *  Not applicable.",

"        Refer to the GMSEC API 'Installation and Configuration Guide' for\n"
"        guidance on configuring the IBM MQ server.",
"",
""
};

#endif
