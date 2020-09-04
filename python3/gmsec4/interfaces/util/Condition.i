%module Condition

%{
#include <gmsec4/util/Condition.h>
using namespace gmsec::api;
%}

%rename("get_mutex") getMutex;
%rename("wait_until") waitUntil;

%include "dox/Condition_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Condition.h>

