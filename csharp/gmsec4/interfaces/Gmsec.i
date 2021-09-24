/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%apply char* {unsigned char*};

%rename("_internal") "internal";

%ignore GMSEC_SubscriptionEntry;
%ignore GMSEC_BOOL;
%ignore GMSEC_ConfigEntry;
%ignore GMSEC_ConnectionEvent;
%ignore GMSEC_LogEntry;
%ignore GMSEC_MessageEntry;
%ignore GMSEC_SubscriptionInfo;

%module Gmsec

#pragma SWIG nowarn=302,451,844

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
        gmsec::api::BinaryField* CastToBinaryField(gmsec::api::Field* field) {
                return static_cast<gmsec::api::BinaryField*>(field);
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
%include mist/mist_defs.i
%include mist/FieldSpecification.i
%include mist/MessageSpecification.i
%include mist/MessageValidator.i
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

