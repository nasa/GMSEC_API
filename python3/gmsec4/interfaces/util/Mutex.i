%module Mutex
%{
#include <gmsec4/util/Mutex.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::AutoMutex;

%rename("enter_mutex") enterMutex;
%rename("leave_mutex") leaveMutex;

%include "dox/Mutex_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Mutex.h>
