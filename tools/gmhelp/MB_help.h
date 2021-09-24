/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef MB_HELP_H
#define MB_HELP_H

#include "gmhelp.h"

// MessageBus Client Options
//
ConfigOption mb_client_opt1 =
{
" *  mw-id=<string>",

"        <string> must be mb. [required]",

"        Used to specify the type of connection object to set up. For MessageBus\n"
"        clients, this must be set to mb.",

"        Example:\n"
"            mw-id=mb"
};

ConfigOption mb_client_opt2 =
{
" *  server=<string>",

"        Used to specify server address(es). [optional]", 

"        Comma-delimited list of server names or IP addresses. If not specified,\n"
"        the default server is 'localhost', and the default port is 9000.",

"        Examples:\n"
"            server=10.1.2.159\n"
"            server=10.1.2.159:8008\n"
"            server=10.1.2.159,10.1.2.160\n"
"            server=10.1.2.159:8008,10.1.2.160:9009"
};

ConfigOption mb_client_opt3 =
{
" *  mw-compress=<true | false>",

"        Sets the flag for compressing MessageBus messages. [optional]",

"        This option can be used to specify that messages sent via MessageBus\n"
"        are compressed. The default is no compression.",

"        Examples:\n"
"            mw-compress=true\n"
"            mw-compress=false\n"
};

ConfigOption mb_client_opt4 =
{
" *  debug=<true | false>",

"        Enables MessageBus debug information to be written to a file. [optional]",

"        This option can be used to specify that debug information of the\n"
"        MessageBus be written to a file. This file will have the name TBD.\n"
"        The default setting is 'false'.",

"        Examples:\n"
"            debug=true\n"
"            debug=false"
};

ConfigOption mb_client_opt5 =
{
" *  loopback=<true | false>",

"        Enables echoing of published messages back to client.",

"        This option can be used to request MessageBus to echo to clients\n"
"        any message that is published by the client. The default setting is\n"
"        'false'.",

"        Examples:\n"
"            loopback=true\n"
"            loopback=false"
};


// MessageBus Server Options
//
ConfigOption mb_server_opt1 =
{
" *  port",

"        Socket port that MessageBus listens on for client connections.\n"
"        [optional]",

"        This integer parameter specifies what port MessageBus should be\n"
"        listening on other than the default port of 9000.\n\n"
"        Ports 1-65535 are available (unless already in use).  Certain systems\n"
"        require administrative privileges for using ports 1-1023.",

"        Example:\n"
"            ./MBServer 9334"
};

#endif
