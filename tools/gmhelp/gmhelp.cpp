/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include "gmhelp.h"

#include "AMQ_help.h"
#include "Artemis_help.h"
#include "Bolt_help.h"
#include "IBMMQ_help.h"
#include "JMS_help.h"
#include "MB_help.h"
#include "OpenDDS_help.h"
#include "WS_help.h"
#include "ZeroMQ_help.h"

#include <gmsec5/internal/StringUtil.h>

#include <iostream>

using namespace gmsec::api5::util;

using std::cerr;
using std::cout;
using std::endl;
using std::string;


int main(int argc, char** argv)
{
    MiddlewareHelp help;
    return help.processUserRequest(argc, argv);
}


MiddlewareHelp::MiddlewareHelp()
{
    MiddlewareOptions amq;
    amq.client.insert(amq_client_opt1);
    amq.client.insert(amq_client_opt2);
    amq.client.insert(amq_client_opt3);
    amq.server.insert(amq_server_opt1);

    MiddlewareOptions art;
    art.client.insert(art_client_opt1);
    art.client.insert(art_client_opt2);
    art.client.insert(art_client_opt3);
    art.server.insert(art_server_opt1);

    MiddlewareOptions bolt;
    bolt.client.insert(bolt_client_opt1);
    bolt.client.insert(bolt_client_opt2);
    bolt.server.insert(bolt_server_opt1);
    bolt.server.insert(bolt_server_opt2);
    bolt.server.insert(bolt_server_opt3);
    bolt.server.insert(bolt_server_opt4);
    bolt.server.insert(bolt_server_opt5);
    bolt.server.insert(bolt_server_opt6);
    bolt.server.insert(bolt_server_opt7);

    MiddlewareOptions ibmmq;
    ibmmq.client.insert(ibmmq_client_opt1);
    ibmmq.client.insert(ibmmq_client_opt2);
    ibmmq.client.insert(ibmmq_client_opt3);
    ibmmq.client.insert(ibmmq_client_opt4);
    ibmmq.client.insert(ibmmq_client_opt5);
    ibmmq.client.insert(ibmmq_client_opt6);
    ibmmq.server.insert(ibmmq_server_opt1);

    MiddlewareOptions mb;
    mb.client.insert(mb_client_opt1);
    mb.client.insert(mb_client_opt2);
    mb.client.insert(mb_client_opt3);
    mb.client.insert(mb_client_opt4);
    mb.client.insert(mb_client_opt5);
    mb.server.insert(mb_server_opt1);

    MiddlewareOptions opendds;
    opendds.client.insert(opendds_client_opt1);
    opendds.client.insert(opendds_client_opt2);
    opendds.client.insert(opendds_client_opt3);
    opendds.client.insert(opendds_client_opt4);
    opendds.server.insert(opendds_server_opt1);

	MiddlewareOptions zmq;
	zmq.client.insert(zmq_client_opt1);
	zmq.client.insert(zmq_client_opt2);
	zmq.client.insert(zmq_client_opt3);
	zmq.server.insert(zmq_server_opt1);

    MiddlewareOptions jms_amq;
    jms_amq.client.insert(jms_amq_client_opt1);
    jms_amq.client.insert(jms_amq_client_opt2);
    jms_amq.client.insert(jms_amq_client_opt3);
    jms_amq.client.insert(jms_amq_client_opt4);
    jms_amq.client.insert(jms_amq_client_opt5);

    MiddlewareOptions jms_ibmmq;
    jms_ibmmq.client.insert(jms_ws_client_opt1);
    jms_ibmmq.client.insert(jms_ws_client_opt2);
    jms_ibmmq.client.insert(jms_ws_client_opt3);
    jms_ibmmq.client.insert(jms_ws_client_opt4);
    jms_ibmmq.client.insert(jms_ws_client_opt5);


    // Setup look-up mapping.
    m_helpMap["activemq"] = amq;
    m_helpMap["artemis"] = art;
    m_helpMap["bolt"] = bolt;
    m_helpMap["ibmmq"] = ibmmq;
    m_helpMap["mb"] = mb;
    m_helpMap["opendds"] = opendds;
    m_helpMap["zeromq"] = zmq;

    m_helpMap["generic_jms_activemq"] = jms_amq;
    m_helpMap["generic_jms_ibmmq"] = jms_ibmmq;
}


