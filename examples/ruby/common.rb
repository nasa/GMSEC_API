#!/usr/bin/env ruby

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


require 'libgmsec_ruby'


class MyLogHandler < Libgmsec_ruby::LogHandler
	def initialize()
		super()
		@@semaphore = Mutex.new
		$stdout.sync = true
	end

	def on_message(entry)
		@@semaphore.lock

		file = ""
		line = ""
		caller_locations(1,1).first.tap{|loc| file = loc.path; line = loc.lineno}

		str = Libgmsec_ruby::TimeUtil::format_time(entry.time)
		str << " [" << Libgmsec_ruby::Log.to_string(entry.level) << "]"
		str << " [#{file}:#{line}]"
		str << " " << entry.message

		# Prevent log forging
		str.gsub!("\n", "\n\t")

		puts "#{str}\n\n"

		@@semaphore.unlock
	end
end


class Common
	@@client_name = "UNKNOWN"
	@@config = nil
	@@log_handler = nil

	def initialize(name, args)
		# Accept given name (under certain conditions)
		if name != nil && name != ""
			@@client_name = name.upcase
		end

		# Instantiate configuration for middleware connections
		@@config = Libgmsec_ruby::Config.new(args)

		# Default to showing logs at INFO level, unless otherwise configured for other level.
		@@config.add_value("loglevel", @@config.get_value("loglevel", "info"))

		# Default to showing logs to stdout, unless otherwise configured.
		@@config.add_value("logfile", @@config.get_value("logfile", "stdout"))

		# Instantiate custom log handler
		@@log_handler = MyLogHandler.new

		# Register log handler (note, custom logging may not work on Windows; try at your own risk)
		if OS.unix?
			Libgmsec_ruby::Log::register_handler( @@log_handler )
		end
	end

	def get_config
		return @@config
	end

	def run
		run_example

		# Un-register log handler; this mitigates the chance that last moment log output is
		# directed to our custom log handler, which by now, may have been garbage collected.
		if OS.unix?
			Libgmsec_ruby::Log::register_handler( nil )
		end
	end

	def get_standard_fields(spec_version = Libgmsec_ruby::GMSEC_ISD_CURRENT)
		mission  = Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION")
		cnst     = Libgmsec_ruby::StringField.new("CONSTELLATION-ID", "MY-CONSTELLATION-ID")
		satPhys  = Libgmsec_ruby::StringField.new("SAT-ID-PHYSICAL", "MY-SAT-ID")
		satLog   = Libgmsec_ruby::StringField.new("SAT-ID-LOGICAL", "MY-SAT-ID")
		comp     = Libgmsec_ruby::StringField.new("COMPONENT", @@client_name)
		facility = Libgmsec_ruby::StringField.new("FACILITY", "HOME")

		fields = Libgmsec_ruby::FieldArray.new()
		fields << mission
		fields << cnst
		fields << satPhys
		fields << satLog
		fields << comp
		fields << facility

		if spec_version > Libgmsec_ruby::GMSEC_ISD_2016_00
			domain1 = Libgmsec_ruby::StringField.new("DOMAIN1", "MY-DOMAIN-1")
			domain2 = Libgmsec_ruby::StringField.new("DOMAIN2", "MY-DOMAIN-2")

			fields << domain1
			fields << domain2
		end

		fields.each { |field| field.set_header(true) }

		return fields
	end
end


module OS
	def OS.windows?
		(/mswin/ =~ RUBY_PLATFORM) != nil
	end

	def OS.mac?
		(/darwin/ =~ RUBY_PLATFORM) != nil
	end

	def OS.unix?
		!OS.windows?
	end

	def OS.linux?
		OS.unix? and not OS.mac?
	end

	def OS.jruby?
		RUBY_ENGINE == 'jruby'
	end
end
