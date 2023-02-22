#!/usr/bin/env ruby

require 'libgmsec_ruby'
require 'socket'


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


class String
	def hex_inspect
		'"' + each_byte.map { |b| '\x%02X' % b }.join + '"'
	end
end


class TestException < Exception
	def initialize(msg)
		super(msg)
	end
end


class Test
	@@nOk = 0
	@@nBad = 0
	@@prefix = ""
	@@reason = ""
	@@exitReason = ""
	@@description = ""
	@@signal = ""
	@@config = nil
	@@standardFields = nil

    # Declare the (standard) fields as static so they are preserved for the lifetime of Test
	@@mission  = Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION", true)
	@@cnst     = Libgmsec_ruby::StringField.new("CONSTELLATION-ID", "MY-CONSTELLATION-ID", true)
	@@satPhys  = Libgmsec_ruby::StringField.new("SAT-ID-PHYSICAL", "MY-SAT-ID", true)
	@@satLog   = Libgmsec_ruby::StringField.new("SAT-ID-LOGICAL", "MY-SAT-ID", true)
	@@comp     = Libgmsec_ruby::StringField.new("COMPONENT", "MY-COMPONENT", true)
	@@facility = Libgmsec_ruby::StringField.new("FACILITY", "HOME", true)
	@@domain1  = Libgmsec_ruby::StringField.new("DOMAIN1", "MY-DOMAIN-1", true)
	@@domain2  = Libgmsec_ruby::StringField.new("DOMAIN2", "MY-DOMAIN-2", true)

	@@logHandler = CustomLogHandler.new


	def initialize(args)
		for arg in args
			if arg[0] == '-' and arg.include? "-signal="
				@@signal = arg[8..-1]
				break
			end
		end

		set_prefix($0)

# TODO: Keep snip of code below; custom log handler may not be usable with Windoze.
=begin
=end
		if OS.unix?
			Libgmsec_ruby::Log.register_handler( @@logHandler )
		end

		@@config = Libgmsec_ruby::Config.new( args )

		@@config.add_value("loglevel", @@config.get_value("loglevel", "info"))
		@@config.add_value("logfile",  @@config.get_value("logfile", "stderr"))

		set_description( self.class.name )

		init_standard_fields()
	end


	def init_standard_fields()
		@@standardFields = Libgmsec_ruby::FieldArray.new()
		@@standardFields << @@mission
		@@standardFields << @@cnst
		@@standardFields << @@satPhys
		@@standardFields << @@satLog
		@@standardFields << @@comp
		@@standardFields << @@facility
		@@standardFields << @@domain1
		@@standardFields << @@domain2
	end


	def get_config()
		return @@config
	end


	def get_standard_fields()
		return @@standardFields
	end


	def set_standard_fields(factory)
		factory.set_standard_fields(@@standardFields)
	end


	def verify_standard_fields(msg)
		for field in @@standardFields
			check("Message is missing standard field", msg.has_field(field.get_field_name()))
			check("Message unexpected standard field type", msg.get_field(field.get_field_name()).get_field_type() == field.get_field_type())
			check("Message unexpected standard field value", msg.get_field(field.get_field_name()).get_string_value() == field.get_string_value())
			check("Message not a header field", msg.get_field(field.get_field_name()).is_header() == true)
		end
	end


	def check(what, flag=nil, status=nil)
		if status == nil
			return _check(what, flag)
		else
			return _check(what + ": #{status.get()}", status.is_error() == false)
		end
	end


	def require(what, flag=nil, status=nil)
		if status == nil
			if _check(what, flag) == false
				raise TestException.new(what)
			end
		else
			if _check(what, (status.is_error() == false)) == false
				raise TestException.new(what + ": #{status.get()}")
			end
		end
	end


	def get_mw()
		return get_config().get_value("mw-id")
	end


	def get_subject(tag=nil)
		if tag == nil
			subject = @@prefix
		else
			subject = @@prefix + "." + tag
		end
		return subject.upcase
	end


    def get_unique_component()
        component = @@prefix
        component.gsub(".", "-")
        return component
    end


	def get_data_file(file)
		path = nil
		addonDir = @@config.get_value("addons")
		if addonDir == nil
			path = @@directory + "/addons/" + file
		else
			path = addonDir + "/" + file
		end
		return path
	end


	def excuse(reason)
		@@reason = reason
	end


	def set_description(desc)
		@@description = desc.gsub("Test_", "")
	end


	def get_description()
		return @@description
	end


	def run()
		begin
            start()
			Libgmsec_ruby::Log.info("Running #{get_description()}...")
			test()
		rescue TestException, Exception => ex
			check("Exception: #{ex.message}", false)
		ensure
			if @@exitReason != ""
				puts "Forcing exit due to: #{@@exitReason}"
			end

			finish()

# TODO: It is possible that custom LogHandler cannot be used with Ruby.
=begin
=end
			if OS.unix?
				Libgmsec_ruby::Log.register_handler( nil )
			end
		end
	end


	private


	def _check(what, flag)
		if flag
			@@nOk += 1
		else
			@@nBad += 1
			Libgmsec_ruby::Log.warning(what)
		end
		return flag
	end


	def get_test_number(prog_name)
		test_number = ""

		"#{prog_name}".split('').each { |c|
			if c =~ /[0-9]/
				test_number << "#{c}"
			end
		}
		return test_number
	end


	def set_prefix(prog_name)

		test_number = get_test_number(prog_name)

		# Set up prefix
		@@prefix  = "NIGHTRUN.RUBY."
        @@prefix += "#{Process.pid}_"
        @@prefix += Socket.gethostname
		@@prefix += ".T#{test_number}"

        @@prefix = @@prefix.upcase

        # Also capture our current working directory
        @@directory = File.dirname(prog_name)
	end


	def force_exit_at_cleanup(reason)
		@@exitReason = reason
	end


	def start()
		summary = "DESCRIPTION=#{@@description}"

		if @@signal != ""
			begin
				File.open(@@signal, 'w') { |f| f << summary; f.close }
			rescue IOError => e
				puts e.message
			end
		end
	end


	def finish()
		result = "FAIL"
		if @@nBad == 0 && @@nOk > 0
			result = "PASS"
		end
		if @@reason != ""
			result = "EXCUSE"
		end

		summary = "RESULT=" + result + "\nDESCRIPTION=" + @@description + "\nN_OK=#{@@nOk}" + "\nN_BAD=#{@@nBad}" + "\nPREFIX=#{@@prefix}"

		if @@reason != ""
			summary += "\n\tEXCUSE=#{@@reason}"
		end

		Libgmsec_ruby::Log.info("Summary:\n" + summary)

		if @@signal != ""
			begin
				File.open(@@signal, "w") { |f| f << summary; f.close }
			rescue IOError => e
				puts e.message
			end
		end
	end
end


module OS
	def OS.windows?
		(/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil
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
