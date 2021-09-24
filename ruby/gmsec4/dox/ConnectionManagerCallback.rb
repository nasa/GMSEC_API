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
	# A user created class, derived from this class, can be passed into ConnectionManager.subscribe()
	# to have user code executed when a message is received.
	#
	# Note: Applications <b>must</b> retain a reference to their custom ConnectionManagerCallback object. Usage of an
	# anonymous object is not advised, and could lead to a crash of the application.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    class MyConnMgrCallback < Libgmsec_ruby::ConnectionManagerCallback
	#        def initialize
    #            super
    #        end
	#
	#        def on_message(conn_mgr, msg)
	#            puts("Received message:\n#{msg.to_xml}")
	#        end
	#    end
	#
	#    begin
	#        mcb    = MyConnMgrCallback.new
	#        config = Libgmsec_ruby::Config.new( ARGV )
	#        begin
	#            conn_mgr = Libgmsec_ruby::ConnectionManager.new(config)
	#            conn_mgr.connect()
	#            conn_mgr.subscribe("GMSEC.>", mcb)
	#            while true do
	#                msg = conn_mgr.receive(Libgmsec_ruby::GMSEC_WAIT_FOREVER)
	#                conn_mgr.dispatch(msg)
	#                conn_mgr.release(msg)
	#            end
	#        rescue GmsecException => e
	#            puts(e.message)
	#        end
	#    end
	#
	class ConnectionManagerCallback
	    ##
		# The callback method called by the API when a Message is sent through the usage of dispatch().
	    # A class derived from ConnectionManagerCallback needs to be registered with a ConnectionManager,
		# using ConnectionManager.subscribe() in order to be called for a particular subject registration pattern.
		#
	    # <b>DO NOT DESTROY or CHANGE STATE</b> of the ConnectionManager object that is passed to
	    # the callback method, nor store it for use beyond the scope of the callback method.
	    #
	    # <b>DO NOT STORE</b> the Message object for use beyond the scope of the callback.
	    # If necessary, make a copy of the Message object.
		#
		# ==== Attributes
		#
	    # * +conn_mgr+ - ConnectionManager on which the message was received.
	    # * +msg+      - The received message.
		#
		# ==== See also
		#
		# ConnectionManager.subscribe
		#
		def on_message(conn_mgr, msg)
		end
	end
end
