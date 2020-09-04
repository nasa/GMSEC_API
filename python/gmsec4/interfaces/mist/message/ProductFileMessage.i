/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%rename(ProductFileMessage) gmsec::api::mist::message::ProductFileMessage;

%module ProductFileMessage
%{
#include <gmsec4/mist/message/ProductFileMessage.h>
using namespace gmsec::api::mist::message;
%}

%include "dox/ProductFileMessage_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/message/ProductFileMessage.h>
