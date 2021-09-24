/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message

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

%apply unsigned char INOUT[] {unsigned char* bin}
%apply int {size_t}

%{
#include <gmsec4/Message.h>
#include <list>
using namespace gmsec::api;
%}

/* Functions containing lists that will be redefined */
%ignore gmsec::api::Message::addFields(const gmsec::api::util::DataList<Field*>&);

%ignore gmsec::api::Message::operator=(const Message&);

%rename("AddConfig") addConfig;
%rename("GetConfig") getConfig;
%rename("GetSubject") getSubject;
%rename("SetSubject") setSubject;
%rename("GetKind") getKind;
%rename("AddField") addField;
%rename("AddFields") addFields;
%rename("ClearFields") clearFields;
%rename("ClearField") clearField;
%rename("GetIntegerValue") getIntegerValue;
%rename("GetUnsignedIntegerValue") getUnsignedIntegerValue;
%rename("GetDoubleValue") getDoubleValue;
%rename("GetStringValue") getStringValue;
%rename("HasField") hasField;
%rename("GetField") getField;
%rename("GetFieldType") getFieldType;
%rename("GetBinaryField") getBinaryField;
%rename("GetBooleanField") getBooleanField;
%rename("GetCharField") getCharField;
%rename("GetF32Field") getF32Field;
%rename("GetF64Field") getF64Field;
%rename("GetI16Field") getI16Field;
%rename("GetI32Field") getI32Field;
%rename("GetI64Field") getI64Field;
%rename("GetI8Field") getI8Field;
%rename("GetU16Field") getU16Field;
%rename("GetU32Field") getU32Field;
%rename("GetU64Field") getU64Field;
%rename("GetU8Field") getU8Field;
%rename("GetStringField") getStringField;
%rename("GetFieldCount") getFieldCount;
%rename("CopyFields") copyFields;
%rename("ToXML") toXML;
%rename("ToJSON") toJSON;
%rename("GetSize") getSize;
%rename("GetFieldIterator") getFieldIterator;

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Message.h>


PTR_REF_TYPEMAPS(Field, gmsec::api::Field*)
SWIG_STD_LIST_SPECIALIZE(Field, gmsec::api::Field*)

namespace std
{
    %template(FieldList) list<gmsec::api::Field*>;
}


%extend gmsec::api::Message {

    bool CALL_TYPE addFields(const std::list<gmsec::api::Field*>& fields) {
        gmsec::api::util::DataList<gmsec::api::Field*> dl_fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            dl_fields.push_back(*it);
        }

        return self->addFields(dl_fields);
    }
}
