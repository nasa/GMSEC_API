#!/usr/bin/env ruby

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# Note: Set RUBYLIB to $GMSEC_HOME/bin
#
# Linux/macOS:
#
#	export RUBYLIB=$GMSEC_HOME/bin
#
# Windows:
#
#	set RUBYLIB=%GMSEC_HOME%\bin
#


require 'libgmsec_ruby'

load 'common.rb'


class Publish < Common
	def initialize(args)
		super("RUBY-PUB", args)
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

			# Create message
			msg = Libgmsec_ruby::MistMessage.new("GMSEC.MY-MISSION.MY-SAT-ID.MSG.HB.RUBY-PUB", "HB", conn_mgr.get_specification)

			pubRate = 1   # in seconds
			msg.set_value("PUB-RATE", pubRate)

			for i in 1..5
				msg.set_value("COUNTER", i)

				# Publish message
				conn_mgr.publish(msg)

				Libgmsec_ruby::Log.info("Published:\n#{msg.to_xml}")

				# Delay before publishing again
				Libgmsec_ruby::TimeUtil::millisleep(pubRate * 1000)    # convert seconds to milliseconds
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
	publish = Publish.new( ARGV )
	publish.run
end
