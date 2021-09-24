/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageSpecification

%include "std_list.i"

//pointer typemaps:
%define PTR_REF_TYPEMAPS(CSTYPE, CTYPE)
#if defined(SWIGCSHARP)
%typemap(ctype)    CTYPE *, CTYPE &    "void *"
%typemap(imtype)   CTYPE *, CTYPE &    "IntPtr"
%typemap(cstype)   CTYPE *, CTYPE &    "CSTYPE"
%typemap(csin)     CTYPE *, CTYPE &    "CSTYPE.getCPtr($csinput).Handle"
%typemap(csout, excode=SWIGEXCODE)    CTYPE *, CTYPE &    {
    IntPtr cPtr = $imcall;
    return (cPtr == IntPtr.Zero) ? null : new CSTYPE(cPtr, $owner);
}
%typemap(in)       CTYPE *, CTYPE &    %{ $1 = (CTYPE *)$input; %}
%typemap(out)      CTYPE *, CTYPE &    %{ $result = (void *)$1; %}
#endif
%enddef

%{
#include <gmsec4/mist/MessageSpecification.h>
#include <list>
using namespace gmsec::api::mist;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api::mist::MessageSpecification::getFieldSpecifications() const;

%rename("GetSchemaID") getSchemaID;
%rename("GetFieldSpecifications") getFieldSpecifications;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/MessageSpecification.h>

PTR_REF_TYPEMAPS(FieldSpecification, gmsec::api::mist::FieldSpecification*)
SWIG_STD_LIST_SPECIALIZE(FieldSpecification, gmsec::api::mist::FieldSpecification*)

namespace std
{
    %template(FieldSpecificationList) list<gmsec::api::mist::FieldSpecification*>;
}


%extend gmsec::api::mist::MessageSpecification {

    const std::list<gmsec::api::mist::FieldSpecification*> CALL_TYPE getFieldSpecifications()
    {
        const gmsec::api::util::DataList<gmsec::api::mist::FieldSpecification*>& fieldSpecs = self->getFieldSpecifications();

        std::list<gmsec::api::mist::FieldSpecification*> newFieldSpecs;

        for (gmsec::api::util::DataList<gmsec::api::mist::FieldSpecification*>::const_iterator it = fieldSpecs.begin(); it != fieldSpecs.end(); ++it)
        {
            newFieldSpecs.push_back(*it);
        }

        return newFieldSpecs;
    }
}

