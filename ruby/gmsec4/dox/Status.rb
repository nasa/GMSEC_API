# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This class provides status feedback from certain methods.
	#
	class Status
		##
		# Basic constructor where status is set to Libgmsec_ruby::NO_ERROR.
		#
		# ==== Other constructors
		#
		# * Constructor that initializes a Status instance with a class code, status code, an optional text and custom code.
		#
		#     Libgmsec_ruby::Status.new(status_class, status_code, text = nil, custom_code = 0)
		#
		# * Copy constructor.
		#
		#     Libgmsec_ruby::Status.new(other_status)
		#
		def initialize
		end

		
		##
		# Returns the verbose error string that contains the Status class, code, custom code, and reason.
		# The format is as follows:
		#
		#    [class,code,custom code] : reason
		#
		# ==== Returns
		#
		# A string containing the verbose description of the status.
		#
		def get
		end


		##
		# Returns the error class associated with the status.
		#
		def get_class
		end


		##
		# Returns the error code associated with the status.
		#
		def get_code
		end


		##
		# Returns the error (as a string) associated with the status.
		#
		def get_reason
		end


		##
		# Reports whether the Status object is reporting an error or not.
		#
		# ==== Returns
		#
		# Returns false if the status class is set to NO_ERROR; true otherwise.
		#
		def is_error
		end
	end
end
