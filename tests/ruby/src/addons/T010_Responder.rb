#!/usr/bin/env ruby

require 'libgmsec_ruby'


class T010_Responder
	def initialize(args)
		@@config = Libgmsec_ruby::Config.new(args)
	end


	def get_config
		return @@config
	end


	def get_standard_fields
		mission  = Libgmsec_ruby::StringField.new("MISSION-ID", "MY-MISSION", true)
		cnst     = Libgmsec_ruby::StringField.new("CONSTELLATION-ID", "MY-CONSTELLATION-ID", true)
		satPhys  = Libgmsec_ruby::StringField.new("SAT-ID-PHYSICAL", "MY-SAT-ID", true)
		satLog   = Libgmsec_ruby::StringField.new("SAT-ID-LOGICAL", "MY-SAT-ID", true)
		comp     = Libgmsec_ruby::StringField.new("COMPONENT", "MY-COMPONENT", true)
		facility = Libgmsec_ruby::StringField.new("FACILITY", "MY-FACILITY", true)
		domain1  = Libgmsec_ruby::StringField.new("DOMAIN1", "MY-DOMAIN-1", true)
		domain2  = Libgmsec_ruby::StringField.new("DOMAIN2", "MY-DOMAIN-2", true)

		fields = Libgmsec_ruby::FieldArray.new()
		fields << mission
		fields << cnst
		fields << satPhys
		fields << satLog
		fields << comp
		fields << facility
		fields << domain1
		fields << domain2

		return fields
	end


	def respond_to_request
		request_subject = get_config().get_value("req-subject", "GMSEC.UNKNOWN")
		reply_subject   = get_config().get_value("rep-subject", "GMSEC.UNKNOWN")

		conn = nil

		begin
			Libgmsec_ruby::Log::info("Responder is creating Connection...")
			conn = Libgmsec_ruby::Connection::create( get_config() )

			Libgmsec_ruby::Log::info("Responder is connecting...")
			conn.connect()

			conn.get_message_factory().set_standard_fields( get_standard_fields() )

			Libgmsec_ruby::Log::info("Responder is subscribing to: #{request_subject}")
			conn.subscribe( request_subject )

			iter = 0

			while iter < 3
				Libgmsec_ruby::Log::info("Responder is waiting to receive message...")
				reqMsg = conn.receive(15000)

				if reqMsg == nil
					Libgmsec_ruby::Log::warning("Responder failed to receive message")
				else
					Libgmsec_ruby::Log::info("Responder got request message")

					if iter == 0 or iter == 2
						Libgmsec_ruby::Log::info("Responder is preparing response message...")

						# prepare response message
						repMsg = conn.get_message_factory().create_message("RESP.DIR")
                        repMsg.set_subject( reply_subject )

						if reqMsg.has_field("REQUEST-ID")
							repMsg.add_field( reqMsg.get_field("REQUEST-ID") )
						end

						if reqMsg.has_field("COMPONENT")
							repMsg.add_field( Libgmsec_ruby::StringField.new("DESTINATION-COMPONENT", reqMsg.get_string_value("COMPONENT")) )
						end

						repMsg.add_field( Libgmsec_ruby::I16Field.new("RESPONSE-STATUS", 1) )

						# send response message
						Libgmsec_ruby::Log::info("Responder is sending response message...")
						conn.reply(reqMsg, repMsg)
					else
						Libgmsec_ruby::Log::info("Responder is ignoring the request message")
					end
				end

				iter += 1
			end
		rescue GmsecException => e
			Libgmsec_ruby::Log::error("Exception in respond_to_request thread: #{e.message}")
		ensure
			if conn != nil
				Libgmsec_ruby::Log::info("Responder is cleaning up...")
				conn.disconnect()
				Libgmsec_ruby::Connection::destroy( conn )
				conn = nil
			end
		end
	end
end


begin
	responder = T010_Responder.new( ARGV )
	responder.respond_to_request()
end
