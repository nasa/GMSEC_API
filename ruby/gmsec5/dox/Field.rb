# Copyright 2007-2022 United States Government as represented by the
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
	end
end
