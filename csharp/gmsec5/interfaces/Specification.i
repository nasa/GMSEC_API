/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Specification

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
#include <gmsec5/Specification.h>
using namespace gmsec::api5;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api5::Specification::getSchemaIDIterator() const;
%ignore gmsec::api5::Specification::getMessageSpecifications() const;
%ignore gmsec::api5::Specification::getHeaderFieldNames() const;

%rename("GetSchemaIDIterator") getSchemaIDIterator;
%rename("GetVersion") getVersion;
%rename("GetSchemaLevel") getSchemaLevel;
%rename("GetMessageSpecifications") getMessageSpecifications;
%rename("GetHeaderFieldNames") getHeaderFieldNames;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>

PTR_REF_TYPEMAPS(MessageSpecification, gmsec::api5::MessageSpecification*)
SWIG_STD_LIST_SPECIALIZE(MessageSpecification, gmsec::api5::MessageSpecification*)

namespace std
{
    %template(MessageSpecificationList) list<gmsec::api5::MessageSpecification*>;
    %template(HeaderList) list<std::string>;
}


%extend gmsec::api5::Specification {

    SchemaIDIterator& GetSchemaIDIterator()
    {
        return self->getSchemaIDIterator();
    }

    const std::list<gmsec::api5::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api5::util::List<gmsec::api5::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::list<gmsec::api5::MessageSpecification*> newMsgSpecs;

        for (gmsec::api5::util::List<gmsec::api5::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
	
	const std::list<std::string> CALL_TYPE getHeaderFieldNames()
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames("DEFAULT");
		
		std::list<std::string> headerList;
		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerList.push_back(*it);
		}
		
		return headerList;
	}
	
	const std::list<std::string> CALL_TYPE getHeaderFieldNames(std::string schemaName)
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames(schemaName.c_str());
		
		std::list<std::string> headerList;
		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerList.push_back(*it);
		}
		
		return headerList;
	}
}

