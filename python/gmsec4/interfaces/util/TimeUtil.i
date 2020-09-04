%module TimeUtil
%{
#include <gmsec4/util/TimeUtil.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::TimeUtil::formatTime(const GMSEC_TimeSpec&, char*, int); /* deprecated */

%ignore gmsec::api::util::operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore gmsec::api::util::operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore gmsec::api::util::operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%ignore operator==(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator>(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);
%ignore operator<(const GMSEC_TimeSpec&, const GMSEC_TimeSpec&);

%include "dox/TimeUtil_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/TimeUtil.h>

