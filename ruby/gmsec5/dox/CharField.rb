# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This is a specialized Field class that can be used to store an ASCII char value.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        field = Libgmsec_ruby::CharField.new("MY-CHAR-FIELD", 'c')
	#    end
	#
	class CharField < Libgmsec_ruby::Field
		# Constructor.
		#
		# ==== Attributes
		#
	    # * +name+  - The name of the binary field.
	    # * +value+ - The ASCII char value of the field.
        # * +is_header+ - Used to indicate if Field is a header field (default is false).
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the name is nil, is an empty string, or is otherwise non-compliant.
		#
		def initialize(name, value, is_header)
		end


		# Returns the value associated with the field.
		#
		# ==== Returns
		#
		# An ASCII char value.
		#
		def get_value
		end
	end
end
