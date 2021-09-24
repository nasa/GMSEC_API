# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class represents a message definition derived from a message schema.
	#
	# ==== See also
	#
	# Libgmsec_ruby::FieldSpecification
	#
	# Libgmsec_ruby::Specification.get_message_specifications
	#
	class MessageSpecification
		##
		# Returns the schema (template) ID for this Message Specification.
		#
		# ==== Returns
		#
		# The schema ID as a string.
		#
		def get_schema_id
		end


		##
		# Returns an array of FieldSpecification objects for this Message Specification.
		#
		# ==== Returns
		#
		# An array of Libgmsec_ruby::FieldSpecification objects.
		#
		def get_field_specifications
		end
	end
end
