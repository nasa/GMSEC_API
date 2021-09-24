# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# Enumeration of response status values that can be used with the Response Directive Message.
	#
	class ResponseStatus
		##
		# ==== Enumerated values
		#
		# * +ACKNOWLEDGEMENT+       - Basic acknowledgement
		# * +WORKING_KEEP_ALIVE+    - Still working on operation
		# * +SUCCESSFUL_COMPLETION+ - Operation successfully completed
		# * +FAILED_COMPLETION+     - Operation failed to complete
		# * +INVALID_REQUEST+       - The request for an operation was invalid
		# * +FINAL_MESSAGE+         - This message is the last in a set of responses
		#
		def ResponseStatus_Enum
		end
	end
end


class Libgmsec_ruby
	##
	# 2014 version of the GMSEC Interface Specification Document (ISD).
	#
	def GMSEC_ISD_2014_00 = 201400
	end

	##
	# 2016 version of the GMSEC Interface Specification Document (ISD).
	#
	def GMSEC_ISD_2016_00 = 201600
	end

	##
	# 2018 version of the Command and Control Message Specification (C2MS).
	#
	def GMSEC_ISD_2018_00 = 201800
	end

	##
	# 2019 version of the Command and Control Message Specification (C2MS).
	#
	def GMSEC_ISD_2019_00 = 201900
	end

	##
	# Current version of the Command and Control Message Specification (C2MS).
	#
	def GMSEC_ISD_CURRENT = GMSEC_ISD_2019_00
	end

	##
	# Wait indefinitely to receive a message.
	#
	def GMSEC_WAIT_FOREVER = -1
	end

	##
	# Do not republish request message upon reaching timeout.
	#
	def GMSEC_REQUEST_REPUBLISH_NEVER = -1
	end
end
