/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module F64Field
%{
#include <gmsec5/field/F64Field.h>
using namespace gmsec::api5;
%}

%include <gmsec5/util/wdllexp.h>
%include <gmsec5/field/F64Field.h>

%extend gmsec::api5::F64Field {
    static F64Field* CALL_TYPE castField(Field* field)
    {
        F64Field* casted = dynamic_cast<F64Field*>(field);

        if (casted == NULL)
        {
            throw GmsecException(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Field cannot be casted to an F64Field");
        }

        return casted;
    }
};


%perlcode%{
=pod

=head1 NAME

libgmsec_perl::F64Field

=head1 DESCRIPTION

Specialized Field class that can be used to store a 64-bit floating point value.

Note: This is a subclass of Field and thus inherits all of its public member data.

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

=item $libgmsec_perl::Field::Type_BIN

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

=head3 new

C<libgmsec_perl::F64Field-E<gt>new($name, $value, $isHeader)>

        Constructor for creating specialized Field object containing 64-bit floating point value.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $name - the name of the Field
        $value - the 64-bit floating point value to store
        $isHeader - used to indicate if Field is a header field (default is 0)

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the name is NULL, or is an empty string.

C<libgmsec_perl::F64Field-E<gt>new($field)>

        Copy-constructor for instantiating a copy of another F64Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        $field - the F64Field to copy



=head3 DESTROY

C<libgmsec_perl::F64Field-E<gt>DESTROY()>

        Destructor.



=head3 getValue

C<libgmsec_perl::F64Field-E<gt>getValue()>

        Exposes the underlying data held by the object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns a pointer to the underlying data held by this object.



=head3 castField

C<libgmsec_perl::F64Field-E<gt>castField()>

        Converts the given Field object to an F64Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        Field object that represents an F64Field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An exception is thrown if the field object cannot be converted to an F64Field.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the F64Field representation of the given field object.



=head3 toXML 

C<libgmsec_perl::Field-E<gt>toXML()>

        Converts the Field object into an XML string representation.

        Note: Implementation is specific to F64Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns an XML string.



=head3 toJSON

C<libgmsec_perl::Field-E<gt>toJSON()>

        Converts the Field object into a JSON string representation.

        Note: Implementation is specific to F64Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns an JSON string.



=head3 getName

C<libgmsec_perl::Field-E<gt>getName()>

        Returns the name associated with the Field object.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        String containing Field name.



=head3 setName

C<libgmsec_perl::Field-E<gt>setName()>

        Sets the field name to the given string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

        Field name string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An exception is thrown if the field name is null or is an empty string.



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

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted into an integer.



=head3 getUnsignedIntegerValue

C<libgmsec_perl::Field-E<gt>getUnsignedIntegerValue()>

        Attempts to convert the field value into an unsigned 64-bit integer number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as an unsigned integer.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted into an unsigned integer.



=head3 getDoubleValue

C<libgmsec_perl::Field-E<gt>getDoubleValue()>

        Attempts to convert the field value into a 64-bit floating point number representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a floating point number.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted into a double.



=head3 getStringValue

C<libgmsec_perl::Field-E<gt>getStringValue()>

        Attempts to convert the field value into a string representation.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

        Returns the field value as a string.

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>GmsecExceptions:</b><br>

        An GmsecException is thrown if the field cannot be successfully converted to a string.

=cut
%}
