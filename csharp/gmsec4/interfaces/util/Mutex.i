%module Mutex

%{
#include <gmsec4/util/Mutex.h>
using namespace gmsec::api;
%}

%rename("EnterMutex") enterMutex;
%rename("LeaveMutex") leaveMutex;
%rename("Enter") enter;
%rename("Leave") leave;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Mutex.h>
