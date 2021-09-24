# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class wraps a name and value pair from a Config object.
	#
	# ==== Example Usage
	#
	#    cp = Libgmsec_ruby::ConfigPair.new
	#
	#    has_pair = config.get_first(cp)
	#
	#    while has_pair do
	#        puts("Name = #{cp.get_name}, Value = #{cp.get_value}")
	#
	#        has_pair = config.get_next(cp)
	#    end
	#
	# ==== See also
	# Config
	#
	class ConfigPair
		##
		# Returns the name associated with the configuration entry.
		#
		# ==== Returns
		#
		# A string containing the name of the configuration entry.
		#
		def get_name
		end


		##
		# Returns the value associated with the configuration entry.
		#
		# ==== Returns
		#
		# A string containing the value of the configuration entry.
		#
		def get_value
		end
	end
end
