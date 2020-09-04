/*
 * Copyright 2007-2020 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef SS_HELP_H
#define SS_HELP_H

#include "gmhelp.h"

// TIBCO SmartSockets Client Options
//
ConfigOption ss_client_opt1 =
{
" *  connectionType=<string>",

"        <string> must be a variant of gmsec_ss. [required]",

"        Used to specify the type of connection object to set up.  For Smart\n"
"        Sockets clients, this must be set to a variant of gmsec_ss (see\n"
"        examples below).",

"        Examples:\n"
"            connectionType=gmsec_ss\n"
"            connectionType=gmsec_ss68\n"
"            connectionType=gmsec_ss681\n"
"            connectionType=gmsec_ss682"
};

ConfigOption ss_client_opt2 =
{
" *  server=<string>",

"        Used to specify server address(es). [optional]", 

"        Comma-delimited list of server names or IP addresses.  If not specified,\n"
"        the default protocol is 'tcp', the default server is 'localhost', and\n"
"        the default port is 5101.",

"        Examples:\n"
"            server=tcp:10.1.2.159\n"
"            server=tcp:10.1.2.159:5101\n"
"            server=10.1.2.159,10.1.2.160\n"
"            server=10.1.2.159:8008,10.1.2.160:9009"
};

ConfigOption ss_client_opt3 =
{
" *  unique_subject=<string>",

"        Used to specify the sender subject. [optional]",

"        Always used as the sender property of a message sent to RTserver\n"
"        through a given connection.  When a connection is first opened\n"
"        to RTserver, it automatically subscribes to its unique subject.\n"
"        RTservers in a cloud do not allow multiple clients to have the same\n"
"        unique subject.\n"
"        \n"
"        Note: GMSEC API automatically creates a unique subject.",

"        Example:\n"
"            unique_subject=GMSEC.UNIQUE.SUBJ"
};

ConfigOption ss_client_opt4 =
{
" *  project=<string>",

"        Project partition name for routing messages. [optional]",

"        A self-contained unit or partition that prevents unwanted messages\n"
"        from being sent to processes, so that connections in different\n"
"        projects cannot send messages to each other.  A connection belongs\n"
"        to only one project, while an RTserver can provide publish-subscribe\n"
"        routing services for one or more projects.  If set to NULL, this\n"
"        option defaults to 'rtworks'.",

"        Example:\n"
"            project=MyProject"
};

ConfigOption ss_client_opt5 =
{
" *  subject_prefix=<string>",

"        Message subject prefix. [optional]",

"        Specifies the qualifier to prepend to message subject names that\n"
"        don't start with /.  Subject names are organized in a hierarchical\n"
"        namespace where the components are delimited by /.  A subject name\n"
"        that starts with / is called an absolute subject name.  All non-\n"
"        absolute subject names have Default_Subject_Prefix prepended to\n"
"        them.  If set to NULL, this option defaults to '/'.",

"        Example:\n"
"            subject_prefix=/GMSEC"
};

ConfigOption ss_client_opt6 =
{
" *  gmdSubject=<string>",

"        Message subject for Guaranteed Message Delivery. [optional]",

"        Specifies the subject that the Guaranteed Message Delivery will use\n"
"        for the connection.  This should be set for any process that is\n"
"        going to send GMD (Guaranteed Message Delivery) Messages.\n"
"        \n"
"        Note: The unique_subject will be overwritten with this value if set.\n"
"              This means that the value must be unique on the RTserver as\n"
"              defined by the unique_subject.",

"        Example:\n"
"            gmdSubject=/GMSEC"
};

ConfigOption ss_client_opt7 =
{
" *  gmdResend=<true | false>",

"        Resends any messages in GMD area. [optional]",

"        Can be used to specify whether or not to automatically resend any\n"
"        messages in the file-based GMD area.  By default, all outstanding\n"
"        messages will be deleted when the process restarts.",

"        Examples:\n"
"            gmdResend=true\n"
"            gmdResend=false"
};

ConfigOption ss_client_opt8 =
{
" *  commandFile=<file>",

"        File to execute via process' command interface. [optional]",

"        Submits a file to the process' command interface for execution.\n"
"        This method does not search for the file in any directories\n"
"        such as the SmartSockets standard directory, but simply uses the\n"
"        file name verbatim.",

"        Example:\n"
"            commandFile=TBD"
};

ConfigOption ss_client_opt9 =
{
" *  compress=<true | false>",

"        Turns on/off message compression. [optional]",

"        Turns on/off message compression.  To set the type of compression,\n"
"        and other SS options, use commandFile or commandString, options.\n"
"        Decompression is automatic and occurs when any Message::GetField()\n"
"        call is made for the first time.  By default, no compression is\n"
"        performed.",

"        Examples:\n"
"            compress=true\n"
"            compress=false"
};

ConfigOption ss_client_opt10 =
{
" *  autoServerReconnect=<true | false>",

"        Turns on/off server reconnect after disconnect. [optional]",

"        Turns on/off server reconnect after disconnect.  This facilitates\n"
"        automatic handling of failover and server going down for extended\n"
"        periods of time.  By default, this option is enabled.",

"        Examples:\n"
"            autoServerReconnect=true\n"
"            autoServerReconnect=false"
};

ConfigOption ss_client_opt11 =
{
" *  reconnectAttempts=<num>",

"        Number of times to try to reconnect to RTserver. [optional]",

"        If autoServerReconnect is enabled, this is the number of times to try\n"
"        to reconnect.  Less than or equal to 0 is implies infinite retries\n"
"        (default).  Greater than 0 and the client will try to reconnect to\n"
"        the server that many times.",

"        Examples:\n"
"            reconnectAttempts=5\n"
"            reconnectAttempts=0"
};

ConfigOption ss_client_opt12 =
{
" *  reconnectDelay=<milliseconds>",

"        Delay between reconnect attempts. [optional]",

"        If autoServerReconnect is enabled, this option can be used to specify\n"
"        the delay between reconnect attempts.  By default, the delay is 100\n"
"        milliseconds.",

"        Example:\n"
"            reconnectDelay=300"
};

ConfigOption ss_client_opt13 =
{
" *  ipc_gmd_type=<default | memory>",

"        Specifies whether GMD is file-based or memory-based. [optional]",

"        Specifies whether file-based or memory-based GMD is to be used.  If\n"
"        left to its default value of 'default', file-based GMD is attempted,\n"
"        and if that is unsuccessful, memory-based GMD is used.  If the value\n"
"        is set to 'memory', memory-based GMD is used.  This option is only for\n"
"        use with GMD.",

"        Examples:\n"
"            ipc_gms_type=default\n"
"            ipc_gms_type=memory"
};

ConfigOption ss_client_opt14 =
{
" *  server_disconnect_mode=<warm | gmd_failure | gmd_success>",

"        Action RTserver should take when RTclient disconnects. [optional]",

"        Can be used to specify the action RTserver should take when the\n"
"        RTclient disconnects from RTserver.\n"
"        \n"
"        Available modes are as follows:\n"
"        \n"
"            warm -- RTserver saves subject information about RTclient for GMD\n"
"                so that no messages are lost.\n"
"        \n"
"            gmd_failure -- RTserver destroys all information about RTclient\n"
"                and causes pending guaranteed message delivery to fail.\n"
"        \n"
"            gmd_success -- RTserver destroys all information about RTclient\n"
"                and causes pending guaranteed message delivery to succeed.",

"        Examples:\n"
"            server_disconnect_mode=warm\n"
"            server_disconnect_mode=gmd_failure\n"
"            server_disconnect_mode=gmd_success"
};

ConfigOption ss_client_opt15 =
{
" *  server_max_reconnect_delay=<seconds>",

"        Maximum delay for RTclient to reconnect to RTserver. [optional]",

"        Can be used to specify the upper bound on a random delay introduced\n"
"        when an RTclient has to reconnect to RTserver.  This option is useful\n"
"        when an RTserver with many clients fails and all of those RT processes\n"
"        are attempting to reconnect.  The delay enhances total reconnect time\n"
"        by slightly staggering reconnect requests.  Setting the option to zero\n"
"        disables the delay.\n"
"        \n"
"        Note: The GMSEC API uses 0.0 as the default.  This is different from\n"
"              the SS API which uses 30.0 (seconds) as the default.",

"        Examples:\n"
"            server_max_reconnect_delay=30.0\n"
"            server_max_reconnect_delay=5.0\n"
"            server_max_reconnect_delay=0.0\n"
};

ConfigOption ss_client_opt16 =
{
" *  command_feedback=<always | interactive | never>",

"        Display feedback when a command is successfully executed. [optional]",

"        Can be used to specify whether feedback is displayed after a command is\n"
"        successfully executed.  If the command results in an error such as\n"
"        incorrect syntax, feedback is always displayed, regardless of the value\n"
"        of command_feedback.\n"
"        \n"
"        The three possible values for command_feedback are:\n"
"        \n"
"            always -- feedback is always given, regardless of how the command\n"
"                was executed, interactively or using a control message.\n"
"        \n"
"            interactive -- feedback is only given when the command is entered\n"
"                interactively.\n"
"        \n"
"            never -- feedback is never given.",

"        Examples:\n"
"            command_feedback=always\n"
"            command_feedback=interactive\n"
"            command_feedback=never"
};

ConfigOption ss_client_opt17 =
{
" *  server_keep_alive_timeout=<seconds>",

"        Ensures RTclient stays connected to RTserver. [optional]",

"        This option can be used to specify whether the RTclient should attempt\n"
"        to maintain its connection to the RTserver during periods when the\n"
"        client has not read any data from the server after a period of time has\n"
"        elapsed.  The time period, in seconds, is the maximum time to wait be-\n"
"        tween read events.\n"
"        \n"
"        The default timeout period is 15.0 seconds.  If the timeout period is\n"
"        set to 0.0, then the keep alives are disabled.",

"        Examples:\n"
"            server_keep_alive_timeout=15.0\n"
"            server_keep_alive_timeout=2.5\n"
"            server_keep_alive_timeout=0.0"
};

ConfigOption ss_client_opt18 =
{
" *  debug=<true | false>",

"        Enable debugging information to be outputted to a file. [optional]",

"        If true, the SmartSockets debug information will be written out to a\n"
"        file.  The default is to not produce debug output.",

"        Examples:\n"
"            debug=true\n"
"            debug=false"
};

ConfigOption ss_client_opt19 =
{
" *  socket_connect_timeout=<seconds>",

"        Timeout period to wait for a connection. [optional]",

"        When set, this option specifies the maximum amount of time (in\n"
"        seconds) the RT process waits when trying to create a client socket\n"
"        connected to a server process. This timeout is used to check for\n"
"        possible network failures.  Checking for connect timeouts is disabled\n"
"        if this option is set to 0.0.\n"
"        \n"
"        All SmartSockets standard processes use sockets for inter-process\n"
"        communication.  Proper operation requires JRE 1.4 or later.\n"
"        \n"
"        This option is not supported for Java SmartSockets 6.5 and 6.6.  This\n"
"        option is necessary for failover from a physical disconnection (such\n"
"        as a cable being disconnected) to function within a timely manner (20\n"
"        sec vs. 2 min).  For any GMSEC components using the Pure Java inter-\n"
"        face AND SmartSockets 6.5 or 6.6, the physical disconnect failover\n"
"        will be slow.\n"
"        \n"
"        The workaround is for those Java-based components to switch to the\n"
"        JNI interface by simply setting the configuration parameter\n"
"        'connectionType' to gmsec_ssxx'.",

"        Examples:\n"
"            socket_connect_timeout=5.0\n"
"            socket_connect_timeout=0.0"
};


// TIBCO SmartSockets Server Options
//
ConfigOption ss_server_opt1 =
{
" *  Not applicable.",

"        Refer to the GMSEC API 'Installation and Configuration Guide' for\n"
"        guidance on configuring the TIBCO SmartSockets server.",
"",
""
};

#endif
