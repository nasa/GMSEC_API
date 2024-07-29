# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class is a container for GMSEC Messages. The methods of this class allow for the construction of
	# the data in a message.
	#
	# ==== See also
	#
	# Field
	#
	# Config
	#
	# MessageFieldIterator
	#
	# Connection
	#
	class Message

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


		##
		# Possible message kinds for a Message object.
		#
		class Kind
			##
			# ==== Enumerated values
			#
			# * +PUBLISH+ - For messages that will be published.
			# * +REQUEST+ - For request messages.
			# * +REPLY+   - For response/reply messages.
			#
			def Kind_Enum
			end
		end


		##
		# Creates a new Message object of unknown kind and with no associated fields.
		#
		# ==== Note
		# To make a copy of a Message object, use the following:
		#
		#    msg_copy = Libgmsec_ruby::Message.new( orig_msg )
		#
		def initialize()
		end


        ##
        # Acknowledges the processing of the message that has been received from the GMSEC Bus.
        #
        # Note: Some middleware can retain a delivered message in a queue until said time
        # the message has been acknowledged as being processed. Should the message not be
        # acknowledged, the middleware will deliver the message to another subscriber client.
		#
		# ==== See also
		#
		# Connection.receive
		#
		# Connection.request
        #
		def acknowledge()
		end


		##
		# Destroy a Message object.
		#
		# ==== Attributes
		#
		# * +msg+ - The Message object to release.
		#
		# ==== See also
		#
		# Connection.receive
		#
		# Connection.request
		#
		def self.destroy(msg)
		end


		##
		# Returns a string that identifies (if known) the schema from which the message is based.
		#
		# ==== Returns
		#
		# A schema ID (if known) that was used to build the message.
		#
		def get_schema_id()
		end


		##
		# Returns the specification version number that was used to create the message.
		#
		# ==== Returns
		#
		# A specification version number that was used to create the message.
		#
		def get_version()
		end


		##
		# Returns the specification schema level that was used to create the message.
		#
		# ==== Returns
		#
		# A specification schema level that was used to creae the message.
		#
		def get_schema_level()
		end


		##
		# The contents of the message are compared to the template to ensure the message is in compliance with the message specification.
		#
		# ==== Returns
		#
		# A Status object to indicate whether the message is valid. If invalid, a list of errors will be provided of issues found within the message.
		#
		def is_compliant()
		end


		##
		# Registers the given message validator to use when the message is checked for compliance.
		#
		# ==== Attributes
		#
		# * +validator+ - A custom MessageValidator object.
		#
		# ==== See also
		#
		# MessageValidator
		#
		def register_message_validator(validator)
		end


		##
        # Adds/replaces a field with the given name and value to the message. If
        # a field template for the schema in use is available for the named field, then
        # the value's type will be coerced to match that which is described in the field
        # template.
		#
		# ==== Notes
		#
        # This method is not very efficient; if the type of the field is known, consider calling add_field() instead.
		#
		# ==== Attributes
		#
		# * +name+  - The name of the field.
		# * +value+ - The value to associated with the field. Value can be string, integer, or double.
		#
		# ==== Returns
		#
		# Returns true if the field was replaced; false otherwise.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field name is null, is an empty string, or otherwise is non-compliant.
		# A GmsecException is thrown if the field value type cannot be converted into the type required by the field, or if the string is too big when converting to a char.
		#
		def set_field_value(name, value)
		end


		##
		# Sets the configuration items to the message.
		#
		# ==== Notes
		#
		# If a configuration item included within the given Config is already associated with the message,
		# then the message's configuration item will be replaced with the new item.
		#
		# * Only message-related configuration options are held by the Message object. These configuration
		# options are <b>NOT</b> included with the message when it is sent across the GMSEC Bus.
		#
		# ==== Attributes
		#
		# * +config+ - Config object that contains configuration values to add to the message.
		#
		def set_config(config)
		end


		##
		# Returns the configuration associcated with the message.
		#
		# ==== Returns
		#
		# A reference to the Config object held by the message.
		#
		def get_config
		end


		##
		# Accessor that can be used to change a message subject.
		#
		# ==== Attributes
		#
		# * +subject+ - The new message subject string.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or contains an empty string.
		#
		def set_subject(subject)
		end


		##
		# Allows for the setting of individual subject elements. The name of the elements are defined
		# by the message's corresponding message template. This value will be overridden by automatic subject
		# generation if the subject element is defined by an existing field in the message, or if the subject
		# was manually defined with setSubject.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the subject element.
		# * +value+ - The value of the subject element. An empty or null value will be seen as FILL in the subject line.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the name is NULL, an empty string, or does not match a subject element
		# name defined the message template, or if the message does not have a corresponding message template.
		#
		def set_subject_element(name, value)
		end


		##
		# Returns the subject identifying the message object.
		#
		# ==== Returns
		#
		# A string containing the message subject.
		#
		def get_subject
		end


		##
		# Accessor that can be used to set the message kind.
        #
        # Note: The API can auto-deduce the message kind, but there may be cases where
        # this method may be useful. For most common Use Cases, this method should not
        # need to be called.
		#
		# ==== Attributes
		#
		# * +kind+ - The message kind.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or contains an empty string.
		#
		def set_kind(kind)
		end


		##
		# Returns the message kind.
		#
		# ==== Returns
		#
		# One of the enumerated values from Message::Kind.
		#
		def get_kind
		end


		##
		# Adds the given (specialized) Field object to the message.
		#
		# Note: A copy of the given field is made; thus applications are free to dispose of the given Field object.
		#
		# ==== Attributes
		#
		# * +field+ - The (specialized) Field object to add to the message.
		#
		# ==== Returns
		#
		# Returns true if an existing field was overwritten; false otherwise.
		#
		# ==== Example usage:
		#
		#    msg.add_field( Libgmsec_ruby::StringField.new("MY-FIELD", "HELLO RUBY") )
		#
		def add_field(field)
		end

	
		##
		# Adds the FieldArray of fields to the message.
		#
		# Note: Copies of the given fields are made; thus applications are free to dispose of the given Field objects.
		#
		# ==== Attributes
		#
		# * +fields+ - A FieldArray of fields.
		#
		# ==== Returns
		#
		# Returns true if any existing field was overwritten; false otherwise.
		#
		# ==== Example usage:
		#
		#    fields = Libgmsec_ruby::FieldArray.new
		#    fields << Libgmsec_ruby::StringField.new("FIELD-1", "HELLO RUBY") )
		#    fields << Libgmsec_ruby::StringField.new("FIELD-2", "HELLO WORLD") )
		#
		#    msg.add_fields( fields )
		#
		def add_fields(fields)
		end


		##
		# Removes all fields from the message.
		#
		def clear_fields
		end


		##
		# Removes the field, specified by name, from the message.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to remove.
		#
		# ==== Returns
		#
		# Returns true if the field was removed; false otherwise.
		#
		def clear_field(field_name)
		end


		##
		# Reports whether a field with the given name is present in the message.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# Returns true if the field is present; false otherwise.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field name is nil or is an empty string.
		#
		def has_field(field_name)
		end


		##
		# Attempts to convert and return the field value into string representation.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# Returns the field value as a string.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be found, or if it cannot succesfully be converted to a string.
		#
		def get_string_value(field_name)
		end


		##
		# Attempts to convert the field value into boolean representation. Any non-zero value will be interpreted as true, otherwise as false.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# Returns the field value as a boolean value.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be found, or if it cannot succesfully be converted to a boolean.
		#
		def get_boolean_value(field_name)
		end


		##
		# Attempts to convert and return the field value into a 64-bit signed integer number.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# Returns the field value as an integer value.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be found, or if it cannot succesfully be converted to an integer.
		#
		def get_integer_value(field_name)
		end


		##
		# Attempts to convert and return the field value into a 64-bit floating point number.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# Returns the field value as a floating point number.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be found, or if it cannot succesfully be converted to a floating point number.
		#
		def get_double_value(field_name)
		end


		##
		# Returns a reference to the named field if it exists within the message; otherwise nil is returned.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to the named field, or nil if it does not exist within the message.
		#
		def get_field(field_name)
		end


		##
		# Returns the Field::FieldType associated with the named field.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A Field::FieldType enumerated value.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found.
		#
		def get_field_type(field_name)
		end


		##
		# Returns a reference to the BinaryField object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a BinaryField object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a BinaryField.
		#
		def get_binary_field(field_name)
		end


		##
		# Returns a reference to the BooleanField object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a BooleanField object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a BooleanField.
		#
		def get_boolean_field(field_name)
		end


		##
		# Returns a reference to the CharField object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a CharField object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a CharField.
		#
		def get_char_field(field_name)
		end


		##
		# Returns a reference to the F32Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to an F32Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent an F32Field.
		#
		def get_F32_field(field_name)
		end


		##
		# Returns a reference to the F64Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to an F64Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent an F64Field.
		#
		def get_F64_field(field_name)
		end


		##
		# Returns a reference to the I16Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to an I16Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent an I16Field.
		#
		def get_I16_field(field_name)
		end


		##
		# Returns a reference to the I32Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to an I32Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent an I32Field.
		#
		def get_I32_field(field_name)
		end


		##
		# Returns a reference to the I64Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to an I64Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent an I64Field.
		#
		def get_I64_field(field_name)
		end


		##
		# Returns a reference to the I8Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to an I8Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent an I8Field.
		#
		def get_I8_field(field_name)
		end


		##
		# Returns a reference to the StringField object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a StringField object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a StringField.
		#
		def get_string_field(field_name)
		end


		##
		# Returns a reference to the U16Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a U16Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a U16Field.
		#
		def get_U16_field(field_name)
		end


		##
		# Returns a reference to the U32Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a U32Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a U32Field.
		#
		def get_U32_field(field_name)
		end


		##
		# Returns a reference to the U64Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a U64Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a U64Field.
		#
		def get_U64_field(field_name)
		end


		##
		# Returns a reference to the U8Field object being referenced by name.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# A reference to a U8Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the named field cannot be found or does not represent a U8Field.
		#
		def get_U8_field(field_name)
		end


		##
		# Returns the number of fields present in the message.
		#
		# ==== Returns
		#
		# The number of fields present in the message.
		#
		def get_field_count
		end


		##
		# Copies the fields from the message into that of another message.
		#
		# ==== Attributes
		#
		# * +to_msg+ - The Message to where the fields will be copied.
		#
		def copy_fields(to_msg)
		end


		##
		# Dumps the message into an XML formatted string representation of the message.
		#
		# ==== Returns
		#
		# An XML string representation of the message object.
		#
		# ==== XML format example
		#
		#    <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
		#        <FIELD TYPE="BOOL" NAME="BOOL-FIELD">TRUE</FIELD>
		#        <FIELD TYPE="STRING" NAME="STRING-FIELD">This is a test of the Message::toXML function</FIELD>
		#    </MESSAGE>
		#
		def to_xml
		end


		##
		# Dumps the message into a JSON formatted string representation of the message.
		#
		# ==== Returns
		#
		# A JSON string representation of the message object.
		#
		# ==== JSON format example
		#
		#    {
		#        "MESSAGE":{
		#            "KIND":"PUBLISH",
		#            "SUBJECT":"GMSEC.MISSION.CONST.SAT.EVT.MSG",
		#            "FIELD":[
		#                {
		#                    "NAME":"BOOL-FIELD",
		#                    "TYPE":"BOOL",
		#                    "VALUE":"TRUE"
		#                },
		#                {
		#                    "NAME":"STRING-FIELD",
		#                    "TYPE":"STRING",
		#                    "VALUE":"This is a test of the Message::toJSON function"
		#                }
		#            ]
		#        }
		#    }
		#
		def to_json
		end


		##
		# Returns the physical storage size of the message in bytes.
		#
		# Note that if the message is sent across the GMSEC Bus, additional tracking fields are added to the
		# message, which will affect the size of the messages that is sent across the wire.
		#
		def get_size
		end


		##
		# Return the MessageFieldIterator associated with the Message object.
		#
		# This iterator will allow for applications to iterate over the Field objects stored within the message.
		# The iterator is reset each time get_field_iterator is called. The iterator itself is destroyed when the
		# Message object is destroyed.
		#
		# ==== Note
		# Only one MessageFieldIterator is associated with a Message object; multiple class to get_field_iterator
		# will return a reference to the same MessageFieldIterator object. Each call will reset the iterator.
		#
		# ==== Attributes
		#
		# * +selector+ - Choices are Selector_ALL_FIELDS, Selector_HEADER_FIELDS, or Selector_NON_HEADER_FIELDS; default is Selector_ALL_FIELDS.
		#
		# ==== Returns
		#
		# A reference to a MessageFieldIterator.
		#
		def get_field_iterator(selector = MessageFieldIterator.Selector_ALL_FIELDS)
		end
end