int
MiddlewareHelp::processUserRequest(int argc, char** argv) const
{
    // Sample input:  prog <mw> [jms-mw] [param]
    //         argv:   0    1      2        3
    //
    const char* progName = argv[0];

    if (argc == 2)
    {
        string mw = StringUtil::stringToLower(argv[1]);

        if (mw.find("generic_jms") != string::npos)
        {
            cerr << "ERROR: missing [jms-mw].\n" << endl;
            showUsage(progName);
            return -1;
        }

        showHelp(mw, "");
    }
    else if (argc >= 3)
    {
        string mw = StringUtil::stringToLower(argv[1]);
        string jms_mw;
        string param;

        if (mw.find("generic_jms") != string::npos)
        {
            jms_mw = StringUtil::stringToLower(argv[2]);

            if (jms_mw != "activemq" &&
                jms_mw != "ibmmq")
            {
                cerr << "ERROR: unknown jms-mw of '" << jms_mw << "' found.\n" << endl;
                showUsage(progName);
                return -1;
            }

            if (argc == 4)
            {
                param = StringUtil::stringToLower(argv[3]);
            }
        }
        else
        {
            param = StringUtil::stringToLower(argv[2]);
        }

        if (argc == 3 && mw.find("generic_jms") != string::npos)
        {
            showHelp(mw, jms_mw);
        }
        else
        {
            showDetailedHelp(mw, jms_mw, param);
        }
    }
    else
    {
        showUsage(progName);
        return -1;
    }

    return 0;
}


void
MiddlewareHelp::showUsage(const string& progName) const
{
#if defined(WIN32) || defined(WIN64)
    size_t pos = progName.rfind("\\");
#else
    size_t pos = progName.rfind("/");
#endif

    string name = progName;

    if (pos != string::npos)
    {
        name = progName.substr(pos + 1);
    }

    cerr << "\n"
         << "USAGE: " << name << " [mw-id] [jms-mw] [parameter name]" << "\n"
         << "\n"
         << "Description of options:\n\n"
         << " *  [mw-id]\n"
         << "\n"
         << "    Must be one of the following middlewares:\n"
         << "\n"
         << "      activemq39\n"
         << "      artemis\n"
         << "      bolt\n"
         << "      ibmmq90\n"
         << "      mb\n"
         << "      opendds312\n"
         << "      zeromq413\n"
         << "      generic_jms\n"
         << "\n"
         << "      Notes:\n"
         << "         When specifying generic_jms, the [jms-mw] must be specified.\n"
         << "\n"
         << endl;

    cerr << " *  [jms-mw]\n"
         << "\n"
         << "    The type of middleware server being used by the JMS wrapper.  Must be one\n"
         << "    of the following:\n"
         << "\n"
         << "      activemq\n"
         << "      ibmmq\n"
         << "\n"
         << endl;

    cerr << " *  [parameter name]\n"
         << "\n"
         << "    Must be specific parameter option supported by a middleware.\n"
         << "    Specifying a parameter name, if valid, will display detailed\n"
         << "    information regarding that connection parameter.\n"
         << endl;

    cerr << "\n"
         << "For details on available middleware options, please re-run this utility with\n"
         << "the appropriate middleware type.\n"
         << "\n"
         << "If additional assistance is required, please submit your queries to:\n\n"
         << "    gmsec-support@lists.nasa.gov\n"
         << endl;
}


void
MiddlewareHelp::showHelp(const string& mw, const string& jms_mw) const
{
    const string MW_OPT       = "mw-id=";
    const string GMSEC_PREFIX = "gmsec_";

    string search_mw = mw;

    if (search_mw.find(MW_OPT) != string::npos)
    {
        search_mw = search_mw.substr(MW_OPT.length());
    }
    else if (search_mw.find(GMSEC_PREFIX) != string::npos)
    {
        search_mw = search_mw.substr(GMSEC_PREFIX.length());
    }

    if (jms_mw.length() > 0)
    {
        search_mw += string("_") + jms_mw;
    }

    bool found_mw = false;

    for (HelpMap::const_iterator it = m_helpMap.begin(); !found_mw && it != m_helpMap.end(); ++it)
    {
        if (it->first.find(search_mw.c_str(), 0, it->first.length()) != string::npos)
        {
            found_mw = true;
            displayOptions(search_mw, jms_mw, it->second);
        }
    }

    if (!found_mw)
    {
        cerr << "ERROR: unknown middleware '" << mw << "'" << endl;
    }
}


