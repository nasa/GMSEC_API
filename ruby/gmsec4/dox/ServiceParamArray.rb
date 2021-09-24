# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This is a specialized class for storing an array of ServiceParam objects.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        service_params = Libgmsec_ruby::ServiceParamArray.new
	#
	#        service_params << Libgmsec_ruby::ServiceParam.new("MY-PARAM", Libgmsec_ruby::I16Field.new("PARAM-VALUE", 1234))
	#
	#        ...
	#    end
	#
	class ServiceParamArray
		##
		# Constructor.
		#
		def initialize
		end


		##
		# Pushes the Libgmsec_ruby::ServiceParam object into the array of service parameters.
		#
		# ==== Attributes
		#
		# * +service_param+ - The Libgmsec_ruby::ServiceParam object to insert into the array.
		#
		def <<(service_param)
		end
	end
end
