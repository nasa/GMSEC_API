%module(directors="1") LogHandler

%{
#include <gmsec4/util/LogHandler.h>
using namespace gmsec::api;
%}

/* Turn on director mapping for the Callback class */
%feature("director") LogHandler;

%rename("OnMessage") onMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/LogHandler.h>

