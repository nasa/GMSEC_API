%module Mutex
%{
#include <gmsec4/util/Mutex.h>
using namespace gmsec::api;
%}

%rename("enter_mutex") enterMutex;
%rename("leave_mutex") leaveMutex;

%include "dox/Mutex_dox.i"
%include "dox/AutoMutex_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Mutex.h>
