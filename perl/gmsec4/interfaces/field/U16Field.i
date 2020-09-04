/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module U16Field
%{
#include <gmsec4/field/U16Field.h>
using namespace gmsec::api;
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/field/U16Field.h>

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::U16Field

=head1 DESCRIPTION

Specialized Field class that can be used to store an unsigned 16-bit value.

Note: This is a subclass of Field and thus inherits all of its public member data.

=head2 Public Types and Constants

=head3 FieldType

The various types of GMSEC Fields that are supported

Note: Field numbering was chosen to match that of API 3.X

=over

=item libgmsec_perl::Field::CHAR_TYPE

Field containing a character value.

=item libgmsec_perl::Field::BOOL_TYPE

Field containing a boolean value.

=item libgmsec_perl::Field::I16_TYPE

Field containing a signed 16-bit value.

=item libgmsec_perl::Field::U16_TYPE

Field containing an unsigned 16-bit value.

=item libgmsec_perl::Field:I32_TYPE

Field containing a signed 32-bit value.

=item libgmsec_perl::Field::U32_TYPE

Field containing an unsigned 32-bit value.

=item libgmsec_perl::Field::F32_TYPE

Field containing a 16-bit float value.

=item libgmsec_perl::Field::F64_TYPE

Field containing a 64-bit float value.

=item libgmsec_perl::Field::STRING_TYPE

Field containing a string.

=item libgmsec_perl::Field::BIN_TYPE

Field containing raw data.

=item libgmsec_perl::Field::I8_TYPE

Field containing a signed 8-bit value.

=item libgmsec_perl::Field::U8_TYPE

Field containing an unsigned 8-bit value.

=item libgmsec_perl::Field::I64_TYPE

Field containing a signed 64-bit value.

=item libgmsec_perl::Field::U64_TYPE

Field containing an unsigned 64-bit value.

=back

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::U16Field-E<gt>new($name, $value)>

        Constructor for creating specialized Field object containing an unsigned 16-bit value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - the name of the Field
        $value - the unsigned 16-bit value to store

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        An Exception is thrown if the name is NULL, or is an empty string.

C<libgmsec_perl::U16Field-E<gt>new($field)>

        Copy-constructor for instantiating a copy of another U16Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - the U16Field to copy

=head3 DESTROY

C<libgmsec_perl::U16Field-E<gt>DESTROY()>

        Destructor.

=head3 getValue

C<libgmsec_perl::U16Field-E<gt>getValue()>

        Exposes the underlying data held by the object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns a pointer to the underlying data held by this object.

=head3 toXML 

C<libgmsec_perl::Field-E<gt>toXML()>

        Converts the Field object into an XML string representation.

        Note: Implementation is specific to U16Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns an XML string.

=head3 toJSON

C<libgmsec_perl::Field-E<gt>toJSON()>

        Converts the Field object into a JSON string representation.

        Note: Implementation is specific to U16Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns an JSON string.

=head3 getName

C<libgmsec_perl::Field-E<gt>getName()>

        Returns the name associated with the Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        String containing Field name.

=head3 getType

C<libgmsec_perl::Field-E<gt>getType()>

        Returns the type of the Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        FieldType enumerated value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>See also:</b><br>

=for html &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="Field.html#fieldtype">FieldType</a>

=head3 isHeader

C<libgmsec_perl::Field-E<gt>isHeader()>

        Returns whether the Field represents a header field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns 1 (true) if a header field, 0 (false) otherwise.

=head3 getIntegerValue

C<libgmsec_perl::Field-E<gt>getIntegerValue()>

        Attempts to convert the field value into a signed 64-bit integer number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as an integer.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        An Exception is thrown if the field cannot be successfully converted into an integer.

=head3 getUnsignedIntegerValue

C<libgmsec_perl::Field-E<gt>getUnsignedIntegerValue()>

        Attempts to convert the field value into an unsigned 64-bit integer number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as an unsigned integer.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        An Exception is thrown if the field cannot be successfully converted into an unsigned integer.

=head3 getDoubleValue

C<libgmsec_perl::Field-E<gt>getDoubleValue()>

        Attempts to convert the field value into a 64-bit floating point number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a floating point number.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        An Exception is thrown if the field cannot be successfully converted into a double.

=head3 getStringValue

C<libgmsec_perl::Field-E<gt>getStringValue()>

        Attempts to convert the field value into a string representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

        An Exception is thrown if the field cannot be successfully converted to a string.

=cut
%}