void
MiddlewareHelp::showDetailedHelp(const string& mw, const string& jms_mw, const string& parameter) const
{
    string search_mw = mw;

    size_t pos = search_mw.find("gmsec_");

    if (pos == 0)
    {
        search_mw = search_mw.substr(6);
    }

    if (jms_mw.length() > 0)
    {
        search_mw += string("_") + jms_mw;
    }

    bool found_mw  = false;
    bool found_opt = false;

    for (HelpMap::const_iterator it = m_helpMap.begin(); (!found_mw && !found_opt) && it != m_helpMap.end(); ++it)
    {
        if (it->first.find(search_mw.c_str(), 0, it->first.length()) != string::npos)
        {
            found_mw = true;

            const MiddlewareOptions& mw_opts = it->second;
            const ConfigOptions& client_opts = mw_opts.client;
            const ConfigOptions& server_opts = mw_opts.server;

            string search_param = StringUtil::stringToLower(parameter);

            // Sigh; only MessageBus server has an option that does not follow the key=value paradigm.
            if (search_mw != "mb")
            {
                search_param += "=";
            }

            for (ConfigOptions::const_iterator it2 = client_opts.begin(); !found_opt && it2 != client_opts.end(); ++it2)
            {
                const ConfigOption& cfg_opt = *it2;

                string cfg_opt_name = StringUtil::stringToLower(StringUtil::trim(cfg_opt.name));

                if (cfg_opt_name.find(search_param) != string::npos)
                {
                    found_opt = true;

                    cout << "Detailed Client Connection Option for " << mw;

                    if (jms_mw.length() > 0)
                    {
                        cout << " for the " << jms_mw << " middleware";
                    }
                    cout << ":\n\n";

                    displayOption(mw, cfg_opt, true);
                }
            }

            for (ConfigOptions::const_iterator it2 = server_opts.begin(); !found_opt && it2 != server_opts.end(); ++it2)
            {
                const ConfigOption& cfg_opt = *it2;

                string cfg_opt_name = StringUtil::stringToLower(StringUtil::trim(cfg_opt.name));

                if (cfg_opt_name.find(search_param) != string::npos)
                {
                    found_opt = true;

                    cout << "Detailed Server Option for " << search_mw << ":\n\n";

                    displayOption(mw, cfg_opt, true);
                }
            }
        }
    }

    if (!found_mw)
    {
        cerr << "ERROR: unknown middleware type '" << mw << "'." << endl;
    }
    if (!found_opt)
    {
        cerr << "ERROR: unknown option parameter '" << parameter << "' for " << mw << "." << endl;
    }
}


void
MiddlewareHelp::displayOptions(const string& mw_name, const string& jms_mw, const MiddlewareOptions& mw_opts) const
{
    const ConfigOptions& client_opts = mw_opts.client;
    const ConfigOptions& server_opts = mw_opts.server;

    cout << "Client Connection Options for " << mw_name;

    if (jms_mw.length() > 0)
    {
        cout << " for the " << jms_mw << " middleware";
    }
    cout << ":\n\n";

    for (ConfigOptions::const_iterator it = client_opts.begin(); it != client_opts.end(); ++it)
    {
        displayOption(mw_name, *it, false);
    }
    cout << endl;

    if (server_opts.size() > 0)
    {
        cout << "Server Options for " << mw_name << ":\n\n";

        for (ConfigOptions::const_iterator it = server_opts.begin(); it != server_opts.end(); ++it)
        {
            displayOption(mw_name, *it, false);
        }
        cout << endl;
    }
}


void
MiddlewareHelp::displayOption(const string& mw_name, const ConfigOption& cfg_opt, bool showDetails) const
{
    cout << cfg_opt.name << "\n";

    cout << cfg_opt.briefDescription << "\n\n";

    if (showDetails && cfg_opt.detailedDescription.length() > 0)
    {
        cout << cfg_opt.detailedDescription << "\n\n";
    }

    if (cfg_opt.example.length() > 0)
    {
        cout << cfg_opt.example << "\n" << endl;
    }
}
