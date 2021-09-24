/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef ZEROMQ_HELP_H
#define ZEROMQ_HELP_H

#include "gmhelp.h"

// Apache ZeroMQ Client Options
//
ConfigOption zmq_client_opt1 =
{
" *  connectionType=<type>",

"        <type> value must be gmsec_zeromq413 [required]",

"        Used to specify the type of connection object to set up.  For ZeroMQ\n"
"        clients, this must be set to either gmsec_zeromq413.\n",

"        Example:\n"
"            connectionType=gmsec_zeromq413\n"
};

ConfigOption zmq_client_opt2 =
{
" *  mw-sub-endpoint=<string>",

"        String consists of one or more URLs. Socket operation connects to the\n"
"        provided endpoint by default. [required, optional if mw-pub-endpoint is\n"
"        in use]",

"        This option can be used to specify the addresses and ports\n"
"        that the ZeroMQ subscribing client should connect to."
"        \n\n"
"        The option is used in the form of:\n\n"
"            mw-sub-endpoint=tcp://<addr>:<port>"
"            \n"
"            mw-sub-endpoint=tcp://<addr>:<port>;tcp://<addr2>:<port2>"
"        \n"
"        \n"
"        The option can also be used to bind to a local endpoint\n"
"        in the form of:\n\n"
"            mw-sub-endpoint=bind:tcp://<addr>:<port>"
"            \n"
"            mw-sub-endpoint=bind:tcp://<addr>:<port>;tcp://<addr2>:<port2>"
"        \n"
"        \n"
"        For other additional information, please refer to the GMSEC API\n"
"        'Installation and Configuration Guide'.",

"        Examples:\n"
"            To connect to a single publisher:\n"
"                mw-sub-endpoint=tcp://10.10.50.123:4501\n"
"            \n"
"            To connect to multiple publishers:\n"
"                mw-sub-endpoint=tcp://10.10.50.123:4501;tcp://10.10.50.124:4501\n"
"            \n"
"            To bind to a single port:\n"
"                mw-sub-endpoint=bind:tcp://10.1.123:4501\n"
"            \n"
"            To bind to multiple ports:\n"
"                mw-sub-endpoint=bind:tcp://10.10.50.123:4501;tcp://10.10.50.124:4501\n"

};


ConfigOption zmq_client_opt3 =
{
" *  mw-pub-endpoint=<string>",

"        String consists of one or more URLs. Socket operation binds to the\n"
"        provided endpoint by default [required, optional if mw-sub-endpoint\n"
"        is in use]",

"        This option can be used to specify the addresses and ports\n"
"        that the ZeroMQ publishing client should bind to."
"        \n\n"
"        The option is used in the form of:\n\n"
"            mw-pub-endpoint=tcp://<addr>:<port>"
"            \n"
"            mw-pub-endpoint=tcp://<addr>:<port>;tcp://<addr2>:<port2>"
"        \n"
"        \n"
"        The option can also be used to connect to a local or remote endpoint\n"
"        in the form of:\n\n"
"            mw-pub-endpoint=connect:tcp://<addr>:<port>"
"            \n"
"            mw-pub-endpoint=connect:tcp://<addr>:<port>;tcp://<addr2>:<port2>"
"        \n"
"        \n"
"        For other additional information, please refer to the GMSEC API\n"
"        'Installation and Configuration Guide'.",

"        Examples:\n"
"            To bind to a single port:\n"
"                mw-pub-endpoint=tcp://10.1.123:4501\n"
"            \n"
"            To bind to multiple ports:\n"
"                mw-pub-endpoint=tcp://10.10.50.123:4501;tcp://10.10.50.124:4501\n"
"            \n"
"            To connect to a single port:\n"
"                mw-pub-endpoint=connect:tcp://10.1.123:4501\n"
"            \n"
"            To connect to multiple ports:\n"
"                mw-pub-endpoint=connect:tcp://10.10.50.123:4501;tcp://10.10.50.124:4501\n"

};

// ZeroMQ broker options
//
ConfigOption zmq_server_opt1 =
{
" *  Not applicable.",
"",
"",
""
};

#endif
