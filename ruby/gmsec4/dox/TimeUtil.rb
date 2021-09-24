# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class defines the logging interface.
	#
	class TimeUtil
		##
		# Sleeps for the specified number of milliseconds.
		#
		# ==== Attributes
		#
		# * +time_in_ms+ - The period (in milliseconds) to sleep; minimum allowed period is 1 ms.
		#
		def self.millisleep(time_in_ms)
		end


		##
		# Returns the current time in seconds since Jan 1, 1970.
		#
		# ==== Returns
		#
		# A GMSEC_TimeSpec object is returned.
		#
		def self.get_current_time
		end


		##
		# Returns the current time in seconds since Jan 1, 1970 in the format of a floating point number.
		#
		# If the given time_spec object is non-nil, it will be populated with the current time.
		#
		# ==== Attributes
		#
		# * +time_spec+ - Optional GMSEC_TimeSpec object
		#
		# ==== Returns
		#
		# A float point number is returned.
		#
		def self.get_current_time_s(time_spec = nil)
		end


		##
		# Returns a GMSEC_TimeSpec object that is derived from the given time string.
		#
		# The time string must have a format similar to YYYY-DDD-HH:MM:SS[.s*]
		#
		# ==== Attributes
		#
		# * +time_str+ - The time string.
		#
		# ==== Returns
		#
		# A GMSEC_TimeSpec object is returned.
		#
		def self.get_char_time(time_str)
		end


		##
		# Returns time, derived from the given time string, since Jan 1, 1970, as a floating point value.
		#
		# The time string must have a format similar to YYYY-DDD-HH:MM:SS[.fffffffff]
		#
		# ==== Attributes
		#
		# * +time_str+ - The time string.
		#
		# ==== Returns
		#
		# A floating point time value is returned.
		#
		def self.get_char_time_s(time_str)
		end


		##
		# Returns a GMSEC-style time string.
		#
		# The time string has a format of YYYY-DDD-HH:MM:SS[.s*]. The given GMSEC_TimeSpec, when interpreted as an absolute time value,
		# represents the number of seconds elapsed since Jan 1, 1970.
		#
		# ==== Attributes
		#
		# * +time_spec+ - A GMSEC_TimeSpec object containing time in seconds and nanoseconds.
		# * +subs+      - Optional parameter to indicate the number of digits (0 through 9) to allow for subseconds; default is 3.
		#
		# ==== Returns
		#
		# A time string.
		#
		# ==== Example usage
		#
		#    current_time = Libgmsec_ruby::TimeUtil::format_time( Libgmsec_ruby::TimeUtil::get_current_time )
		#
		def self.format_time(time_spec, subs = 3)
		end


		##
		# Computes the time-delta between the two given times (t1 - t0).
		#
		# ==== Attributes
		#
		# * +t1+ - The second/later GMSEC_TimeSpec object.
		# * +t0+ - The first/earlier GMSEC_TimeSpec object.
		#
		# ==== Returns
		#
		# A GMSEC_TimeSpec object.
		#
		# ==== Example usage
		#
		#    t0_time = Libgmsec_ruby::TimeUtil::get_current_time
		#    # ... some delay
		#    t1_time = Libgmsec_ruby::TimeUtil::get_current_time
		#
		#    delta_time = Libgmsec_ruby::TimeUtil::diff( t1_time, t0_time )
		#
		def self.diff_time(t1, t0)
		end
	end
end
