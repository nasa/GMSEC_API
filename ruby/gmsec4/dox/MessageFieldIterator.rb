# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class is used to iterate over the Field objects of a Message object.
	#
	# ==== Example usage:
	#
	#    # All fields
	#    iter = msg.get_field_iterator
	#
	#    while iter.has_next do
	#        field = iter.next
	#
	#        puts("Have Field: #{field.to_xml}")
	#    end
	#
	#    # Header fields only
	#    iter = msg.get_field_iterator(Libgmsec_ruby::MessageFieldIterator::HEADER_FIELDS)
	#
	#    ...
	#
	#    # Non-header fields
	#    iter = msg.get_field_iterator(Libgmsec_ruby::MessageFieldIterator::NON_HEADER_FIELDS)
	#
	#    ...
	#
	# ==== See also
	#
	# Message
	#
	# Field
	#
	class MessageFieldIterator

		##
		# Possible iteration selectors.
		#
		class Selector
			##
			# ==== Enumerated values
			#
			# * +ALL_FIELDS+        - Used for iterating over all available fields (default selector).
			# * +HEADER_FIELDS+     - Used for iterating over header fields only.
			# * +NON_HEADER_FIELDS+ - Used for iterating over non-header fields only.
			#
			def Selector_Enum
			end
		end


		# Reports whether there is an additional Field that can be referenced using next().
		#
		# ==== Returns
		#
		# Returns true if another field is available, false otherwise.
		#
		# ==== See also
		#
		# MessageFieldIterator.next
		#
		def has_next
		end


		##
		# Returns a reference to the next available Field object.
		#
		# ==== Returns
		#
		# A reference to a Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the method is called and no more fields are available via the iterator.
		#
		# ==== See also
		#
		# MessageFieldIterator.has_next
		#
		def next
		end


		##
		# Resets the iterators to the beginning of the Field list that is maintained by the Message object.
		#
		def reset
		end
	end
end
