/*
 * Copyright 2007-2024 United States Government as represented by the
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
#include <gmsec5/Specification.h>
#include <gmsec5/Message.h>
#include <list>
using namespace gmsec::api5;
%}

/* Ignore methods pertinent to the C binding */
%ignore gmsec::api5::Message::registerMessageValidator(GMSEC_MessageValidator*);

/* SWIG doesn't play well with these function headers
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api5::Message::destroy(Message*&);

/* Functions containing lists that will be redefined */
%ignore gmsec::api5::Message::getConfig() const;
%ignore gmsec::api5::Message::addFields(const gmsec::api5::util::List<Field*>&);
%ignore gmsec::api5::Message::operator=(const Message&);

%rename("Acknowledge") acknowledge;
%rename("Destroy") destroy;
%rename("GetSchemaID") getSchemaID;
%rename("GetVersion") getVersion;
%rename("GetSchemaLevel") getSchemaLevel;
%rename("IsCompliant") isCompliant;
%rename("RegisterMessageValidator") registerMessageValidator;
%rename("SetFieldValue") setFieldValue;
%rename("SetConfig") setConfig;
%rename("GetConfig") getConfig;
%rename("SetSubject") setSubject;
%rename("SetSubjectElement") setSubjectElement;
%rename("GetSubject") getSubject;
%rename("SetKind") setKind;
%rename("GetKind") getKind;
%rename("AddField") addField;
%rename("AddFields") addFields;
%rename("ClearFields") clearFields;
%rename("ClearField") clearField;
%rename("HasField") hasField;
%rename("GetStringValue") getStringValue;
%rename("GetBooleanValue") getBooleanValue;
%rename("GetI16Value") getI16Value;
%rename("GetI32Value") getI32Value;
%rename("GetI64Value") getI64Value;
%rename("GetU16Value") getU16Value;
%rename("GetU32Value") getU32Value;
%rename("GetU64Value") getU64Value;
%rename("GetF64Value") getF64Value;
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

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>
%include <gmsec5/Message.h>


PTR_REF_TYPEMAPS(Field, gmsec::api5::Field*)
SWIG_STD_LIST_SPECIALIZE(Field, gmsec::api5::Field*)

namespace std
{
    %template(FieldList) list<gmsec::api5::Field*>;
}


%extend gmsec::api5::Message {

    bool CALL_TYPE addFields(const std::list<gmsec::api5::Field*>& fields) {
        gmsec::api5::util::List<gmsec::api5::Field*> list_fields;

        for (std::list<gmsec::api5::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            list_fields.push_back(*it);
        }

        return self->addFields(list_fields);
    }

    static void CALL_TYPE destroy(Message* msg)
    {
        Message::destroy(msg);
    }
}
