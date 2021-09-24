# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This is custom GMSEC API extension of the Ruby Exception class.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        # ... some anomalous operation occurred ...
	#    rescue GmsecException => e
	#       puts("Error occurred: #{e.message}")
	#    end
	#
	class GmsecException < Exception
		##
		# Returns the description of the anomaly that caused the exception to be raised.
		#
		# ==== Returns
		#
		# A string containing the description of the exception.
		#
		def message
		end
	end
end
