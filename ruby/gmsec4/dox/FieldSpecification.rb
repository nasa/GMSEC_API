# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class stores the known attributes of a Field definition that is derived from a Message Specification template file.
	#
	# ==== See also
	#
	# Libgmsec_ruby::MessageSpecification
	#
	# Libgmsec_ruby::Specification.get_message_specifications
	#
	class FieldSpecification
		##
		# Returns the name for the Field Specification.
		#
		# ==== Returns
		#
		# The field name as a string.
		#
		def get_name
		end


		##
		# Returns the field type (e.g. STRING, I16, etc.) for the Field Specification.
		#
		# ==== Returns
		#
		# The field type as a string.
		#
		def get_field_type
		end


		##
		# Returns the mode (e.g. REQUIRED, OPTIONAL, etc.) for the Field Specification.
		#
		# ==== Returns
		#
		# The field mode as a string.
		#
		def get_mode
		end


		##
		# Returns the field class type for the Field Specification.
		#
		# ==== Returns
		#
		# The field classification type as a string.
		#
		def get_classification
		end


		##
		# Returns the default value, if any, for the Field Specification.
		#
		# If a value is not available, then an empty string is returned.
		#
		# ==== Returns
		#
		# The field value as a string.
		#
		def get_value
		end


		##
		# Returns the description of the Field Specification.
		#
		# ==== Returns
		#
		# The field description as a string.
		#
		def get_description
		end
	end
end
