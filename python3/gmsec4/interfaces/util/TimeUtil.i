%module TimeUtil
%{
#include <gmsec4/util/TimeUtil.h>
using namespace gmsec::api;
using namespace gmsec::api::util;
%}

%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char*);
%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, int, char*);
%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char*, int); /* deprecated */

%ignore gmsec::api::util::operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore gmsec::api::util::operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore gmsec::api::util::operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%ignore operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%rename("get_current_time") getCurrentTime;
%rename("get_current_time_s") getCurrentTime_s;
%rename("get_char_time") getCharTime;
%rename("get_char_time_s") getCharTime_s;
%rename("format_time") formatTime;
%rename("diff_time") diffTime;

%include "std_string.i"

%include "dox/TimeUtil_dox.i"
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
