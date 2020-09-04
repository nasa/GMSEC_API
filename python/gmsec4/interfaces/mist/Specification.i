/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Specification

%{
#include <gmsec4/mist/Specification.h>
using namespace gmsec::api::mist;
%}

// Turn on director mapping for the Specification package
%feature("director") Specification;

/*
%feature("director:except") {
    if ($error != NULL) {
        throw gmsec::api::Exception(gmsec::api::OTHER_ERROR, gmsec::api::OTHER_ERROR_CODE, SvPV_nolen($error));
    }
}

%exception {
    try { $action }
    catch (const gmsec::api::Exception& e) { SWIG_croak(e.what()); }
}

%apply SWIGTYPE *DISOWN { Specification *spec };
*/

%include "dox/Specification_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Specification.h>
