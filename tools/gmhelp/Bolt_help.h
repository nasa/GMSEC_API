/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef BOLT_HELP_H
#define BOLT_HELP_H

#include "gmhelp.h"

// Bolt Client Options
//
ConfigOption bolt_client_opt1 =
{
" *  mw-id=<string>",

"        <string> must be bolt. [required]",

"        Used to specify the type of connection object to set up. For Bolt\n"
"        clients, this must be set to bolt.",

"        Example:\n"
"            mw-id=bolt"
};

ConfigOption bolt_client_opt2 =
{
" *  server=<string>",

"        Used to specify server address(es). [optional]", 

"        Comma-delimited list of server names or IP addresses. If not specified,\n"
"        the default server is 'localhost', and the default port is 9100.",

"        Examples:\n"
"            server=10.1.2.159\n"
"            server=10.1.2.159:8008\n"
"            server=10.1.2.159,10.1.2.160\n"
"            server=10.1.2.159:8008,10.1.2.160:9009"
};


// Bolt Server Options
//
ConfigOption bolt_server_opt1 =
{
" *  port=<num>",

"        Socket port that Bolt listens on for client connections. [optional]",

"        This integer parameter specifies what port Bolt should be listening on\n"
"        other than the default port of 9100.\n\n"
"        Ports 1-65535 are available (unless already in use). Certain systems\n"
"        require administrative privileges for using ports 1-1023.",

"        Example:\n"
"            java -jar bolt.jar port=9334"
};

ConfigOption bolt_server_opt2 =
{
" *  maxMessageSize=<num>",

"        Maximum message size, in bytes, to accept. [optional]",

"        This option can be used to set the maximum size, in bytes, of\n"
"        message that will be allowed to be sent through the Bolt Server.",

"        Example:\n"
"            java -jar bolt.jar maxMessageSize=4100000"
};

ConfigOption bolt_server_opt3 =
{
" *  inputBufferSize=<num>",

"        Maximum number of bytes accepted per input operation. [optional]",

"        This option can be used to set the maximum number of bytes that an\n"
"        input operation will support.",

"        Example:\n"
"            java -jar bolt.jar inputBufferSize=5000"
};

ConfigOption bolt_server_opt4 =
{
" *  outputBufferSize=<num>",

"        Maximum number of bytes accepted per output operation. [optional]",

"        This option can be used to set the maximum number of bytes that an\n"
"        output operation will support.",

"        Example:\n"
"            java -jar bolt.jar outputBufferSize=5000"
};

ConfigOption bolt_server_opt5 =
{
" *  inputBufferLimit=<num>",

"        Maximum number of bytes to hold in client's input buffer. [optional]",

"        This option can be used to set the maximum size, in bytes, of the\n"
"        buffer that stores client input messages.",

"        Example:\n"
"            java -jar bolt.jar inputBufferLimit=4000000"
};

ConfigOption bolt_server_opt6 =
{
" *  clientsPerManager=<num>",

"        The number of client connections to assign to each I/O manager.\n"
"        [optional]",

"        This option can be used to set the maximum number of clients that will be\n"
"        assigned to an I/O manager.",

"        Example:\n"
"            java -jar bolt.jar clientsPerManager=5"
};

ConfigOption bolt_server_opt7 =
{
" *  backlogSize=<num>",

"        The depth size for the backlog queue of the listen socket used by the\n"
"        Bolt server.  The default size is 50. [optional]",

"        This option can be used to configure the depth size for the backlog\n"
"        queue for the listen socket that the Bolt server sets up when\n"
"        binding the listen socket.",

"        Example:\n"
"            java -jar bolt.jar backlogSize=100"
};

#endif
