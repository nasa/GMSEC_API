# Copyright 2007-2021 United States Government as represented by the
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
        class FieldType < ActiveRecord::Base
            ##
            # ==== Enumerated values
            #
            # * +CHAR_TYPE+    - Field containing an ASCII character value.
            # * +BOOL_TYPE+    - Field containing a boolean value.
            # * +I16_TYPE+     - Field containing a signed 16-bit value.
            # * +U16_TYPE+     - Field containing an unsigned 16-bit value.
            # * +I32_TYPE+     - Field containing a signed 32-bit value.
            # * +U32_TYPE+     - Field containing an unsigned 32-bit value.
            # * +F32_TYPE+     - Field containing a 32-bit floating point value.
            # * +F64_TYPE+     - Field containing a 64-bit floating point value.
            # * +STRING_TYPE+  - Field containing a string.
            # * +BIN_TYPE+     - Field containing raw binary data.
            # * +I8_TYPE+      - Field containing a signed 8-bit value.
            # * +U8_TYPE+      - Field containing an unsigned 8-bit value.
            # * +I64_TYPE+     - Field containing a signed 64-bit value.
            # * +U64_TYPE+     - Field containing an unsigned 64-bit value.
            #
            def FieldType
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


		# Sets the field name with the given string.
		#
		# ==== Attributes
		#
		# * +name+ - The name to assign to the field.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field name is nil or contains an empty string.
		#
		def set_field_name(name)
		end


		# Returns the type of the field.
		#
		# ==== Returns
		#
		# A FieldType enumerated value.
		#
		def get_field_type
		end


		# Used to indicate if the field is a header field (see C2MS definition).
		#
		# ==== Attributes
		#
		# * +flag+ - Use true to indicate a header field; false otherwise.
		#
		def set_header(flag)
		end


		# Can be used to query if the field is a header field (see C2MS definition).
		#
		# ==== Returns
		#
		# Returns true if the field represents a header field; false otherwise.
		#
		def is_header()
		end


		# Converts the field info into an XML string representation.
		#
		# ==== Returns
		#
		# An XML string.
		#
		def to_xml
		end


		# Converts the field info into a JSON string representation.
		#
		# ==== Returns
		#
		# A JSON string.
		#
		def to_json
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


		# Attempts to convert the field value into a 64-bit unsigned integer representation.
		#
		# ==== Returns
		#
		# Return the field value as an unsigned integer.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be successfully converted to an unsigned integer.
		#
		def get_unsigned_integer_value
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


		# Attempts to convert the field value into string representation.
		#
		# ==== Returns
		#
		# Return the field value as a string.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be successfully converted to a string.
		#
		def get_string_value
		end


		# Creates an exact clone of the existing field object.
		#
		# ==== Returns
		# A clone of a Field object.
		#
		def clone
		end
	end
end
