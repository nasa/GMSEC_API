/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message
%{
#include <gmsec5/Message.h>
#include <list>
using namespace gmsec::api5;
%}

/* method containing list will be redefined */
%ignore gmsec::api5::Message::addFields(const gmsec::api5::util::List<Field*>&);

/* SWIG doesn't play well with these function headers
 * We'll tell SWIG to ignore them and define our own implementation using %extend
 */
%ignore gmsec::api5::Message::destroy(Message*&);

/* ignore overloaded addField() methods since many use types not applicable to Python */
%ignore gmsec::api5::Message::addField(const char*, const GMSEC_U8*, size_t);
%ignore gmsec::api5::Message::addField(const char*, bool);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_CHAR);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_F32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_F64);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I8);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I16);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_I64);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U8);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U16);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U32);
%ignore gmsec::api5::Message::addField(const char*, GMSEC_U64);
%ignore gmsec::api5::Message::addField(const char*, const char*);

/* ignore methods that do not make sense in the python context */
%ignore gmsec::api5::Message::getI16Value();
%ignore gmsec::api5::Message::getI32Value();
%ignore gmsec::api5::Message::getU16Value();
%ignore gmsec::api5::Message::getU32Value();
%ignore gmsec::api5::Message::getU64Value();

/* ignore method that cannot be translated to Python */
%ignore gmsec::api5::Message::operator=(const Message&);

%rename("get_schema_id") getSchemaID;
%rename("get_version") getVersion;
%rename("get_schema_level") getSchemaLevel;
%rename("is_compliant") isCompliant;
%rename("register_message_validator") registerMessageValidator;
%rename("set_field_value") setFieldValue;
%rename("set_config") setConfig;
%rename("get_config") getConfig;
%rename("set_subject") setSubject;
%rename("get_subject") getSubject;
%rename("set_kind") setKind;
%rename("get_kind") getKind;
%rename("add_field") addField;
%rename("add_fields") addFields;
%rename("clear_fields") clearFields;
%rename("clear_field") clearField;
%rename("has_field") hasField;
%rename("get_string_value") getStringValue;
%rename("get_boolean_value") getBooleanValue;
%rename("get_integer_value") getI64Value;
%rename("get_double_value") getF64Value;
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
%rename("to_xml") toXML;
%rename("to_json") toJSON;
%rename("get_size") getSize;
%rename("get_field_iterator") getFieldIterator;


%include "dox/Message_dox.i"
%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>
%include <gmsec5/Message.h>

%include <std_list.i>


%extend gmsec::api5::Message
{
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
