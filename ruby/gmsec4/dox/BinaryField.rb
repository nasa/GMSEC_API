# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This is a specialized Field class that can be used to retain binary data.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        data = []
	#        for i in 0..1023 do
	#            data << (i % 256)
	#        end
	#
    #        field_name = "BINARY-FIELD"
	#        field_data = data.pack('C*').force_encoding('BINARY')
	#
	#        field = Libgmsec_ruby::BinaryField.new(field_name, field_data, field_data.length)
	#    end
	#
	class BinaryField < Libgmsec_ruby::Field
		# Constructor.
		#
		# ==== Attributes
		#
	    # * +name+ - The name of the binary field.
	    # * +data+ - The data for the field.
	    # * +data_length+ - The length of the data.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the name is nil, or is an empty string.
		#
		def initialize(name, data, data_length)
		end


		# Returns the data associated with the field.
		#
		# ==== Returns
		#
		# An array containing the binary data.
		#
		def get_value
		end


		# Returns the length of the data associated with the field.
		#
		# ==== Returns
		#
		# The length of the binary data contained within the field.
		#
		def get_length
		end
	end
end
