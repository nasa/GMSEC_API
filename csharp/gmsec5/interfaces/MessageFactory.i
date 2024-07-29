/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module MessageFactory

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


%include <std_string.i>
%include <arrays_csharp.i>
#ifdef SWIGCSHARP
%include "csmodule.i"
#endif

%{
#include <gmsec5/MessageFactory.h>
#include <list>
using namespace gmsec::api5;
%}

/* Ignore methods pertinent to the C binding */
%ignore gmsec::api5::MessageFactory::registerMessageValidator(GMSEC_MessageValidator*);

/* Functions containing lists that will be redefined */
%ignore gmsec::api5::MessageFactory::setStandardFields(const gmsec::api5::util::List<Field*>&);

%rename("SetStandardFields") setStandardFields;
%rename("ClearStandardFields") clearStandardFields;
%rename("SetMessageConfig") setMessageConfig;
%rename("CreateMessage") createMessage;
%rename("FromData") fromData;
%rename("GetSpecification") getSpecification;
%rename("RegisterMessageValidator") registerMessageValidator;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/MessageFactory.h>


PTR_REF_TYPEMAPS(Field, gmsec::api5::Field*)
SWIG_STD_LIST_SPECIALIZE(Field, gmsec::api5::Field*)

namespace std
{
    %template(FieldList) list<gmsec::api5::Field*>;
}


%extend gmsec::api5::MessageFactory {

    void CALL_TYPE setStandardFields(const std::list<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::list<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->setStandardFields(list_fields);
    }
}
