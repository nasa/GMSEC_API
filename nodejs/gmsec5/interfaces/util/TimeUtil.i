/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module TimeUtil

%include "std_string.i"

%{
#include <gmsec5/util/TimeUtil.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char* buffer);
%ignore gmsec::api5::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, int, char* buffer);

%ignore operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);
%ignore operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);
%ignore operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/util/TimeUtil.h>

%extend gmsec::api5::util::TimeUtil {
        static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts)
        {
                char tmp[GMSEC_TIME_BUFSIZE];
                gmsec::api5::util::TimeUtil::formatTime(ts, tmp);
                return std::string(tmp);
        }

        static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, int subs)
        {
                char tmp[GMSEC_TIME_BUFSIZE];
                gmsec::api5::util::TimeUtil::formatTime(ts, subs, tmp);
                return std::string(tmp);
        }
};
