# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This is a specialized class for storing information on a Simple Service parameter,
	# and used to generate GMSEC Simple Service messages by the Libgmsec_ruby::ConnectionManager.
	#
	class ServiceParam
		##
		# Constructor.
		#
		# ==== Attributes
		#
		# * +name+  - The name of the service parameter.
		# * +value+ - The Libgmsec_ruby::Field containing the value for the service parameter.
		#
		# ==== Other constructors
		#
		# * Copy constructor.
		#
		#     Libgmsec_ruby::ServiceParam.new(other_service_param)
		#
		def initialize(name, value)
		end


		##
		# Returns the name associated with the Service Parameter.
		#
		# ==== Returns
		#
		# A string containing the name of the ServiceParam.
		#
		def get_name
		end


		##
		# Returns the value associated with the Service Parameter.
		#
		# ==== Returns
		#
		# A Libgmsec_ruby::Field object containing the value associated with the ServiceParam.
		#
		def get_value
		end
	end
end
