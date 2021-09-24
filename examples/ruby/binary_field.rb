#!/usr/bin/env ruby

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# Note: Set RUBYLIB to $GMSEC_HOME/bin


require 'libgmsec_ruby'

load 'common.rb'


class BinaryFieldExample < Common
	def initialize(args)
		super("BinaryFieldExample", args)
	end

	def run_example
		# Define data
		data = []
		for i in 0..128 do
			data << (i % 256)
		end

		field_data = data.pack('C*').force_encoding('BINARY')

		# Create message
		msg = Libgmsec_ruby::Message.new("GMSEC.FOO.BAR", Libgmsec_ruby::Message::PUBLISH)

		# Create BinaryField
		bin_field = Libgmsec_ruby::BinaryField.new("MY-DATA-1", field_data, field_data.length)

		# Add BinaryField to the message
		msg.add_field(bin_field)

		# Display the message
		Libgmsec_ruby::Log::info("Message:\n#{msg.to_xml()}")

		# Inspect data for first BinaryField
		bin = msg.get_binary_field("MY-DATA-1")
		Libgmsec_ruby::Log::info("Field Name: #{bin.get_field_name()}")
		Libgmsec_ruby::Log::info("Field Type: #{bin.get_field_type()}")
		Libgmsec_ruby::Log::info("Bin: #{bin.to_xml()}")

		# Pull arbitrary byte from second BinaryField
		Libgmsec_ruby::Log::info("Bin Data Pos 3: #{bin.get_value()[3].ord()}")

		for i in 0..(bin.get_length() - 1)
			Libgmsec_ruby::Log::info("Data at [#{i}]: #{bin.get_value()[i].ord()}")
		end
	end
end


# Launch example program
begin
    binary = BinaryFieldExample.new( ARGV )
    binary.run
end

