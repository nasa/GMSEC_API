/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field
%{
#include <gmsec4/field/Field.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::Field::destroyClone(Field*&);

%include "dox/Field_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/Field.h>

// Note: %extend places new additions into public member data
%extend gmsec::api::Field {
        static void CALL_TYPE destroyClone(Field* clone)
        {
                Field::destroyClone(clone);
        }
};

