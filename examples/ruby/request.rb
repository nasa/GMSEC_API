#!/usr/bin/env ruby

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# Note: Set RUBYLIB to $GMSEC_HOME/bin
#
# Linux/macOS:
#
#   export RUBYLIB=$GMSEC_HOME/bin
#
# Windows:
#
#   set RUBYLIB=%GMSEC_HOME%\bin
#


require 'libgmsec_ruby'

load 'common.rb'


class Request < Common
	def initialize(args)
		super("RUBY-REQUEST", args)
	end

	def run_example
		# Display API version information
		Libgmsec_ruby::Log.info("#{Libgmsec_ruby::ConnectionManager.get_api_version}")

		# Setup standard fields for all MistMessages
		Libgmsec_ruby::MistMessage.set_standard_fields( get_standard_fields )

		begin
			# Instantiate Connection Manager
			conn_mgr = Libgmsec_ruby::ConnectionManager.new( get_config )

			# Connect to the GMSEC Bus
			conn_mgr.connect()

			# Display middleware version information
			Libgmsec_ruby::Log.info("Middleware version = #{conn_mgr.get_library_version}")

			# Create request message
			msg = Libgmsec_ruby::MistMessage.new("GMSEC.MY-MISSION.MY-SAT-ID.REQ.DIR.RUBY-REPLY", "REQ.DIR", conn_mgr.get_specification)

			# Indicate a response should be sent
			msg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", "RUBY-REPLY") )
			msg.add_field( Libgmsec_ruby::BooleanField.new("RESPONSE", true) )

			# Display request message
			Libgmsec_ruby::Log.info("Issuing Request:\n#{msg.to_xml()}")

			# Send request message; await for synchronous response
			response = conn_mgr.request(msg, 5000, Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)

			if response != nil
				Libgmsec_ruby::Log.info("Received Response:\n#{response.to_xml()}")

				conn_mgr.release(response)
			else
				Libgmsec_ruby::Log.info("Failed to receive a response")
			end

			# Clean up
			conn_mgr.cleanup()

		# Exception handling
		rescue GmsecException, Exception => e
			Libgmsec_ruby::Log.error("Exception: #{e.message}")
		ensure
			# Free resources for MistMessage standard fields
			Libgmsec_ruby::MistMessage.clear_standard_fields()
		end
	end
end


# Launch example program
begin
	request = Request.new( ARGV )
	request.run
end

