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


class MyEventCallback < Libgmsec_ruby::ConnectionManagerEventCallback
	def on_event(conn_mgr, status, event)
		if status.is_error()
			Libgmsec_ruby::Log.error("Event Status: #{status.get_reason()}")
		else
			Libgmsec_ruby::Log.info("Event Status: #{status.get_reason()}")
		end
	end
end


class MyCallback < Libgmsec_ruby::ConnectionManagerCallback
	def on_message(conn_mgr, msg)
		Libgmsec_ruby::Log.info("Received Message:\n#{msg.to_xml()}")
	end
end


class Subscribe < Common
	def initialize(args)
		super("RUBY-SUB", args)
	end

	def run_example
		# Display API version information
		Libgmsec_ruby::Log.info("#{Libgmsec_ruby::ConnectionManager.get_api_version}")

		# Declare callbacks
		ecb = MyEventCallback.new
		mcb = MyCallback.new

		begin
			# Instantiate Connection Manager
			conn_mgr = Libgmsec_ruby::ConnectionManager.new( get_config )

			# Register custom Event Callback
			conn_mgr.register_event_callback(Libgmsec_ruby::Connection::ALL_EVENTS, ecb)

			# Connect to the GMSEC Bus
			conn_mgr.connect()

			# Display middleware version information
			Libgmsec_ruby::Log.info("Middleware version = #{conn_mgr.get_library_version}")

			# Subscribe using a custom Callback
			info = conn_mgr.subscribe("GMSEC.>", mcb)

			Libgmsec_ruby::Log.info("Subscribed to: #{info.get_subject}")

			done = false
			while !done
				# Attempt to receive a message within 10 seconds
				rcvd = conn_mgr.receive(10000)

				if rcvd != nil
					# Received a message; dispatch to callback(s)
					conn_mgr.dispatch(rcvd)

					# Free message resource
					conn_mgr.release(rcvd)
				else
					# Timeout (no message received)
					done = true
				end
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
	subscribe = Subscribe.new( ARGV )
	subscribe.run
end
