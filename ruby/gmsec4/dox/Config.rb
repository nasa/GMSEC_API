# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class is a collection for managing connection configuration items.
	#
	# It encapsulates the parameters necessary for initialization Connections and setting middleware dependent
	# values in Messages. Used by Connection and Message, this class provides a generic means
	# of supplying initialization data without being Connection/Message specific.
	#
	# There are various constructors associated with this class. A Config object can be constructed with no
	# arguments, or with command-line arguments (i.e. ARGV), or with data that is represented as XML, JSON, or
	# a string containing name=value pairs.
	#
	# For example:
	#
	# XML:
	#     <CONFIG NAME="MyConfig">
	#         <PARAMETER NAME="mw-id">bolt</PARAMETER>
	#         <PARAMETER NAME="server">localhost</PARAMETER>
	#     </CONFIG>
	#
	# JSON:
	#     {"CONFIG":{"PARAMETER":[{"NAME":"mw-id","VALUE":"bolt"},{"NAME":"server","VALUE":"localhost"}]}}
	#
	# String data:
	#     "mw-id=bolt server=localhost"
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        config2 = Libgmsec_ruby::Config.new( ARGV )     # Config populated using command line arguments containing name=value pairs
	#
	#        begin
	#            connMgr = Libgmsec_ruby::ConnectionManager.new( config2 )
	#        rescue GmsecException => e
	#            puts("Error: #{e.message}")
	#        end
	#    end
	#
	class Config
		##
		# Creates a Config instance using the array of name=value pairs.
		#
		# ==== Attributes
		#
		# * +args+ - An array of arguments (e.g. ARGV).
		#
		# ==== Other constructors
		#
		# * Basic constructor that creates an empty Config object.
		#
		#     Libgmsec_ruby::Config.new
		#
		# * Constructor that populates the Config object using XML, JSON, or string data that define name=value pairs.
		#
		#     Libgmsec_ruby::Config.new(str_data)
		#
		# * Copy constructor.
		#
		#     Libgmsec_ruby::Config.new(other_config)
		#
		def initialize(args)
		end


		##
		# Adds a value to the Config by name.
		#
		# ==== Attributes
		#
		# * +name+   - The name of the value being added.
		# * +value+  - The value to associate with the name.
		#
		def add_value(name, value)
		end


		##
		# Clears a value from the Config by name.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the value being cleared.
		#
		def clear_value(name)
		end


		##
		# Gets a value by name. If a value cannot be referenced by name, then the
		# default value provided is returned.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the value being sought.
		# * +defaultValue+ - The default value to return if named item is not found.
		#
		# ==== Returns
		#
		# String value for the named configuration item, or the default value if not found.
		#
		def get_value(name, defaultValue = nil)
		end


		##
		# Gets a Boolean value referenced by name. If a value cannot be
		# referenced using the name, or converted to a Boolean, then the default value
		# provided is returned.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the value being sought.
		# * +defaultValue+ - The default value to return if named item is not found, or cannot be converted to a Boolean value.
		#
		# ==== Returns
		#
		# Boolean value for the named configuration item, or the default value if not found or cannot be converted to a Boolean value.
		#
		def get_boolean_value(name, defaultValue)
		end


		##
		# Gets an integer value referenced by name. If a value cannot be
		# referenced using the name, or converted to an integer, then the default value
		# provided is returned.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the value being sought.
		# * +defaultValue+ - The default value to return if named item is not found, or cannot be converted to an integer value.
		#
		# ==== Returns
		#
		# Integer value for the named configuration item, or the default value if not found or cannot be converted to an integer value.
		#
		def get_integer_value(name, defaultValue)
		end


		##
		# Gets a double value referenced by name. If a value cannot be
		# referenced using the name, or converted to a double, then the default value
		# provided is returned.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the value being sought.
		# * +defaultValue+ - The default value to return if named item is not found, or cannot be converted to a double value.
		#
		# ==== Returns
		#
		# Double value for the named configuration item, or the default value if not found or cannot be converted to a double value.
		#
		def get_double_value(name, defaultValue)
		end


		##
		# Clears all values from the Config.
		#
		def clear
		end


		##
		# Gets the first name and value for iteration.
		#
		# ==== Attributes
		#
		# * +cp+ - The ConfigPair object where to store the first name and value pair.
		#
		# ==== Returns
		#
		# Boolean value of true to indicate if name/value pair was found; false otherwise.
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
		#
		# Config.get_next
		#
		def get_first(cp)
		end


		##
		# Gets the next name and value for iteration.
		#
		# ==== Attributes
		#
		# * +cp+ - The ConfigPair object where to store the name and value pair.
		#
		# ==== Returns
		#
		# Boolean value of true to indicate if name/value pair was found; false otherwise.
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
		#
		# Config.get_first
		#
		def get_next(cp)
		end


		##
		# Merges the given Config with the current Config.
		#
		# ==== Attributes
		#
		# * +other_config+ - The Config object from where to copy configuration pairs.
		# * +overwrite_existing+ - Flag to indicate whether to overwrite any existing name/value pairs; default value is true.
		#
		def merge(other_config, overwrite_existing = true)
		end


		##
		# Dumps the Config into XML format. The format is the same as that which is parsed by from_xml().
		#
		# ==== Returns
		#
		# XML string.
		#
		# ==== See also
		#
		# Config.from_xml
		#
		def to_xml
		end


		##
		# Reads a configuration from an XML string. The format is the same as that which is generated by to_xml().
		#
		# ==== Example XML string
		#
		#    <CONFIG>
		#        <PARAMETER NAME="mw-id">bolt</PARAMETER>
		#        <PARAMETER NAME="server">localhost</PARAMETER>
		#    </CONFIG>
		#
		# ==== See also
		#
		# Config.to_xml
		#
		def from_xml
		end


		##
		# Dumps the Config into JSON format. The format is the same as that which is parsed by Config.new(data) constructor.
		#
		# ==== Returns
		#
		# JSON string.
		#
		# ==== Example JSON string
		#
		#    {"CONFIG":{"PARAMETER":[{"NAME":"mw-id","VALUE":"bolt"},{"NAME":"server","VALUE":"localhost"}]}}
		#
		def to_json
		end
	end
end
