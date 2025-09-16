/*
 * Copyright 2007-2025 United States Government as represented by the
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

%module libgmsec_perl

/*
 * This allows SWIG to handle Perl Strings for things such as BinaryField's
 * constructor
 */
%apply char* {unsigned char*};

#pragma SWIG nowarn=314,451

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

/* Obsolete as of API 5.1, but leave in place to preserve binary compatibility */
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
/*
Not currently supported due to instability issues
%include util/LogHandler.i
*/
%include util/Log.i
%include util/Mutex.i
%include util/Condition.i

%include MessageFieldIterator.i
%include Message.i
%include MessageFactory.i
%include MessageValidator.i
%include SubscriptionInfo.i
%include Callback.i
%include EventCallback.i
/*
Not current supported due to instability issues
%include ReplyCallback.i
*/
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

%perlcode%{
    libgmsec_perl::initPerlInterpreter();

=pod

=for html <center><img src=../../gmsec_logo.png width="150" height="150"></center>

=head1 NAME

libgmsec_perl - Standard Perl Interface to the GMSEC API 5.x

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

=for html <a href="FieldSpecification.html">FieldSpecification</a><br><br>

=for html <a href="GmsecException.html">GmsecException</a><br><br>

=for html <a href="HeartbeatGenerator.html">HeartbeatGenerator</a><br><br>

=for html <a href="Message.html">Message</a><br><br>

=for html <a href="MessageFactory.html">MessageFactory</a><br><br>

=for html <a href="MessageFieldIterator.html">MessageFieldIterator</a><br><br>

=for html <a href="MessageSpecification.html">MessageSpecification</a><br><br>

=for html <a href="ResourceGenerator.html">ResourceGenerator</a><br><br>

=for html <a href="SchemaIDIterator.html">SchemaIDIterator</a><br><br>

=for html <a href="Specification.html">Specification</a><br><br>

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

=cut
%}
