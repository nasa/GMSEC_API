/*
 * Copyright 2007-2020 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef WL_HELP_H
#define WL_HELP_H

#include "gmhelp.h"

// Oracle WebLogic Client Options
//
ConfigOption wl_client_opt1 =
{
" *  connectionType=<string>",

"        <string> must be gmsec_weblogic11. [required]",

"        Used to specify the type of connection object to set up.  For WebLogic\n"
"        clients, this must be set to gmsec_weblogic11.",

"        Example:\n"
"            connectionType=gmsec_weblogic11\n"
"        \n"
"        Note: For information regarding using the GMSEC Generic JMS wrapper\n"
"        with WebLogic, run 'gmhelp generic_jms weblogic'."
};

ConfigOption wl_client_opt2 =
{
" *  server=<string>",

"        The server URL. [required]",

"        Used to specify the server URL.  WebLogic supports the T3 and T3S\n"
"        (T3 w/ SSL) protocols.  The default port for T3 is 7001; for T3S,\n"
"        the default port is 7002.",

"        Examples:\n"
"            server=t3://10.1.2.123:7001\n"
"            server=t3s://10.1.2.123:7002"
};

ConfigOption wl_client_opt3 =
{
" *  mw-channel=<string>",

"        The channel name string. [required]",

"        Used to specify the channel the client will use to connect to the\n"
"        server.  This value is required for network communication.",

"        Example:\n"
"            mw-channel=GMSEC.TEST.CHANNEL"
};

ConfigOption wl_client_opt4 =
{
" *  username=<string>",

"        User ID used for authentication and authorization. [required]",

"        Used to specify the user ID that the WebLogic server will use to\n"
"        authenticate and authorize the connecting client.",

"        Example:\n"
"            username=joe"
};

ConfigOption wl_client_opt5 =
{
" *  password=<string>",

"        Password of the user requesting connection. [required]",

"        Used to specify the user password that the WebLogic server will use to\n"
"        authenticate and authorize the connection client.",

"        Example:\n"
"            password=$ecretX43"
};


// Oracle WebLogic Server Options
//
ConfigOption wl_server_opt1 =
{
" *  Not applicable.",

"        Refer to the GMSEC API 'Installation and Configuration Guide' for\n"
"        guidance on configuring the Oracle WebLogic server.",
"",
""
};

#endif
