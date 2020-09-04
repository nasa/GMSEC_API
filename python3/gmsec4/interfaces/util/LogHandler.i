%module(directors="1") LogHandler
%{
#include <gmsec4/util/LogHandler.h>
using namespace gmsec::api;
%}

// Turn on director mapping for the Callback class
%feature("director") LogHandler;

%rename("on_message") onMessage;

%apply SWIGTYPE *DISOWN { LogHandler *cb };

%include "dox/LogHandler_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/LogHandler.h>

