# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class supports the iteration of a Specification object's loaded schema (message) templates.
	#
	# ==== See also
	#
	# Libgmsec_ruby::Specification.get_schema_id_iterator
	#
	class SchemaIDIterator
		##
		# Reports whether there are additional schema IDs that can be referenced.
		#
		# ==== Returns
		#
		# Returns true if additional schema IDs are available; false otherwise.
		#
		# ==== See also
		#
		# SchemaIDIterator.next
		#
		def has_next
		end


		##
		# Returns the next available schema ID.
		#
		# ==== Returns
		#
		# A schema ID string.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the iterator has reached the end (i.e. there are no more schema IDs).
		#
		# ==== See also
		#
		# SchemaIDIterator.has_next
		#
		def next
		end


		##
		# Resets the iterator to the beginning of the schema list that is maintained by Specification.
		#
		def reset
		end
	end
end
