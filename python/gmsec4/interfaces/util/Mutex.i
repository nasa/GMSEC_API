%module Mutex
%{
#include <gmsec4/util/Mutex.h>
using namespace gmsec::api;
%}

%include "dox/Mutex_dox.i"
%include "dox/AutoMutex_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Mutex.h>
