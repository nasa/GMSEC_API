# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This is a specialized class for storing an array of Field objects.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        standard_fields = Libgmsec_ruby::FieldArray.new
	#
	#        standard_fields << Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION")
	#        standard_fields << Libgmsec_ruby::StringField.new("CONSTELLATION-ID", "MY-CONSTELLATION")
	#        standard_fields << Libgmsec_ruby::StringField.new("SAT-ID-PHYSICAL", "MY-SAT-ID")
	#        ...
	#        # Use for setting standard fields in MistMessage.
	#        Libgmsec_ruby::MistMessage::set_standard_fields( standard_fields )
	#        ...
	#
	#        # Use for other purposes, such as checking if a message has the field.
	#        standard_fields.each do | field |
	#            if not msg.has_field( field.get_field_name )
	#                # ... incomplete message
	#            end
	#        end
	#    end
	#
	class FieldArray
		##
		# Constructor.
		#
		def initialize
		end


		##
		# Pushes the field into the array of fields.
		#
		# ==== Attributes
		#
		# * +field+ - The Field object to insert into the array.
		#
		def <<(field)
		end
	end
end
