# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class defines the logging interface.
	#
	class GMSEC_TimeSpec
		##
		# Constructor
		#
		# ==== Attributes
		#
		# * +seconds+     - Optional number of seconds since midnight of Jan 1, 1970; default is 0 (zero).
		# * +nanoseconds+ - Optional fractional portion of seconds since midnight of Jan 1, 1970; default is 0 (zero).
		#
		# ==== See also
		#
		# TimeUtil
		#
		def initialize(:seconds = 0, :nanoseconds = 0)
		end
	end
end
