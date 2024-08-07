/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Log

%{
#include <map>
#include <gmsec5/util/Log.h>
using namespace gmsec::api5;
using namespace gmsec::api5::util;
%}

%ignore gmsec::api5::util::Log::registerHandler(GMSEC_LogHandler*);
%ignore gmsec::api5::util::Log::registerHandler(GMSEC_LogLevel, GMSEC_LogHandler*);

%rename("set_reporting_level") setReportingLevel;
%rename("get_reporting_level") getReportingLevel;
%rename("register_handler") registerHandler;
%rename("to_string") toString;
%rename("from_string") fromString;


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/Log.h>


%extend gmsec::api5::util::Log
{
    static void error(const char* msg)
    {
        GMSEC_ERROR << msg;
    }

    static void secure(const char* msg)
    {
        GMSEC_SECURE << msg;
    }

    static void warning(const char* msg)
    {
        GMSEC_WARNING << msg;
    }

    static void info(const char* msg)
    {
        GMSEC_INFO << msg;
    }

    static void verbose(const char* msg)
    {
        GMSEC_VERBOSE << msg;
    }

    static void debug(const char* msg)
    {
        GMSEC_DEBUG << msg;
    }
}

