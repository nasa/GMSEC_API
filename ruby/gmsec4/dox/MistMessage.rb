# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This specialized message subclass shares all of the functionality of the base Libgmsec_ruby::Message class.
	# In addition, this subclass can be auto-populated with fields based on how the message schema identifies itself
	# with the given Specification.
	#
	# ==== See also
	#
	# * Libgmsec_ruby::Message
	# * Libgmsec_ruby::Specification
	#
	class MistMessage < Libgmsec_ruby::Message
		##
		# Constructor that initializes the message instance with a template determined by the schema ID and Specification.
		#
		# ==== Attributes
		#
		# * +subject+       - The subject to apply to the message.
		# * +schema_id+     - The schema ID string used to identify the message (e.g. MSG.HB, MSG.LOG, REQ.DIR, RESP.DIR, etc.)
		# * +specification+ - The Libgmsec_ruby::Specification to which this message will adhere.
		#
		# ==== Raises
		#
		# * A Libgmsec_ruby::GmsecException is thrown if the subject is nil or contains an empty string.
		# * A Libgmsec_ruby::GmsecException is thrown if the schema ID is not valid.
		#
		# ==== Example usage
		#
		#     spec = Libgmsec_ruby::Specification.new
		#     msg  = Libgmsec_ruby::MistMessage.new("GMSEC.MY-MISSION.MY-SAT-ID.MSG.HB.MY-COMPONENT", "MSG.HB", spec)
		#
		# ==== Other constructors
		#
		# * Constructor that initializes a MistMessage instance using a Config object.
		#
		#     Libgmsec_ruby::MistMessage.new(subject, schema_id, config, specification)
		#
		# * Constructor that attempts to construct using basic Libgmsec_ruby::Message.
		#
		#     Libgmsec_ruby::MistMessage.new(other_msg)
		#
		# * Constructor that attempts to construct using basic Message and Config (for a custom Specification).
		#
		#     Libgmsec_ruby::MistMessage.new(other_msg, spec_config)
		#
		# * Constructor that initializes a MistMessage using the given XML or JSON data representation of the message.
		#
		#     Libgmsec_ruby::MistMessage.new(specification, data)
		#
		# * Copy constructor.
		#
		#     Libgmsec_ruby::MistMessage.new(other_mist_msg)
		#
		# ==== See also
		#
		# * Libgmsec_ruby::Config
		# * Libgmsec_ruby::Message
		# * Libgmsec_ruby::Specification
		#
		def initialize(subject, schema_id, specification)
		end


		##
		# Sets the internal list of fields which are to be automatically placed in all
		# MistMessage objects that are created.
		#
		# Internal copies of the Fields are made, and thus ownership of the fields that
		# are provided are not retained by MistMessage.
		#
		# ==== Attributes
		#
		# * +standard_fields+ - A Libgmsec_ruby::FieldArray containing Field objects.
		#
		# ==== See also
		#
		# * Libgmsec_ruby::FieldArray
		# * clear_standard_fields
		#
		def self.set_standard_fields(standard_fields)
		end


		##
		# Destroys the list of standard fields that is held by MistMessage.
		#
		def self.clear_standard_fields
		end


		##
		# Returns the schema ID (e.g. MSG.HB) that identifies the schema from which the message is based on.
		#
		# ==== Returns
		#
		# The schema ID as a string.
		#
		def get_schema_id
		end


		##
		# Sets the value for a field, indicated by name, within the message.
		#
		# The value's type is automatically determined depending on the message schema being used. If the
		# field does not have a known type, then a new field using the value type given will be created
		# within the message.
		#
		# ==== Note: This is an overloaded method.
		#
		# * With a string field value:
		#
		#    set_value(field_name, string_field_value)
		#
		# * With an integer field value:
		#
		#    set_value(field_name, integer_field_value)
		#
		# * With a floating point field value:
		#
		#    set_value(field_name, double_field_value)
		#
		# ==== Attributes
		#
		# * +field_name+  - The name of the field.
		# * +field_value+ - The value to assign to the field.
		#
		# ==== Raises
		#
		# * A Libgmsec_ruby::GmsecException is thrown if the field name is nil or contains an empty string.
		# * A Libgmsec_ruby::GmsecException is thrown if the field value cannot be converted into the type required for the field.
		#
		def set_value(field_name, field_value)
		end
	end
end
