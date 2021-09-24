# Copyright 2007-2021 United States Government as represented by the
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
	# Connection, ConnectionManager
	#
	class Message

		##
		# Possible message kinds for a Message object.
		#
		class MessageKind
			##
			# ==== Enumerated values
			#
			# * +PUBLISH+ - For messages that will be published.
			# * +REQUEST+ - For request messages.
			# * +REPLY+   - For response/reply messages.
			#
			def MessageKind_Enum
			end
		end


		##
		# Creates a new Message object.
		#
		# ==== Attributes
		#
		# * +subject+ - The subject string to identify the message.
		# * +kind+    - A Message::MessageKind enumerated value to indicate the kind of message to instantiate.
		# * +config+  - Optional Config object that can be used to configure the message itself.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or contains an empty string.
		#
		# ==== Note
		# To make a copy of a Message object, use the following:
		#
		#    msg_copy = Libgmsec_ruby::Message.new( orig_msg )
		#
		def initialize(subject, kind, config = nil)
		end


		##
		# Message constructor that accepts XML or JSON formatted data representing a message.
		#
		# ==== Attributes
		#
		# * +data+ - An XML or JSON formatted string representation of a message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the given data is nil or cannot be parsed as a message in either XML or JSON format.
		#
		# ==== Usage:
		#
		#    msg1 = Libgmsec_ruby::Message.new( xml_data )
		#    msg2 = Libgmsec_ruby::Message.new( json_data )
		#
		# ==== Example XML representation of a message
		#
		#     <MESSAGE SUBJECT="GMSEC.FOO.BAR" KIND="PUBLISH">
		#         <CONFIG>
		#             <PARAMETER NAME="tracking">false</PARAMETER>
		#         </CONFIG>
		#         <FIELD NAME="FIELD-1" TYPE="STRING">ONE</FIELD>
		#         <FIELD NAME="FIELD-2" TYPE="I16">2</FIELD>
		#         <FIELD NAME="FIELD-3" TYPE="F64">3.14</FIELD>
		#     </MESSAGE>
		#
		# ==== Example JSON representation of a message
		#
		#     {"MESSAGE":{"SUBJECT":"GMSEC.FOO.BAR","KIND":"PUBLISH","CONFIG":{"PARAMETER":[{"NAME":"tracking","VALUE":"false"}]},"FIELD":[{"NAME":"FIELD-1","TYPE":"STRING","VALUE":"ONE"},{"NAME":"FIELD-2","TYPE":"I16","VALUE":"2"},{"NAME":"FIELD-3","TYPE":"F64","BITS":"40091EB851EB851F","VALUE":"3.14"}]}}
		#
		def initialize2(data)
		end


		##
		# Adds configuration items to the message.
		#
		# ==== Notes
		*
		# * If a configuration item included within the given Config is already associated with the message,
		# then the message's configuration item will be replaced with the new item.
		#
		# * Only message-related configuration options are held by the Message object. These configuration
		# options are <b>NOT</b> included with the message when it is sent across the GMSEC Bus.
		#
		# ==== Attributes
		#
		# * +config+ - Config object that contains configuration values to add to the message.
		#
		def add_config(config)
		end


		##
		# Returns a reference to the Config object held by the message.
		#
		# ==== Returns
		#
		# A reference to the Config object held by the message.
		#
		def get_config
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
		# Returns the message kind.
		#
		# ==== Returns
		#
		# One of the enumerated values from Message::MessageKind.
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
		# Attempts to convert and return the field value into a 64-bit unsigned integer number.
		#
		# ==== Attributes
		#
		# * +field_name+ - The name of the field to reference.
		#
		# ==== Returns
		#
		# Returns the field value as an unsigned integer value.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the field cannot be found, or if it cannot succesfully be converted to an unsigned integer.
		#
		def get_unsigned_integer_value(field_name)
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
		# ==== Returns
		#
		# A reference to a MessageFieldIterator.
		#
		def get_field_iterator
		end
end
