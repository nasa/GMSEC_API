/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") MessageValidator
%{
#include <gmsec4/mist/MessageValidator.h>
using namespace gmsec::api::mist;
%}

%feature("director") MessageValidator;

%rename("validate_message") validateMessage;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MessageValidator.h>

