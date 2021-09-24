/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/*
 * This allows SWIG to handle Python Strings for things such as BinaryField's
 * constructor
 */

%apply char* {unsigned char*};

%module libgmsec_python3

#pragma SWIG nowarn=451

%include gmsec_version.i

%include gmsec4_defs.i

%include Errors.i
%include Exception.i
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
        gmsec::api::BinaryField* to_BinaryField(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::BinaryField*>(field);
        }

        gmsec::api::BooleanField* to_BooleanField(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::BooleanField*>(field);
        }

        gmsec::api::CharField* to_CharField(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::CharField*>(field);
        }

        gmsec::api::F32Field* to_F32Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::F32Field*>(field);
        }

        gmsec::api::F64Field* to_F64Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::F64Field*>(field);
        }

        gmsec::api::I16Field* to_I16Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::I16Field*>(field);
        }

        gmsec::api::I32Field* to_I32Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::I32Field*>(field);
        }

        gmsec::api::I64Field* to_I64Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::I64Field*>(field);
        }

        gmsec::api::I8Field* to_I8Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::I8Field*>(field);
        }

        gmsec::api::U16Field* to_U16Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::U16Field*>(field);
        }

        gmsec::api::U32Field* to_U32Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::U32Field*>(field);
        }

        gmsec::api::U64Field* to_U64Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::U64Field*>(field);
        }

        gmsec::api::U8Field* to_U8Field(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::U8Field*>(field);
        }

        gmsec::api::StringField* to_StringField(gmsec::api::Field* field) {
                return dynamic_cast<gmsec::api::StringField*>(field);
        }
%}

%include util/TimeUtil.i
%include util/LogHandler.i
%include util/Log.i
%include util/Mutex.i
%include util/Condition.i

%include Message.i
%include SubscriptionInfo.i
%include Callback.i
%include EventCallback.i
%include ReplyCallback.i
%include Connection.i
%include Config.i
%include ConfigFile.i
%include ConfigFileIterator.i
%include MessageFieldIterator.i
%include HeartbeatGenerator.i

%include mist/ConnectionManagerCallback.i
%include mist/ConnectionManagerEventCallback.i
%include mist/ConnectionManagerReplyCallback.i
%include mist/ConnectionManager.i
%include mist/Device.i
%include mist/DeviceIterator.i
%include mist/DeviceParam.i
%include mist/FieldSpecification.i
%include mist/MessageSpecification.i
%include mist/MessageValidator.i
%include mist/mist_defs.i
%include mist/Mnemonic.i
%include mist/MnemonicIterator.i
%include mist/MnemonicSample.i
%include mist/ProductFile.i
%include mist/ProductFileIterator.i
%include mist/SchemaIDIterator.i
%include mist/ServiceParam.i
%include mist/Specification.i
%include mist/SubscriptionInfo.i
%include mist/message/MistMessage.i
%include mist/message/DeviceMessage.i
%include mist/message/MnemonicMessage.i
%include mist/message/ProductFileMessage.i

