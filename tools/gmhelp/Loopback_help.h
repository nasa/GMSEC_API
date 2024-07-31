/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef LOOPBACK_HELP_H
#define LOOPBACK_HELP_H

#include "gmhelp.h"

// Loopback Client Options
//
ConfigOption loopback_client_opt1 =
{
" *  mw-id=<string>",

"        <string> must be loopback. [required]",

"        Used to specify the type of connection object to set up. For Loopback\n"
"        clients, this must be set to loopback.",

"        Example:\n"
"            mw-id=loopback"
};

ConfigOption loopback_client_opt2 =
{
" *  mw-sim-connect-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to connect (to the loopback bus).\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-connect-failure=true\n"
"            mw-sim-connect-failure=false\n"
};

ConfigOption loopback_client_opt3 =
{
" *  mw-sim-subscribe-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to subscribe (to a topic).\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-subscribe-failure=true\n"
"            mw-sim-subscribe-failure=false\n"
};

ConfigOption loopback_client_opt4 =
{
" *  mw-sim-unsubscribe-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to unsubscribe.\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-unsubscribe-failure=true\n"
"            mw-sim-unsubscribe-failure=false\n"
};

ConfigOption loopback_client_opt5 =
{
" *  mw-sim-publish-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to publish a message.\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-publish-failure=true\n"
"            mw-sim-publish-failure=false\n"
};

ConfigOption loopback_client_opt6 =
{
" *  mw-sim-request-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to issue a request message.\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-request-failure=true\n"
"            mw-sim-request-failure=false\n"
};

ConfigOption loopback_client_opt7 =
{
" *  mw-sim-reply-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to reply to a request message.\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-reply-failure=true\n"
"            mw-sim-reply-failure=false\n"
};

ConfigOption loopback_client_opt8 =
{
" *  mw-sim-receive-failure=<bool>",

"        <bool> must be either true or false. [optional]",

"        Boolean value to indicate whether the loopback wrapper should simulate\n"
"        a failure when an attempt is made to receive a message.\n\n"
"        If not specified, the default is false.",

"        Example:\n"
"            mw-sim-receive-failure=true\n"
"            mw-sim-receive-failure=false\n"
};

ConfigOption loopback_client_opt9 =
{
" *  mw-sim-publish-delay-ms=<int>",

"        <int> time delay value, representing milliseconds, greater than zero (0). [optional]",

"        A period, in milliseconds, to simulate a delay in sending a message.\n"
"        The delay must be greater than zero (0).\n\n"
"        If not specified, the default value is one (1) millisecond.",

"        Example:\n"
"            mw-sim-publish-delay-ms=5\n"
};

#endif
