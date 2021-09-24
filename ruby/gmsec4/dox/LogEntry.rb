# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This class represents the object that is passed to a LogHandler object when a message is logged.
	#
	class LogEntry
	    ##
		# The LogEntry structure is defined as containing:
		#
		#    time    : Libgmsec_ruby::GMSEC_TimeSpec
		#    level   : The severity level of the log level
		#    file    : The file name from where the log message originated from
		#    line    : The line number from within the file
		#    message : The log message
		#
		# ==== See also
		#
		# * Log
		# * LogHandler
		#
		def initialize
		end
	end
end
