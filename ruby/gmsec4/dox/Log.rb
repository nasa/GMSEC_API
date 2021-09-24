# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class defines the logging interface.
	#
	class Log
		##
		# Registers a LogHandler that will be called for all logging levels.
		#
		# ==== Attributes
		#
		# * +log_handler+ - Instance of a custom LogHandler.
		#
		# ==== Notes
		#
		# * Do <b>not</b> rely on an anonymous custom LogHandler object; this type of object
		#   could be garbage collected without notice and cause a crash in the application.
		#
		# * An application should call register_handler(nil) before exiting.
		#
		def self.register_handler(log_handler)
		end


		##
		# Converts the given log level number of a string value.
		#
		# ==== Attributes
		#
		# * +level+ - The enumerated log level to convert.
		#
		# ==== Returns
		#
		# String with possibilities of "NONE", "ERROR", "SECURE", "WARNING", "INFO", "VERBOSE", "DEBUG", or "INVALID".
		#
		# ==== See also
		# Log.from_string
		#
		def self.to_string(level)
		end


		##
		# Converts the string representation of a log level to the appropriate log level number.
		#
		# ==== Attributes
		#
		# * +level+ - The string representation of a log level.
		#
		# ==== Returns
		#
		# String with possibilities of LogNONE, LogERROR, LogSECURE, LogWARNING, LogINFO, LogVERBOSE, or LogDEBUG.
		#
		# ==== See also
		# Log.to_string
		#
		def self.from_string(level)
		end


		##
		# Sets the maximum logging level.
		#
		# ==== Attributes
		#
		# * +level+ - The enumerated log level (LogNONE, LogERROR, LogSECURE, LogWARNING, LogINFO, LogVERBOSE, or LogDEBUG)
		#
		def self.set_reporting_level(level)
		end


		##
		# Returns the current logging level.
		#
		# ==== Returns
		#
		# An enumerated log level (LogNONE, LogERROR, LogSECURE, LogWARNING, LogINFO, LogVERBOSE, or LogDEBUG)
		#
		def self.get_reporting_level()
		end
	end
end
