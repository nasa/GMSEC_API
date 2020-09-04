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

%rename("Millisleep") millisleep;
%rename("GetCurrentTime") getCurrentTime;
%rename("GetCurrentTime_s") getCurrentTime_s;
%rename("GetCharTime") getCharTime;
%rename("GetCharTime_s") getCharTime_s;
%rename("FormatTime") formatTime;
%rename("DiffTime") diffTime;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/TimeUtil.h>

%extend gmsec::api::util::TimeUtil {
        static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts)
        {
                char tmp[GMSEC_TIME_BUFSIZE];
                gmsec::api::util::TimeUtil::formatTime(ts, tmp);
                return std::string(tmp);
        }

        static std::string CALL_TYPE formatTime(const GMSEC_TimeSpec& ts, int subs)
        {
                char tmp[GMSEC_TIME_BUFSIZE];
                gmsec::api::util::TimeUtil::formatTime(ts, tmp, subs);
                return std::string(tmp);
        }
};
