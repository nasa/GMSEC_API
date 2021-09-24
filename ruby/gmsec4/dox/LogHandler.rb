# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This class is an abstract base class for processing a log messages via a callback.
	#
	# ==== Example usage
	#    class MyLogHandler < Libgmsec_ruby::LogHandler
	#        def initialize()
	#            super()
	#            @@semaphore = Mutex.new
	#            $stdout.sync = true
	#        end
	#
	#        def on_message(entry)
	#            @@semaphore.lock
	#
	#            file = ""
	#            line = ""
	#            caller_locations(1,1).first.tap{|loc| file = loc.path; line = loc.lineno}
	#
	#            str = Libgmsec_ruby::TimeUtil::format_time(entry.time)
	#            str << " [" << Libgmsec_ruby::Log.to_string(entry.level) << "]"
	#            str << " [#{file}:#{line}]"
	#            str << " " << entry.message
	#
	#            # Prevent log forging
	#            str.gsub!("\n", "\n\t")
	#
	#            puts "#{str}\n\n"
	#
	#            @@semaphore.unlock
	#        end
	#    end
	#
	class LogHandler
	    ##
		# Called by the API in response to a log message.
		#
		# A class derived from LogHandler needs to be registered with Log.register_handler() in order to be called for a particular logging level.
		#
		# ==== Attributes
		#
		# * +entry+ - The received LogEntry
		#
		# ==== Notes
		#
		# The LogEntry structure is defined as containing:
		#
		#    time    : The time the log entry was produced
		#    level   : The severity level of the log level
		#    file    : The file name from where the log message originated from
		#    line    : The line number from within the file
		#    message : The log message
		#
		def on_message(entry)
		end
	end
end
