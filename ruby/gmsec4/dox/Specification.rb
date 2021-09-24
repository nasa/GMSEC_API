# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class loads a series of message templates from a designated directory which is
	# used to create and validate message schemas.
	#
	# Each template has an associated ID that is used to identify the kind of message schema
	# the message template will be defining.
	#
	# ==== See also
	#
	# Libgmsec_ruby::Config
	#
	# Libgmsec_ruby::SchemaIDIterator
	#
	class Specification
        ##
        # Possible schema levels for representing message specifications.
        #
        class SchemaLevel
            ##
            # ==== Enumerated values
            #
            # * +LEVEL_0+ - C2MS
            # * +LEVEL_1+ - C2MS Extensions, or optionally user-defined
            # * +LEVEL_2+ - NASA/GMSEC Addendum, or optionally user-defined
            # * +LEVEL_3+ - User-defined
            # * +LEVEL_4+ - User-defined
            # * +LEVEL_5+ - User-defined
            # * +LEVEL_6+ - User-defined
            #
            def SchemaLevel_Enum
            end
        end


		##
		# Constructor that initializes the Specification using options provided with the configuration.
		#
		# ==== Attributes
		#
		# * +config+ - A Config object with configuration options.
		#
		# ==== Raises
		#
		# * A Libgmsec_ruby::GmsecException is thrown if illegal values are passed via the configuration.
		# * A Libgmsec_ruby::GmsecException is thrown if an error occurs while loading the message specification.
		#
		# ==== Example usage
		#
		#     config = Libgmsec_ruby::Config.new
		#     config.add_value("gmsec-schema-level", "0")                   # Level 0 == C2MS
		#     config.add_value("gmsec-specification-version", "201900")     # 201900  == Latest available message specification
		#
		#     spec = Libgmsec_ruby::Specification.new(config)
		#
		# ==== Other constructors
		#
		# * Constructor that initializes the Specification to the default message specification (NASA/GMSEC Addendum).
		#
		#     Libgmsec_ruby::Specification.new
		#
		# * Copy constructor
		#
		#     Libgmsec_ruby::Specification.new(other_spec)
		#
		def initialize(config)
		end


		##
		# Uses the message subject in the message registry to grab the appropriate message template
		# (based on its assigned schema ID).
		#
		# The contents of the message are then compared to the message template to ensure the message
		# complies with the specification.
		#
		# This method will also register the message with an appropriate schema ID if one does not
		# already exist in the registry.
		#
		# ==== Attributes
		#
		# * +msg+ - The Libgmsec_ruby::Message object to validate.
		#
		# ==== Raises
		#
		# A Libgmsec_ruby::GmsecException is thrown if the message fails to pass validation; a list of errors will be given of any issues found.
		#
		def validate_message(msg)
		end


		##
		# Returns a SchemaIDIterator which can be used to iterate over the schema IDs of the loaded message templates.
		#
		# Note: Only one SchemaIDIterator object is associated with a Specification object; multiple calls to this
		# method will return a reference to the same SchemaIDIterator object. Each call will reset the iterator.
		#
		# ==== Returns
		#
		# A reference to a Libgmsec_ruby::SchemaIDIterator object.
		#
		def get_schema_id_iterator
		end


		##
		# Returns the version of the message specification that is referenced by the Specification.
		#
		# ==== Returns
		#
		# The version number represented as an integer (e.g. 201900 for the 2019 message specification).
		#
		def get_version
		end


		##
		# Returns the schema level of the message specification that is referenced by the Specification.
		#
		# ==== Returns
		#
		# A SchemaLevel enumerated value.
		#
		def get_schema_level
		end


		##
		# Returns an array of MessageSpecification objects, each of which represents a message template.
		#
		# ==== Returns
		#
		# An array of Libgmsec_ruby::MessageSpecification objects.
		#
		def get_message_specifications
		end


		##
		# Registers the given MessageValidator to be used when message validation takes place.
		#
		# Note: Avoid using an anonymous MessageValidator object; retain a reference to ensure it
		# is not garbage collected. Otherwise the application will crash if the object has been destroyed.
		#
		# ==== Attributes
		#
		# * +validator+ - A custom Libgmsec_ruby::MessageValidator object.
		#
		# ==== Raises
		#
		# A Libgmsec_ruby::GmsecException is thrown if the validator object is nil.
		#
		def register_message_validator(validator)
		end
	end
end
