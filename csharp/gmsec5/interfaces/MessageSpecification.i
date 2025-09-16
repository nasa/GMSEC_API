/*
 * Copyright 2007-2025 United States Government as represented by the
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
#include <gmsec5/MessageSpecification.h>
#include <list>
using namespace gmsec::api5;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageSpecification::getFieldSpecifications() const;

%rename("GetSchemaID") getSchemaID;
%rename("GetSubjectTemplate") getSubjectTemplate;
%rename("GetFieldSpecifications") getFieldSpecifications;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageSpecification.h>

PTR_REF_TYPEMAPS(FieldSpecification, gmsec::api5::FieldSpecification*)
SWIG_STD_LIST_SPECIALIZE(FieldSpecification, gmsec::api5::FieldSpecification*)

namespace std
{
    %template(FieldSpecificationList) list<gmsec::api5::FieldSpecification*>;
}


%extend gmsec::api5::MessageSpecification {

    const std::list<gmsec::api5::FieldSpecification*> CALL_TYPE getFieldSpecifications()
    {
        const gmsec::api5::util::List<gmsec::api5::FieldSpecification*>& fieldSpecs = self->getFieldSpecifications();

        std::list<gmsec::api5::FieldSpecification*> newFieldSpecs;

        for (gmsec::api5::util::List<gmsec::api5::FieldSpecification*>::const_iterator it = fieldSpecs.begin(); it != fieldSpecs.end(); ++it)
        {
            newFieldSpecs.push_back(*it);
        }

        return newFieldSpecs;
    }
}

