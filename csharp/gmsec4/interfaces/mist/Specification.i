/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Specification

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
#include <gmsec4/mist/Specification.h>
using namespace gmsec::api::mist;
%}

// Turn on director mapping for the Specification package
%feature("director") Specification;

/* Functions containing lists that will be redefined */
%ignore gmsec::api::mist::Specification::getMessageSpecifications() const;

/* Ignore deprecated methods */
%ignore gmsec::api::mist::Specification::getTemplateXML(const char*, const char*);

/* Ignore C methods */
%ignore gmsec::api::mist::Specification::registerMessageValidator(GMSEC_MessageValidator*);

%rename("ValidateMessage") validateMessage;
%rename("GetSchemaIDIterator") getSchemaIDIterator;
%rename("GetVersion") getVersion;
%rename("GetSchemaLevel") getSchemaLevel;
%rename("GetMessageSpecifications") getMessageSpecifications;
%rename("RegisterMessageValidator") registerMessageValidator;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Specification.h>

PTR_REF_TYPEMAPS(MessageSpecification, gmsec::api::mist::MessageSpecification*)
SWIG_STD_LIST_SPECIALIZE(MessageSpecification, gmsec::api::mist::MessageSpecification*)

namespace std
{
    %template(MessageSpecificationList) list<gmsec::api::mist::MessageSpecification*>;
}


%extend gmsec::api::mist::Specification {

    const std::list<gmsec::api::mist::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::list<gmsec::api::mist::MessageSpecification*> newMsgSpecs;

        for (gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
}
