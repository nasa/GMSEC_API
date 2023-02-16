/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#pragma SWIG nowarn=302
#pragma SWIG nowarn=451

%module libgmsec_ruby

%apply char* {unsigned char*};

%rename("_internal") "internal";

%ignore GMSEC_BOOL;
%ignore GMSEC_Callback;
%ignore GMSEC_EventCallback;
%ignore GMSEC_ReplyCallback;
%ignore GMSEC_Config;
%ignore GMSEC_ConfigEntry;
%ignore GMSEC_ConfigFile;
%ignore GMSEC_ConfigFileIterator;
%ignore GMSEC_Connection;
%ignore GMSEC_ConnectionEvent;
%ignore GMSEC_Field;
%ignore GMSEC_HeartbeatGenerator;
%ignore GMSEC_LogEntry;
%ignore GMSEC_LogHandler;
%ignore GMSEC_LogLevel;
%ignore GMSEC_Message;
%ignore GMSEC_MessageEntry;
%ignore GMSEC_MessageFactory;
%ignore GMSEC_MessageFieldIterator;
%ignore GMSEC_MessageValidator;
%ignore GMSEC_ResourceGenerator;
%ignore GMSEC_ResponseStatus;
%ignore GMSEC_SchemaIDIterator;
%ignore GMSEC_Status;
%ignore GMSEC_SchemaLevel;
%ignore GMSEC_Specification;
%ignore GMSEC_SubscriptionEntry;
%ignore GMSEC_SubscriptionInfo;


%include gmsec_version.i

%include gmsec5_defs.i

%include Errors.i
%include GmsecException.i
%include Status.i

%include field/Field.i
%include field/BinaryField.i
%include field/BooleanField.i
%include field/CharField.i
%include field/F32Field.i
%include field/F64Field.i
%include field/I16Field.i
%include field/I32Field.i
%include field/I64Field.i
%include field/I8Field.i
%include field/StringField.i
%include field/U8Field.i
%include field/U16Field.i
%include field/U32Field.i
%include field/U64Field.i

%inline %{
        gmsec::api5::BinaryField* to_BinaryField(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::BinaryField*>(field);
        }

        gmsec::api5::BooleanField* to_BooleanField(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::BooleanField*>(field);
        }

        gmsec::api5::CharField* to_CharField(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::CharField*>(field);
        }

        gmsec::api5::F32Field* to_F32Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::F32Field*>(field);
        }

        gmsec::api5::F64Field* to_F64Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::F64Field*>(field);
        }

        gmsec::api5::I16Field* to_I16Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::I16Field*>(field);
        }

        gmsec::api5::I32Field* to_I32Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::I32Field*>(field);
        }

        gmsec::api5::I64Field* to_I64Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::I64Field*>(field);
        }

        gmsec::api5::I8Field* to_I8Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::I8Field*>(field);
        }

        gmsec::api5::U16Field* to_U16Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::U16Field*>(field);
        }

        gmsec::api5::U32Field* to_U32Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::U32Field*>(field);
        }

        gmsec::api5::U64Field* to_U64Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::U64Field*>(field);
        }

        gmsec::api5::U8Field* to_U8Field(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::U8Field*>(field);
        }

        gmsec::api5::StringField* to_StringField(gmsec::api5::Field* field) {
                return dynamic_cast<gmsec::api5::StringField*>(field);
        }
%}

%include util/TimeUtil.i
%include util/LogHandler.i
%include util/Log.i

%include MessageFieldIterator.i
%include Message.i
%include MessageFactory.i
%include MessageValidator.i
%include SubscriptionInfo.i
%include Callback.i
%include EventCallback.i
%include Connection.i
%include Config.i
%include ConfigFile.i
%include ConfigFileIterator.i
%include FieldSpecification.i
%include MessageSpecification.i
%include HeartbeatGenerator.i
%include ResourceGenerator.i
%include SchemaIDIterator.i
%include Specification.i

%{
std::vector< swig::GC_VALUE > NativeVector;
%}

%template(NativeVector) std::vector< swig::GC_VALUE >;

