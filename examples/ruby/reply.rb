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


class Reply < Common
	def initialize(args)
		super("RUBY-REPLY", args)
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

			# Subscribe
			info = conn_mgr.subscribe("GMSEC.*.*.REQ.DIR.>")

			Libgmsec_ruby::Log.info("Subscribed to: #{info.get_subject}")

			# Call receive() to synchronously retrieve a message that has
			# been received by the middleware client library
			# Timeout periods:
			# GMSEC_WAIT_FOREVER (or -1) - Wait forever
			#  0 - Return immediately
			# >0 - Time in milliseconds before timing out
			rcvd_msg = conn_mgr.receive(Libgmsec_ruby::GMSEC_WAIT_FOREVER)

			if rcvd_msg != nil
				# Received a message; dispatch to callback(s)
				Libgmsec_ruby::Log.info("Received:\n" + rcvd_msg.to_xml)

				# Check that we received a request message
				if rcvd_msg.get_kind == Libgmsec_ruby::Message::REQUEST

					# Check if a response should be prepared
					if (rcvd_msg.has_field("RESPONSE") && (rcvd_msg.get_integer_value("RESPONSE") == 1)) || (!rcvd_msg.has_field("RESPONSE"))

						# Attempt to get the name of the component issuing the request
						component = "UNKNOWN"
						if rcvd_msg.has_field("COMPONENT")
							component = rcvd_msg.get_string_value("COMPONENT")
						else
							Libgmsec_ruby::Log.warning("Request message does not have COMPONENT field")
						end

						# Set Status Code to indicate Successful Completion.
						# The GMSEC Interface Specification Document defines 6
						# unique STATUS-CODE values:
						# 1 - Acknowledgement
						# 2 - Working/keep alive
						# 3 - Successful completion
						# 4 - Failed completion
						# 5 - Invalid request
						# 6 - Final message
						# If an asynchronous requestor is awaiting a reply, the
						# ReplyCallback in use will remain active for multiple
						# messages, so long as the messages it receives contain
						# a STATUS-CODE of either 1 or 2.
						status_code = "3"

						# Set the reply subject.
						# See API Interface Specificaiton Document for
						# more information on Reply Message subjects.
						# Generally speaking, they are composed
						# accordingly:
						# [Spec].[Mission].[Satellite ID].
						# [Message Type].[Component Name].[Status Code]

						reply_subject = rcvd_msg.get_subject
						reply_subject.gsub!(".REQ.", ".RESP.")
						reply_subject.gsub!("RUBY-REPLY", component)
						reply_subject << "."
						reply_subject << status_code

						# Create reply message
						reply_msg = Libgmsec_ruby::MistMessage.new(reply_subject, "RESP.DIR", conn_mgr.get_specification)

						# Add field(s) to the reply message
						reply_msg.add_field( Libgmsec_ruby::StringField.new("COMPONENT", "RUBY-REPLY") )
						reply_msg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", component) )

						# Display XML representation of the reply message
						Libgmsec_ruby::Log::info("Prepared Reply:\n" + reply_msg.to_xml)

						# Send Reply
						conn_mgr.reply(rcvd_msg, reply_msg)
					else
						Libgmsec_ruby::Log.info("A response message was not requested")
					end
				end

				# Free message resource
				conn_mgr.release(rcvd_msg)
			else
				Libgmsec_ruby::Log.info("Timeout; no request message received")
			end

			# Clean up
			conn_mgr.cleanup()

		# Exception handling
		rescue GmsecException, Exception => e
			Libgmsec_ruby::Log.error("Exception: #{e.message}")
		end
	end
end


# Launch example program
begin
	reply = Reply.new( ARGV )
	reply.run
end
