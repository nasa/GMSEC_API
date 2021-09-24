# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This class is an abstract base class for processing a message via a callback.
	#
	# A user created class, derived from this class, can be passed into Connection.subscribe()
	# to have user code executed when a message is received.
	#
	# Note: Applications <b>must</b> retain a reference to their custom Callback object. Usage of an
	# anonymous object is not advised, and could lead to a crash of the application.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    class MyCallback < Libgmsec_ruby::Callback
	#        def initialize
    #            super
    #        end
	#
	#        def on_message(conn, msg)
	#            puts("Received message:\n#{msg.to_xml}")
	#        end
	#    end
	#
	#    begin
	#        mcb    = MyCallback.new
	#        config = Libgmsec_ruby::Config.new( ARGV )
	#        conn   = nil
	#        begin
	#            conn = Libgmsec_ruby::Connection::create(config)
	#            conn.connect()
	#            conn.subscribe("GMSEC.>", mcb)
	#            while true do
	#                msg = conn.receive(Libgmsec_ruby::GMSEC_WAIT_FOREVER)
	#                conn.dispatch(msg)
	#                conn.release(msg)
	#            end
	#        rescue GmsecException => e
	#            puts(e.message)
	#        ensure
	#            if conn != nil
	#                Libgmsec_ruby::Connection::destroy(conn)
	#                conn = nil
	#            end
	#        end
	#    end
	#
	class Callback
	    ##
		# The callback method called by the API when a Message is sent through the usage of dispatch().
	    # A class derived from Callback needs to be registered with a Connection, using subscribe()
		# in order to be called for a particular subject registration pattern.
		#
	    # <b>DO NOT DESTROY or CHANGE STATE</b> of the Connection object that is passed to
	    # the callback method, nor store it for use beyond the scope of the callback method.
	    #
	    # <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback.
	    # If necessary, make a copy of the Message object.
		#
		# ==== Attributes
		#
	    # * +conn+ - Connection on which the message was received.
	    # * +msg+  - The received message.
		#
		# ==== See also
		#
		# Connection.subscribe
		#
		def on_message(conn, msg)
		end
	end
end
