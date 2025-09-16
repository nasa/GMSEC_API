# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class can be used to start a thread that will continuously publish Heartbeat (HB) messages.
	#
	# Note: Ruby can sometimes be finicky when invoking calls through the C-Ruby interface that start
	# a thread. <b>Use this class at your own risk!</b>
	#
	# ==== Example usage
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        config = Libgmsec_ruby::Config.new( ARGV )
	#        fields = Libgmsec_ruby::FieldArray.new
	#        fields << Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION")
	#        ...
	#
    #        # Create an instance of the HeartbeatGenerator
	#        hbgen = Libgmsec_ruby::HeartbeatGenerator::create( config, 30, fields )
	#
    #        # Start the HeartbeatGenerator
	#        hbgen.start
	#
	#        # ... continue with other stuff as the Heartbeat Generator runs in the background as a separate thread
	#
    #        # Stop the HeartbeatGenerator
	#        hbgen.stop
	#
    #        # Destroy the instance of the HeartbeatGenerator
	#        Libgmsec_ruby::HeartbeatGenerator::destroy( hbgen )
    #
	#    rescue GmsecException => e
	#        puts("Error: #{e.message}")
	#    end
	#
	#
	class HeartbeatGenerator
		##
		# Creates an instance of a Heartbeat Generator object.
		#
		# ==== Attributes
		#
		# * +config+       - The Config object to configure the Connection that is used to publish heartbeat messages.
		# * +hb_pub_rate+  - The publish rate/interval (in seconds) at which to publish heartbeat messages.
		# * +fields+       - Optional list of fields to include within the Heartbeat message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a Connection object cannot be created.
		#
		# ==== Note
		#
		# If the publish rate is set to 0 (zero), the heartbeat generator will only publish one message.
        #
        # ==== See also
        #
        # HeartbeatGenerator.destroy
		#
		def self.create(config, hb_pub_rate, fields = nil)
		end


        ##
        # Destroys the instance of a HeartbeatGenerator object.
        #
        def self.destroy(hbgen)
        end


		##
		# Starts the Heartbeat Generator thread.
		#
		# ==== Returns
		#
		# Returns true if the heartbeat generator has successfully started; false otherwise.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a connection cannot be established with the GMSEC Bus.
		# A GmsecException is thrown if message validation is enabled and a valid Heartbeat message cannot be created.
		#
		# ==== Note
		# If the publish rate is set to 0 (zero), only one heartbeat message will be published,
		# however the heartbeat generator thread will continue to run.
		#
		# ==== See also
		#
		# HeartbeatGenerator.stop
		#
		def start
		end


		##
		# Stops the heartbeat generator thread.
		#
		# ==== Returns
		#
		# Returns true if the heartbeat generator has been successfully stopped false otherwise.
		#
		# ==== See also
		#
		# HeartbeatGenerator.start
		#
		def stop
		end


		##
		# Reports whether the heartbeat generator is running.
		#
		# ==== Returns
		#
		# Returns true if the heartbeat generator is running; false otherwise.
		#
		def is_running
		end


		##
		# Changes the configured publish rate of the heartbeat generator.
		#
		# ==== Attributes
		#
		# * +hb_pub_rate+ - The new publish rate/interval (in seconds).
		#
		# ==== Note
		#
		# If the publish rate is set to 0 (zero), only one additional heartbeat message will be published.
		#
		def change_publish_rate(hb_pub_rate)
		end


		##
		# Adds the given Field to the heartbeat message.
		#
		# ==== Attributes
		#
		# * +field+ - The Field to add to the heartbeat message.
		#
		# ==== Returns
		#
		# Returns true if an existing field was overwritten; false otherwise.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if for whatever reason the field cannot be added to the message.
		#
		# ==== Note
		#
		# If the PUB-RATE field is provided, then the publish rate for the heartbeat generator will
		# be updated to publish at the (new) rate.
		#
		# ==== Note: set_field is an overloaded method; other versions include:
		#
		# * Method that accepts a field name and an integer field value
		#
		#     set_field(field_name, integer_field_value)
		#
		# * Method that accepts a field name and a floating point field value
		#
		#     set_field(field_name, double_field_value)
		#
		# * Method that accepts a field name and a string field value
		#
		#     set_field(field_name, string_field_value)
		#
		def set_field(field)
		end
	end
end
