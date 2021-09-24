/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%include <include/perlthread.swg>

%inline %{
        static void initPerlInterpreter()
        {
                GMSEC_SWIG_INIT_PERL_CONTEXT;
        }
%}

/*
 * This allows SWIG to handle Perl Strings for things such as BinaryField's
 * constructor
 */
%apply char* {unsigned char*};

%module libgmsec_perl

#pragma SWIG nowarn=314,451

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

%include util/TimeUtil.i
/*
Not current supported due to instability issues
%include util/LogHandler.i
*/
%include util/Log.i
%include util/Mutex.i
%include util/Condition.i

%include Message.i
%include SubscriptionInfo.i
/*
Not current supported due to instability issues
%include Callback.i
%include EventCallback.i
%include ReplyCallback.i
*/
%include Connection.i
%include Config.i
%include ConfigFile.i
%include ConfigFileIterator.i
%include MessageFieldIterator.i
%include HeartbeatGenerator.i

%include mist/ConnectionManager.i
/*
Not current supported due to instability issues
%include ConnectionManagerCallback.i
%include ConnectionManagerEventCallback.i
%include ConnectionManagerReplyCallback.i
*/
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
%include mist/message/MnemonicMessage.i
%include mist/message/DeviceMessage.i
%include mist/message/ProductFileMessage.i

%perlcode%{
        libgmsec_perl::initPerlInterpreter();

=pod

=for html <center><img src=../../gmsec_logo.png width="150" height="150"></center>

=head1 NAME

libgmsec_perl - Standard Perl Interface to the GMSEC API

=head1 DESCRIPTION

This is the Goddard Mission Services Evolution Center (GMSEC) Application Programming Interface (API) for the Perl programming language.

The GMSEC API is an Open Source message oriented middleware abstraction layer. Releases can be downloaded at L<http://sourceforge.net/projects/gmsec/files>.

Please visit the GMSEC home page L<https://gmsec.gsfc.nasa.gov> for an introduction to the project.

Customers are welcome to contact the GMSEC Team at L<mailto:gmsec-support@lists.nasa.gov> for assistance.

=head1 SEE ALSO

=for html <a href="Config.html">Config</a><br><br>

=for html <a href="ConfigFile.html">ConfigFile</a><br><br>

=for html <a href="ConfigFileIterator.html">ConfigFileIterator</a><br><br>

=for html <a href="Connection.html">Connection</a><br><br>

=for html <a href="Errors.html">Errors</a><br><br>

=for html <a href="Exception.html">Exception</a><br><br>

=for html <a href="HeartbeatGenerator.html">HeartbeatGenerator</a><br><br>

=for html <a href="Message.html">Message</a><br><br>

=for html <a href="MessageFieldIterator.html">MessageFieldIterator</a><br><br>

=for html <a href="Status.html">Status</a><br><br>

=for html <a href="SubscriptionInfo.html">SubscriptionInfo</a><br><br>

=for html <a href="field/Field.html">Field</a><br><br>

=for html <a href="field/BinaryField.html">BinaryField</a><br><br>

=for html <a href="field/BooleanField.html">BooleanField</a><br><br>

=for html <a href="field/CharField.html">CharField</a><br><br>

=for html <a href="field/F32Field.html">F32Field</a><br><br>

=for html <a href="field/F64Field.html">F64Field</a><br><br>

=for html <a href="field/I16Field.html">I16Field</a><br><br>

=for html <a href="field/I32Field.html">I32Field</a><br><br>

=for html <a href="field/I64Field.html">I64Field</a><br><br>

=for html <a href="field/I8Field.html">I8Field</a><br><br>

=for html <a href="field/StringField.html">StringField</a><br><br>

=for html <a href="field/U16Field.html">U16Field</a><br><br>

=for html <a href="field/U32Field.html">U32Field</a><br><br>

=for html <a href="field/U64Field.html">U64Field</a><br><br>

=for html <a href="field/U8Field.html">U8Field</a><br><br>

=for html <a href="util/Condition.html">Condition</a><br><br>

=for html <a href="util/Log.html">Log</a><br><br>

=for html <a href="util/Mutex.html">Mutex</a><br><br>

=for html <a href="util/TimeUtil.html">TimeUtil</a><br><br>

=for html <a href="mist/ConnectionManager.html">ConnectionManager</a><br><br>

=for html <a href="mist/Device.html">Device</a><br><br>

=for html <a href="mist/DeviceIterator.html">DeviceIterator</a><br><br>

=for html <a href="mist/DeviceParam.html">DeviceParam</a><br><br>

=for html <a href="mist/FieldSpecification.html">FieldSpecification</a><br><br>

=for html <a href="mist/MessageSpecification.html">MessageSpecification</a><br><br>

=for html <a href="mist/mist_defs.html">MIST Definitions</a><br><br>

=for html <a href="mist/Mnemonic.html">Mnemonic</a><br><br>

=for html <a href="mist/MnemonicIterator.html">MnemonicIterator</a><br><br>

=for html <a href="mist/MnemonicSample.html">MnemonicSample</a><br><br>

=for html <a href="mist/ProductFile.html">ProductFile</a><br><br>

=for html <a href="mist/ProductFileIterator.html">ProductFileIterator</a><br><br>

=for html <a href="mist/SchemaIDIterator.html">SchemaIDIterator</a><br><br>

=for html <a href="mist/ServiceParam.html">ServiceParam</a><br><br>

=for html <a href="mist/Specification.html">Specification</a><br><br>

=for html <a href="mist/SubscriptionInfo.html">SubscriptionInfo</a><br><br>

=for html <a href="mist/message/DeviceMessage.html">DeviceMessage</a><br><br>

=for html <a href="mist/message/MistMessage.html">MistMessage</a><br><br>

=for html <a href="mist/message/MnemonicMessage.html">MnemonicMessage</a><br><br>

=for html <a href="mist/message/ProductFileMessage.html">ProductFileMessage</a><br><br>

=cut
%}
