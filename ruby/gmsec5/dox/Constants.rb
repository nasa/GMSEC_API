# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this
end


class Libgmsec_ruby
	##
	# 2014 version of the GMSEC Interface Specification Document (ISD).
	#
	def GMSEC_MSG_SPEC_2014_00 = 201400
	end

	##
	# 2016 version of the GMSEC Interface Specification Document (ISD).
	#
	def GMSEC_MSG_SPEC_2016_00 = 201600
	end

	##
	# 2018 version of the Command and Control Message Specification (C2MS).
	#
	def GMSEC_MSG_SPEC_2018_00 = 201800
	end

	##
	# 2019 version of the Command and Control Message Specification (C2MS).
	#
	def GMSEC_MSG_SPEC_2019_00 = 201900
	end

	##
	# Current version of the Command and Control Message Specification (C2MS).
	#
	def GMSEC_MSG_SPEC_CURRENT = GMSEC_MSG_SPEC_2019_00
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
