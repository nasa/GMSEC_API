/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field

%{
#include <gmsec5/field/Field.h>
using namespace gmsec::api5;
%}

/* ignore methods that do not make sense in the Perl context */
%ignore gmsec::api5::Field::getI16Value();
%ignore gmsec::api5::Field::getI32Value();
%ignore gmsec::api5::Field::getU16Value();
%ignore gmsec::api5::Field::getU32Value();
%ignore gmsec::api5::Field::getU64Value();

%rename("getIntegerValue") getI64Value;
%rename("getDoubleValue")  getF64Value;

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/Field.h>


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Field

=head1 DESCRIPTION

Base class for all field types.

=head2 Public Types and Constants

=head3 FieldType

The various types of GMSEC Fields that are supported

=over

=item $libgmsec_perl::Field::Type_CHAR

Field containing a character value.

=item $libgmsec_perl::Field::Type_BOOL

Field containing a boolean value.

=item $libgmsec_perl::Field::Type_I16

Field containing a signed 16-bit value.

=item $libgmsec_perl::Field::Type_U16

Field containing an unsigned 16-bit value.

=item $libgmsec_perl::Field:Type_I32

Field containing a signed 32-bit value.

=item $libgmsec_perl::Field::Type_U32

Field containing an unsigned 32-bit value.

=item $libgmsec_perl::Field::Type_F32

Field containing a 16-bit float value.

=item $libgmsec_perl::Field::Type_F64

Field containing a 64-bit float value.

=item $libgmsec_perl::Field::Type_STRING

Field containing a string.

=item $libgmsec_perl::Field::Type_BINARY

Field containing raw data.

=item $libgmsec_perl::Field::Type_I8

Field containing a signed 8-bit value.

=item $libgmsec_perl::Field::Type_U8

Field containing an unsigned 8-bit value.

=item $libgmsec_perl::Field::Type_I64

Field containing a signed 64-bit value.

=item $libgmsec_perl::Field::Type_U64

Field containing an unsigned 64-bit value.

=back


=head2 Public Member Subroutines

=head3 getName

C<libgmsec_perl::Field-E<gt>getName()>

        Returns the name associated with the Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        String containing Field name.



=head3 getType

C<libgmsec_perl::Field-E<gt>getType()>

        Returns the type of the Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        libgmsec_perl::Field::Type_X enumerated value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Field.html#type">FieldType</a>



=head3 isHeader

C<libgmsec_perl::Field-E<gt>isHeader()>

        Returns whether the Field represents a header field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns 1 (true) if a header field, 0 (false) otherwise.



=head3 toXML 

C<libgmsec_perl::Field-E<gt>toXML()>

        Converts the Field object into an XML string representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns an XML string.



=head3 toJSON

C<libgmsec_perl::Field-E<gt>toJSON()>

        Converts the Field object into a JSON string representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns an JSON string.



=head3 getStringValue

C<libgmsec_perl::Field-E<gt>getStringValue()>

        Attempts to convert the field value into a string representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a string.



=head3 getBooleanValue

C<libgmsec_perl::Field-E<gt>getBooleanValue()>

        Attempts to convert the field value into a boolean representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a boolean

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted to a boolean.



=head3 getIntegerValue

C<libgmsec_perl::Field-E<gt>getIntegerValue()>

        Attempts to convert the field value into a signed 64-bit integer number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as an integer.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted into an integer.



=head3 getDoubleValue

C<libgmsec_perl::Field-E<gt>getDoubleValue()>

        Attempts to convert the field value into a 64-bit floating point number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a floating point number.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted into a double.



=cut
%}
