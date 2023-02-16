# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class can be used to start a thread that will continuously publish Resource (RSRC) messages.
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
    #        # Create an instance of the ResourceGenerator
	#        rsrcGen = Libgmsec_ruby::ResourceGenerator::create( config, 30, 1, 10, fields )
	#
    #        # Start the ResourceGenerator
	#        rsrcGen.start
	#
	#        # ... continue with other stuff as the Resource Generator runs in the background as a separate thread
	#
    #        # Stop the ResourceGenerator
	#        rsrcGen.stop
	#
    #        # Destroy the instance of the ResourceGenerator
	#        Libgmsec_ruby::ResourceGenerator::destroy( rsrcGen )
	#
	#    rescue GmsecException => e
	#        puts("Error: #{e.message}")
	#    end
	#
	#
	class ResourceGenerator
		##
		# Creates an instance of a Resource Generator object.
		#
		# ==== Attributes
		#
		# * +config+           - The Config object to configure the Connection that is used to publish resource messages.
		# * +pub_rate+         - The publish rate/interval (in seconds) at which to publish resource messages.
		# * +sample_interval+  - The time distance between sample collection (in seconds)
		# * +average_interval+ - The window over which to make a moving average of samples (in seconds)
		# * +fields+           - Optional list of fields to include within the Resource message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a Connection object cannot be created.
		#
		# ==== Note
		#
		# If the publish rate is set to 0 (zero), the resource generator will only publish one message.
		#
		# ==== See also
		#
		# ResourceGenerator.destroy
		#
		def self.create(config, pub_rate, sample_interval, average_interval, fields = nil)
		end


        ##
        # Destroys the instance of a ResourceGenerator object.
        #
		def self.destroy(rsrcgen)
		end


		##
		# Starts the Resource Generator thread.
		#
		# ==== Returns
		#
		# Returns true if the resource generator has successfully started; false otherwise.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a connection cannot be established with the GMSEC Bus.
		# A GmsecException is thrown if message validation is enabled and a valid Resource message cannot be created.
		#
		# ==== Note
		# If the publish rate is set to 0 (zero), only one resource message will be published,
		# however the resource generator thread will continue to run.
		#
		# ==== See also
		#
		# ResourceGenerator.stop
		#
		def start
		end


		##
		# Stops the resource generator thread.
		#
		# ==== Returns
		#
		# Returns true if the resource generator has been successfully stopped false otherwise.
		#
		# ==== See also
		#
		# ResourceGenerator.start
		#
		def stop
		end


		##
		# Reports whether the resource generator is running.
		#
		# ==== Returns
		#
		# Returns true if the resource generator is running; false otherwise.
		#
		def is_running
		end


		##
		# Adds the given Field to the resource message.
		#
		# ==== Attributes
		#
		# * +field+ - The Field to add to the resource message.
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
		# If the PUB-RATE field is provided, then the publish rate for the resource generator will
		# be updated to publish at the (new) rate.
		#
		def set_field(field)
		end


		##
        # Creates/returns a Resource Message using the given MessageFactory.
		#
		# ==== Attributes
        #
		# * +msgFactory+      - The MessageFactory to reference when creating the resource message
		# * +sampleInterval+  - The time distance between sample collection (in seconds)
		# * +averageInterval+ - Time window over which to make a moving average of samples (in seconds)
		#
		# ==== Returns
		#
        # A Message object representing a Resource Message.
		#
		# ==== Raises
		#
        # A GmsecException is thrown if sampleInterval is less than 0 (zero), or if averageInterval is less than sampleInterval
        #
        def self.create_resource_message(msgFactory, sampleInterval, averageInterval)
        end
	end
end
