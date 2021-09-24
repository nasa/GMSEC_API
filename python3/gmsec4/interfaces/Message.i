/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message
%{
#include <gmsec4/Message.h>
#include <list>
using namespace gmsec::api;
%}

/* Functions containing lists that will be redefined */
%ignore gmsec::api::Message::addFields(const gmsec::api::util::DataList<Field*>&);
%ignore gmsec::api::Message::operator=(const Message&);
%ignore gmsec::api::Message::addField(const char*, const GMSEC_BIN, size_t);

%rename("add_config") addConfig;
%rename("get_config") getConfig;
%rename("get_subject") getSubject;
%rename("set_subject") setSubject;
%rename("get_kind") getKind;
%rename("add_field") addField;
%rename("add_fields") addFields;
%rename("clear_fields") clearFields;
%rename("clear_field") clearField;
%rename("get_integer_value") getIntegerValue;
%rename("get_unsigned_integer_value") getUnsignedIntegerValue;
%rename("get_double_value") getDoubleValue;
%rename("get_string_value") getStringValue;
%rename("has_field") hasField;
%rename("get_field") getField;
%rename("get_field_type") getFieldType;
%rename("get_binary_field") getBinaryField;
%rename("get_boolean_field") getBooleanField;
%rename("get_char_field") getCharField;
%rename("get_f32_field") getF32Field;
%rename("get_f64_field") getF64Field;
%rename("get_i16_field") getI16Field;
%rename("get_i32_field") getI32Field;
%rename("get_i64_field") getI64Field;
%rename("get_i8_field") getI8Field;
%rename("get_u16_field") getU16Field;
%rename("get_u32_field") getU32Field;
%rename("get_u64_field") getU64Field;
%rename("get_u8_field") getU8Field;
%rename("get_string_field") getStringField;
%rename("get_field_count") getFieldCount;
%rename("copy_fields") copyFields;
%rename("to_XML") toXML;
%rename("to_JSON") toJSON;
%rename("get_size") getSize;
%rename("get_field_iterator") getFieldIterator;


%include "dox/Message_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Message.h>

%include <std_list.i>


%extend gmsec::api::Message
{
    bool CALL_TYPE addFields(const std::list<gmsec::api::Field*>& fields) {
        gmsec::api::util::DataList<gmsec::api::Field*> dl_fields;

        for (std::list<gmsec::api::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            dl_fields.push_back(*it);
        }

        return self->addFields(dl_fields);
    }

    bool CALL_TYPE addField(char* name, PyObject* data, size_t length)
    {
        char* blob = PyByteArray_AsString(data);

        return self->addField((const char*) name, (const GMSEC_BIN) blob, length);
    }
}
