/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module CharField

%{
#include <gmsec5/field/CharField.h>
using namespace gmsec::api5;
%}

%ignore gmsec::api5::CharField::getValue() const;


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/CharField.h>

%extend gmsec::api5::CharField {

    GMSEC_CHAR CALL_TYPE getValue()
    {
        // restrict to single-byte ASCII values
        return self->getValue() & 0x7F;
    }
};
