/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Log

%{
#include <map>
#include <gmsec5/util/Log.h>
using namespace gmsec::api5;
%}


// These are *not* being ignored... why??
%ignore gmsec::api5::util::Log::registerHandler(GMSEC_LogHandler*);
%ignore gmsec::api5::util::Log::registerHandler(GMSEC_LogLevel, GMSEC_LogHandler*);
%ignore gmsec::api5::util::Log::registerHandler(LogHandler*);


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/Log.h>


%extend gmsec::api5::util::Log {
    static void CALL_TYPE error(const char* msg)
    {
        GMSEC_ERROR << msg;
    }

    static void CALL_TYPE secure(const char* msg)
    {
        GMSEC_SECURE << msg;
    }

    static void CALL_TYPE warning(const char* msg)
    {
        GMSEC_WARNING << msg;
    }

    static void CALL_TYPE info(const char* msg)
    {
        GMSEC_INFO << msg;
    }

    static void CALL_TYPE verbose(const char* msg)
    {
        GMSEC_VERBOSE << msg;
    }

    static void CALL_TYPE debug(const char* msg)
    {
        GMSEC_DEBUG << msg;
    }
};
