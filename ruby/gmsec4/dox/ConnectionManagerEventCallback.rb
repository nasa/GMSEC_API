# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This class is an abstract base class for receiving connection related events via a callback.
	#
	# A user created class, derived from this class, can be passed into ConnectionManager.register_event_callback()
	# to have user code executed when a connection related event occurs.
	#
	# Note: Applications <b>must</b> retain a reference to their custom ConnectionManagerEventCallback object. Usage of an
	# anonymous object is not advised, and could lead to a crash of the application.
	#
	# ==== Example Usage
	#
	#    require 'libgmsec_ruby'
	#
	#    class MyConnMgrEventCallback < Libgmsec_ruby::ConnectionManagerEventCallback
	#        def initialize
    #            super
    #        end
	#
	#        def on_event(conn_mgr, status, event)
	#            puts("Event occurred:\n#{status.get}")
	#        end
	#    end
	#
	#    begin
	#        ecb    = MyConnMgrEventCallback.new
	#        config = Libgmsec_ruby::Config.new( ARGV )
	#
	#        begin
	#            conn_mgr = Libgmsec_ruby::ConnectionManager.new(config)
	#            conn_mgr.register_event_callback(Libgmsec_ruby::Connection::ALL_EVENTS, ecb)
	#            conn_mgr.connect()
	#            # ...
	#            conn_mgr.cleanup()
	#        rescue GmsecException => e
	#            puts("Error: #{e.message}")
	#        end
	#    end
	#
	class ConnectionManagerEventCallback
	    ##
		# This method is called in response to certain events or soft-errors detected by the ConnectionManager object.
		#
	    # <b>DO NOT DESTROY or CHANGE STATE</b> of the ConnectionManager object that is passed to
	    # the callback method, nor store it for use beyond the scope of the callback method.
	    #
	    # <b>DO NOT STORE</b> the Status object for use beyond the scope of the callback.
	    # If necessary, make a copy of the Status object.
		#
		# ==== Attributes
		#
	    # * +conn_mgr+ - ConnectionManager on which the event occurred.
	    # * +status+   - A Status object that includes a description of the event.
	    # * +event+    - A Connection::ConnectionEvent enumerated value.
		#
		# ==== See also
		#
		# ConnectionManager.register_event_callback
		#
		def on_event(conn_mgr, status, event)
		end
	end
end
