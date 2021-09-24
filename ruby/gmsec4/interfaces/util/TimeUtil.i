/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module TimeUtil

%include "std_string.i"

%{
#include <gmsec4/util/TimeUtil.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char* buffer);
%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, int, char* buffer);
%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char* buffer, int); /* deprecated */

%ignore gmsec::api::util::TimeUtil::operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);
%ignore gmsec::api::util::TimeUtil::operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);
%ignore gmsec::api::util::TimeUtil::operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);

%ignore operator==(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);
%ignore operator>(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);
%ignore operator<(const GMSEC_TimeSpec& time1, const GMSEC_TimeSpec& time0);

%rename("millisleep") millisleep;
%rename("get_current_time") getCurrentTime;
%rename("get_current_time_s") getCurrentTime_s;
%rename("get_char_time") getCharTime;
%rename("get_char_time_s") getCharTime_s;
%rename("format_time") formatTime;
%rename("diff_time") diffTime;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/TimeUtil.h>

%extend gmsec::api::util::TimeUtil {
    static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, int subs = 3)
    {
        char tmp[GMSEC_TIME_BUFSIZE];
        gmsec::api::util::TimeUtil::formatTime(ts, subs, tmp);
        return std::string(tmp);
    }
};
