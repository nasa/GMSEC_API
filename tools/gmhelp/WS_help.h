/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef WS_HELP_H
#define WS_HELP_H

#include "gmhelp.h"

// IBM WebSphere Client Options
//
ConfigOption ws_client_opt1 =
{
" *  connectionType=<string>",

"        <string> must be either gmsec_websphere71 or gmsec_websphere75.\n"
"        [required]",

"        Used to specify the type of connection object to set up.  For WebSphere\n"
"        clients, this must be set to either gmsec_websphere71 or\n"
"        gmsec_websphere75.",

"        Examples:\n"
"            connectionType=gmsec_websphere71\n"
"            connectionType=gmsec_websphere75\n"
"        \n"
"        Note: For information regarding using the GMSEC Generic JMS wrapper\n"
"        with WebSphere, run 'gmhelp generic_jms websphere'."
};

ConfigOption ws_client_opt2 =
{
" *  server=<string>",

"        The server name or IP address and optional port number. [optional]",

"        Comma-delimited list of server names or IP addresses.  The default\n"
"        default server is 'localhost'.  The default port is 1414.",

"        Examples:\n"
"            server=10.1.2.123\n"
"            server=10.1.2.123(8008)\n"
"            server=10.1.2.123,10.1.2.321\n"
"            server=10.1.2.123(8008),10.1.2.321(9009)"
};

ConfigOption ws_client_opt3 =
{
" *  mw-channel=<string>",

"        The channel name string. [required]",

"        Used to specify the channel the client will use to connect to the\n"
"        server.  This value is required for network communication.",

"        Example:\n"
"            mw-channel=GMSEC.TEST.CHANNEL"
};

ConfigOption ws_client_opt4 =
{
" *  queueManager=<string>",

"        Queue manager name string. [optional]",

"        Used to specify the queue manager to which the user desires to\n"
"        connect.  The server's default queue manager is automatically used\n"
"        in the event that this value is not specified.",

"        Example:\n"
"            queueManager=TEST_QM"
};

ConfigOption ws_client_opt5 =
{
" *  persistent=<yes | no>",

"        Retain queue manager after the client has disconnected. [optional]",

"        Used to specify whether the queue manager should be retained after\n"
"        the client has disconnected from the server.  The default setting is\n"
"        to not retain the queue manager.",

"        Examples:\n"
"            persistent=yes\n"
"            persistent=no"
};

ConfigOption ws_client_opt6 =
{
" *  mw-filter-dups=<yes | no>",

"        Filter duplicate messages. [optional]",

"        This option can be used to specify whether to filter duplicate\n"
"        messages or not.  The default is 'yes'.",

"        Examples:\n"
"            mw-filter-dups=yes\n"
"            mw-filter-dups=no"
};

// IBM WebSphere Server Options
//
ConfigOption ws_server_opt1 =
{
" *  Not applicable.",

"        Refer to the GMSEC API 'Installation and Configuration Guide' for\n"
"        guidance on configuring the IBM WebSphere server.",
"",
""
};

#endif
