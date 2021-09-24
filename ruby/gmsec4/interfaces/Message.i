/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Message

%include "std_vector.i"


%{
#include <gmsec4/Message.h>
#include <vector>
using namespace gmsec::api;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api::Message::addFields(const gmsec::api::util::DataList<Field*>&);

/* Not supported by Ruby */
%ignore gmsec::api::Message::operator=(const Message&);

/* These methods do not translate well for use in Ruby; for example, how to
   differentiate between a U16 and a U32.
 */
%ignore gmsec::api::Message::addField(const char*, GMSEC_BIN, size_t);
%ignore gmsec::api::Message::addField(const char*, bool);
%ignore gmsec::api::Message::addField(const char*, GMSEC_CHAR);
%ignore gmsec::api::Message::addField(const char*, GMSEC_F32);
%ignore gmsec::api::Message::addField(const char*, GMSEC_F64);
%ignore gmsec::api::Message::addField(const char*, GMSEC_I8);
%ignore gmsec::api::Message::addField(const char*, GMSEC_I16);
%ignore gmsec::api::Message::addField(const char*, GMSEC_I32);
%ignore gmsec::api::Message::addField(const char*, GMSEC_I64);
%ignore gmsec::api::Message::addField(const char*, GMSEC_U8);
%ignore gmsec::api::Message::addField(const char*, GMSEC_U16);
%ignore gmsec::api::Message::addField(const char*, GMSEC_U32);
%ignore gmsec::api::Message::addField(const char*, GMSEC_U64);
%ignore gmsec::api::Message::addField(const char*, const char*);

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
%rename("to_xml") toXML;
%rename("to_json") toJSON;
%rename("get_size") getSize;
%rename("get_field_iterator") getFieldIterator;


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/Message.h>


namespace std
{
    %template(FieldArray) vector<gmsec::api::Field*>;
}

%extend gmsec::api::Message {

    bool CALL_TYPE addFields(const std::vector<gmsec::api::Field*>& fields) {
        gmsec::api::util::DataList<gmsec::api::Field*> dl_fields;

        for (std::vector<gmsec::api::Field*>::const_iterator it = fields.begin(); it != fields.end(); ++it) {
            dl_fields.push_back(*it);
        }

        return self->addFields(dl_fields);
    }

}
