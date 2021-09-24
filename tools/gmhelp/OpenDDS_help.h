/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef OPENDDS_HELP_H
#define OPENDDS_HELP_H

#include "gmhelp.h"

// OpenDDS Client Options
//
ConfigOption opendds_client_opt1 =
{
" *  mw-id=<string>",

"        <string> must be opendds312. [required]",

"        Used to specify the type of connection object to set up. For OpenDDS\n"
"        clients, this must be set to opendds312.",

"        Example:\n"
"            mw-id=opendds312"
};

ConfigOption opendds_client_opt2 =
{
" *  server=<string>",

"        DCPSInfoRepo address and port.",

"        This option can be used to specify the addresses and ports\n"
"        of DCPSInfoRepo (OpenDDS broker) that the client should connect \n"
"        to. If not specified, localhost and port 12345 are assumed.\n\n"

"        The option is used in the form of:\n"
"            server=<addr>:<port>",

"        Example:\n"
"            server=127.0.0.1:12345"
};

ConfigOption opendds_client_opt3 =
{
" *  mw-opendds-config-file=<string>",

"        Configuration file for transport information. [optional]",

"        This option is used to provide a .ini file for DCPSInfoRepo\n"
"        (OpenDDS broker) to manage the transport options for the clients.\n"
"        If not specified, dds_conf.ini in $GMSEC_HOME/bin will be used.",

"        Example:\n"
"            mw-opendds-config-file=$GMSEC_HOME/bin/opendds/dds_conf.ini"
};

ConfigOption opendds_client_opt4 =
{
" *  mw-domain-id=<integer>",

"        Domain-id option. [optional]",

"        This option can be used to specify the domain-id.\n"
"        If not specified, domain-id is assumed as 0.",

"        Example:\n"
"            mw-domain-id=123"
};

// OpenDDS Server Options
//
ConfigOption opendds_server_opt1 =
{
" *  Not applicable.",

"        Refer to the GMSEC API 'Installation and Configuration Guide' for\n"
"        guidance on configuring the OpenDDS server."
};

#endif
