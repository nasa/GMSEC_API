%module Condition

%{
#include <gmsec4/util/Condition.h>
using namespace gmsec::api;
%}

%rename("GetMutex") getMutex;
%rename("Wait") wait;
%rename("WaitUntil") waitUntil;
%rename("Signal") signal;
%rename("Broadcast") broadcast;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Condition.h>

