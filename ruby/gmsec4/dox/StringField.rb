# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This is a specialized Field class that can be used to store a string value.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        field = Libgmsec_ruby::StringField.new("MY-STR-FIELD", "Hello Ruby")
	#    end
	#
	class StringField < Libgmsec_ruby::Field
		# Constructor.
		#
		# ==== Attributes
		#
	    # * +name+  - The name of the binary field.
	    # * +value+ - The string value of the field.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the name is nil, or is an empty string.
		# A GmsecException is thrown if the value is nil.
		#
		def initialize(name, value)
		end


		# Returns the value associated with the field.
		#
		# ==== Returns
		#
		# A string value.
		#
		def get_value
		end
	end
end
