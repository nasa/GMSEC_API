# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This is the abstract base Field class for all field types.
	#
	class Field

        ##
        # Possible field types for a Field object.
        #
        class Type < ActiveRecord::Base
            ##
            # ==== Enumerated values
            #
            # * +CHAR+    - Field containing an ASCII character value.
            # * +BOOL+    - Field containing a boolean value.
            # * +I16+     - Field containing a signed 16-bit value.
            # * +U16+     - Field containing an unsigned 16-bit value.
            # * +I32+     - Field containing a signed 32-bit value.
            # * +U32+     - Field containing an unsigned 32-bit value.
            # * +F32+     - Field containing a 32-bit floating point value.
            # * +F64+     - Field containing a 64-bit floating point value.
            # * +STRING+  - Field containing a string.
            # * +BINARY+  - Field containing raw binary data.
            # * +I8+      - Field containing a signed 8-bit value.
            # * +U8+      - Field containing an unsigned 8-bit value.
            # * +I64+     - Field containing a signed 64-bit value.
            # * +U64+     - Field containing an unsigned 64-bit value.
            #
            def Type
            end
        end


		# Returns the name associated with the field.
		#
		# ==== Returns
		#
		# A string containing the field name.
		#
		def get_field_name
		end


		# Returns the type of the field.
		#
		# ==== Returns
		#
		# A Type enumerated value.
		#
		def get_field_type
		end


		# Can be used to query if the field is a header field (see C2MS definition).
		#
		# ==== Returns
		#
		# Returns true if the field represents a header field; false otherwise.
		#
		def is_header()
		end


		# Can be used to query if the field is a GMSEC API tracking field.
		#
		# ==== Returns
		#
		# Returns true if the field represents a tracking field; false otherwise.
		#
		def is_tracking()
		end


		# Converts the field info into an XML string representation.
		#
		# ==== Returns
		#
		# An XML string.
		#
		def toxml
		end


		# Converts the field info into a JSON string representation.
		#
		# ==== Returns
		#
		# A JSON string.
		#
		def tojson
		end


		# Attempts to convert the field value into string representation.
		#
		# ==== Returns
		#
		# Return the field value as a string.
		#
		def get_string_value
		end


		# Attempts to convert the field value into a boolean representation.
		#
		# ==== Returns
		#
		# Return the field value as a boolean.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be successfully converted to a boolean.
		#
		def get_boolean_value
		end


		# Attempts to convert the field value into a 64-bit integer representation.
		#
		# ==== Returns
		#
		# Return the field value as an integer.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be successfully converted to an integer.
		#
		def get_integer_value
		end


		# Attempts to convert the field value into a 64-bit floating point representation.
		#
		# ==== Returns
		#
		# Return the field value as a floating point.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be successfully converted to a floating point.
		#
		def get_double_value
		end


        # Downcasts the Field object into a BinaryField.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a BinaryField.
		#
		# ==== Returns
		#
        # A BinaryField object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a BinaryField object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toBinaryField(field)
        end


        # Downcasts the Field object into a BooleanField.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a BooleanField.
		#
		# ==== Returns
		#
        # A BooleanField object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a BooleanField object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toBooleanField(field)
        end


        # Downcasts the Field object into a CharField.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a CharField.
		#
		# ==== Returns
		#
        # A CharField object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a CharField object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toCharField(field)
        end


        # Downcasts the Field object into an F32Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to an F32Field.
		#
		# ==== Returns
		#
        # An F32Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to an F32Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toF32Field(field)
        end


        # Downcasts the Field object into an F64Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to an F64Field.
		#
		# ==== Returns
		#
        # An F64Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to an F64Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toF64Field(field)
        end


        # Downcasts the Field object into an I16Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to an I16Field.
		#
		# ==== Returns
		#
        # An I16Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to an I16Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toI16Field(field)
        end


        # Downcasts the Field object into an I32Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to an I32Field.
		#
		# ==== Returns
		#
        # An I32Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to an I32Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toI32Field(field)
        end


        # Downcasts the Field object into an I64Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to an I64Field.
		#
		# ==== Returns
		#
        # An I64Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to an I64Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toI64Field(field)
        end


        # Downcasts the Field object into an I8Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to an I8Field.
		#
		# ==== Returns
		#
        # An I8Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to an I8Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toI8Field(field)
        end


        # Downcasts the Field object into a StringField.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a StringField.
		#
		# ==== Returns
		#
        # A StringField object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a StringField object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toStringField(field)
        end


        # Downcasts the Field object into a U16Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a U16Field.
		#
		# ==== Returns
		#
        # A U16Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a U16Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toU16Field(field)
        end


        # Downcasts the Field object into a U32Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a U32Field.
		#
		# ==== Returns
		#
        # A U32Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a U32Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toU32Field(field)
        end


        # Downcasts the Field object into a U64Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a U64Field.
		#
		# ==== Returns
		#
        # A U64Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a U64Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toU64Field(field)
        end


        # Downcasts the Field object into a U8Field.
        #
        # ==== Attributes
        #
        # * +field+ - The field object to downcast to a U8Field.
		#
		# ==== Returns
		#
        # A U8Field object.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if the given Field object is null.
        #
        # A GmsecException is thrown if the given Field object cannot be converted to a U8Field object.
        #
        # A GmsecException is thrown if the given object is not a Field object.
        #
        def self.toU8Field(field)
        end
	end
end
