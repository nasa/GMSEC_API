# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class provides the public interfae for the MessageFactory.
	#
	class MessageFactory

		##
		# Creates a MessageFactory instance that produces messages based on the latest available message 
        # specification for the NASA/GMSEC Addendum (if no configuration provided), or based on the given configuration.
        #
        # ==== Attributes
		#
		# * +config+ - Optional Config object of which will be used to construct a Specification object that the MessageFactory will rely on to generate messages.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the message specification cannot be deduced/loaded.
		#
		# ==== See also
		#
		# MessageFactory.destroy
		#
		def create(config = nil)
		end


		##
		# Destroys the given MessageFactory instance.
        #
        # ==== Attributes
		#
		# * +factory+ - A MessageFactory object.
		#
		# ==== See also
		#
		# MessageFactory.create
		#
        def destroy(factory)
        end


		##
		# Allows for the setting of common (standard) fields that should be applied to all messages created by the MessageFactory.
		#
		# ==== Attributes
		#
		# * +fields+ - The list of common/standard fields to apply to all messages.
		#
		def set_standard_fields(fields)
		end


		##
		# Removes any common (standard) fields that have previously been associated with the MessageFactory.
		#
		def clear_standard_fields()
		end


		##
		# Creates and returns a Message object.
		# The Message will contain any user-supplied message attributes (such as a message configuration, standard fields, and
		# custom message validator).
		#
		# If a schema ID is provided, the Message will be populated with the fields derived from the working message specification.
		#
		# ==== Attributes
		#
		# * +schema_id+ - Optional schema ID to identify which message to create (e.g. HB).
		#
		# ==== Raises
		#
		# A GmsecException if the schemaID contains an empty string, or references an unknown/illegal schema ID.
		#
		def create_message(schema_id = nil)
		end


		##
		# Creates and returns a Message object based on the given XML or JSON data.
		#
		# ==== Attributes
		#
		# * +data+ - An XML or JSON formatted string representation of a message.
		# * +type+ - The type of data being provided.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the given data is nil or cannot be parsed as a message in either XML or JSON format.
		#
		# ==== Usage:
		#
		#    msg1 = msgFactory.from_data( xml_data, Libgmsec_ruby::DataType_XML_DATA )
		#    msg2 = msgFactory.from_data( json_data, Libgmsec_ruby::DataType_JSON_DATA )
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
		def from_data(data, type)
		end


		##
		# Returns a reference to the Specification object associated with the MessageFactory.
		#
		# ==== Returns
		#
		# A Specification object.
		#
		def get_specification()
		end


		##
		# Registers the given custom MessageValidator with each message created using the MessageFactory.
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
end
