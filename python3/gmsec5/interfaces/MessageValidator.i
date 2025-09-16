/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") MessageValidator
%{
#include <gmsec5/MessageValidator.h>
using namespace gmsec::api5;
%}

%feature("director") MessageValidator;

%rename("validate_message") validateMessage;

%include "dox/MessageValidator_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageValidator.h>

