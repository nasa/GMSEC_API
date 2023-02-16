#!/usr/bin/env ruby

require 'libgmsec_ruby'


class CustomLogHandler < Libgmsec_ruby::LogHandler
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

		sep = file.rindex(File::SEPARATOR)
		if sep != nil
			file = file[(sep+1)..-1]
		end

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
