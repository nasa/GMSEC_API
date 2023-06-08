/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Field

%{
#include <gmsec5/field/Field.h>
#include <gmsec5/field/BinaryField.h>
#include <gmsec5/field/BooleanField.h>
#include <gmsec5/field/CharField.h>
#include <gmsec5/field/F32Field.h>
#include <gmsec5/field/F64Field.h>
#include <gmsec5/field/I16Field.h>
#include <gmsec5/field/I32Field.h>
#include <gmsec5/field/I64Field.h>
#include <gmsec5/field/I8Field.h>
#include <gmsec5/field/StringField.h>
#include <gmsec5/field/U16Field.h>
#include <gmsec5/field/U32Field.h>
#include <gmsec5/field/U64Field.h>
#include <gmsec5/field/U8Field.h>
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

%extend gmsec::api5::Field
{
    static gmsec::api5::BinaryField* toBinaryField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::BinaryField*>(field);
    }

    static gmsec::api5::BooleanField* toBooleanField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::BooleanField*>(field);
    }

    static gmsec::api5::CharField* toCharField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::CharField*>(field);
    }

    static gmsec::api5::F32Field* toF32Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::F32Field*>(field);
    }

    static gmsec::api5::F64Field* toF64Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::F64Field*>(field);
    }

    static gmsec::api5::I16Field* toI16Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I16Field*>(field);
    }

    static gmsec::api5::I32Field* toI32Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I32Field*>(field);
    }

    static gmsec::api5::I64Field* toI64Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I64Field*>(field);
    }

    static gmsec::api5::I8Field* toI8Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::I8Field*>(field);
    }

    static gmsec::api5::StringField* toStringField(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::StringField*>(field);
    }

    static gmsec::api5::U16Field* toU16Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U16Field*>(field);
    }

    static gmsec::api5::U32Field* toU32Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U32Field*>(field);
    }

    static gmsec::api5::U64Field* toU64Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U64Field*>(field);
    }

    static gmsec::api5::U8Field* toU8Field(gmsec::api5::Field* field) {
        return dynamic_cast<gmsec::api5::U8Field*>(field);
    }
};

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



=head3 toBinaryField

C<libgmsec_perl::Field::toBinaryField($field)>

        Down-casts a Field object to a BinaryField object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A BinaryField object, or undef if the given Field object is not a BinaryField.



=head3 toBooleanField

C<libgmsec_perl::Field::toBooleanField($field)>

        Down-casts a Field object to a BooleanField object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A BooleanField object, or undef if the given Field object is not a BooleanField.



=head3 toCharField

C<libgmsec_perl::Field::toCharField($field)>

        Down-casts a Field object to a CharField object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A CharField object, or undef if the given Field object is not a CharField.



=head3 toF32Field

C<libgmsec_perl::Field::toF32Field($field)>

        Down-casts a Field object to an F32Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An F32Field object, or undef if the given Field object is not an F32Field.



=head3 toF64Field

C<libgmsec_perl::Field::toF64Field($field)>

        Down-casts a Field object to an F64Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An F64Field object, or undef if the given Field object is not an F64Field.



=head3 toI16Field

C<libgmsec_perl::Field::toI16Field($field)>

        Down-casts a Field object to an I16Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An I16Field object, or undef if the given Field object is not an I16Field.



=head3 toI32Field

C<libgmsec_perl::Field::toI32Field($field)>

        Down-casts a Field object to an I32Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An I32Field object, or undef if the given Field object is not an I32Field.



=head3 toI64Field

C<libgmsec_perl::Field::toI64Field($field)>

        Down-casts a Field object to an I64Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An I64Field object, or undef if the given Field object is not an I64Field.



=head3 toI8Field

C<libgmsec_perl::Field::toI8Field($field)>

        Down-casts a Field object to an I8Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        An I8Field object, or undef if the given Field object is not an I8Field.



=head3 toStringField

C<libgmsec_perl::Field::toStringField($field)>

        Down-casts a Field object to a StringField object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A StringField object, or undef if the given Field object is not a StringField.



=head3 toU16Field

C<libgmsec_perl::Field::toU16Field($field)>

        Down-casts a Field object to a U16Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A U16Field object, or undef if the given Field object is not a U16Field.



=head3 toU32Field

C<libgmsec_perl::Field::toU32Field($field)>

        Down-casts a Field object to a U32Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A U32Field object, or undef if the given Field object is not a U32Field.



=head3 toU64Field

C<libgmsec_perl::Field::toU64Field($field)>

        Down-casts a Field object to a U64Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A U64Field object, or undef if the given Field object is not a U64Field.



=head3 toU8Field

C<libgmsec_perl::Field::toU8Field($field)>

        Down-casts a Field object to a U8Field object.

for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - a Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        A U8Field object, or undef if the given Field object is not a U8Field.



=cut
%}
