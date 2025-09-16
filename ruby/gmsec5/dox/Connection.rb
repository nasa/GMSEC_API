# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class is the public interface for middleware connections. It abstracts away
	# details of specific middleware into an interface that applications can use to send
	# and receive messages, and perform other services. Applications will not need to access
	# the middleware connection directly.
	#
	# The Connection class provides services available on an implemented GMSEC connection. An
	# application can create multiple Connection object with different parameters and different
	# middleware.
	#
	# Creating multiple connections to the same middleware is not prevented, however this feature
	# may not be supported by all middleware implementations.
	#
	# ==== Example creation and use:
	#
	#    require 'libgmsec_ruby'
	#
	#    begin
	#        config = Libgmsec_ruby::Config.new( ARGV )  # Config populated using command line arguments containing name=value pairs
	#        conn   = nil
	#
	#        begin
	#            conn = Libgmsec_ruby::Connection::create( config )
	#
	#            conn.connect
    #
	#            # ...
	#
	#        rescue GmsecException => e
	#            puts("Error: #{e.message}")
    #        ensure
    #            if conn != nil
    #                conn.disconnect()
	#                Libgmsec_ruby::Connection::destroy( conn )
    #                conn = nil
    #            end
	#        end
	#    end
	#
	class Connection

		##
		# Possible soft errors/events that can occur with the Connection object.
		#
		class Event
			##
			# ==== Enumerated values
			#
			# * +DISPATCHER_ERROR_EVENT+        - Error occurred while attempting to dispatch message.
			# * +REQUEST_TIMEOUT_EVENT+         - A timeout occurred while attempting to receive a reply for a pending request.
			# * +CONNECTION_SUCCESSFUL_EVENT+   - Successfully connected to the middleware server.
			# * +CONNECTION_BROKEN_EVENT+       - Connection to middleware server has been broken
			# * +CONNECTION_RECONNECT_EVENT+    - An attempt is being made to reconnect to the middleware server.
			# * +CONNECTION_EXCEPTION_EVENT+    - An error, possibly fatal, has occurred with the connection to the middleware.
			# * +WSMQ_ASYNC_STATUS_CHECK_EVENT+ - WebSphere MQ Asynchronous Put status check.
			# * +MSG_PUBLISH_FAILURE_EVENT+     - Failure occured while attempting to publish aggregated message.
			# * +INVALID_MESSAGE_EVENT+         - Message failed validation
			# * +ALL_EVENTS+                    - Monitor all events.
			#
			def Event_Enum
			end
		end


		##
		# Returns the version of the GMSEC API.
		#
		# ==== Returns
		#
		# A string containing the GMSEC API version information.
		#
		def self.get_api_version
		end


		##
		# Creates a Connection instance using the given Config. If a MessageFactory is provided, then
        # it will be associated with the Connection instance.
        #
        # When an application requires more than one Connection instance it will be advantageous to
        # rely on a single MessageFactory object. This will assist in reducing the memory footprint
        # of the application. Do NOT destroy the MessageFactory object while the Connection object(s)
        # are in use!
		#
		# ==== Attributes
		#
		# * +config+  - A Config object containing middleware ID and connection information.
		# * +factory+ - Optional MessageFactory object to apply to the Connection object.
		#
		# ==== Returns
		#
		# A Connection object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the configuration information in the Config object cannot be used to deduce the middleware type.
        #
        # ==== See also
        #
        # Connection.destroy
		#
		def self.create(config, factory = nil)
		end


        ##
        # Destroys the Connection instance.
        #
        def self.destroy(conn)
        end


		##
		# Establishes a connection to the middleware (i.e. connects to the GMSEC Bus).
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a connection cannot be established or other anomaly occurs.
		#
		def connect
		end


		##
		# Terminates the connection to the middleware (i.e. disconnects from the GMSEC Bus).
		#
		# Note: This method is automatically called if the Connection object is destroyed.
		#
		def disconnect
		end


		##
		# Identifies the library name, ergo the connection type with which this Connection is associated.
		#
		# ==== Returns
		#
		# A string with the library name.
		#
		def get_library_name
		end


		##
		# Identifies the version information for this Connection's associated middleware.
		#
		# ==== Returns
		#
		# A string with the middleware version information.
		#
		def get_library_version
		end


		##
		# Returns the Config object associated with the Connection.
		#
		# Note: The returned Config object is for reference purposes only. Changing the config object will not affect the Connection
		#
		# ==== Returns
		#
		# A handle to a Config object.
		#
		def get_config
		end


		##
		# Returns the MessageFactory object associated with the Connection which can be used to construct messages.
		#
		# ==== Returns
		#
		# A handle to a MessageFactory object.
		#
		def get_message_factory
		end


		##
		# Allows for the registration of a custom EventCallback that is summoned when a particular event occurs.
		#
		# ==== Attributes
		#
		# * +event+          - An Event enumeration value.
		# * +event_callback+ - The custom EventCallback to register for the given event.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the custom EventCallback is nil.
		#
		def register_event_callback(event, event_callback)
		end


		##
		# Subscribes to a particular subject or pattern. If a custom Callback object is provided, messages that
		# are received by the Connection object can be distributed to it by using dispatch().
		#
		# Multiple subscriptions can be set up, however applications will not be permitted to set up redundant
		# subscriptions using the same topic and/or custom Callback.
		#
		# ==== Attributes
		#
		# * +topic+    - The topic to which to subscribe.
		# * +callback+ - Optional custom Callback object.
		# * +config+   - Optional Config object for configuring middleware.
		#
		# ==== Returns
		#
		# A SubscriptionInfo object is returned.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if the topic is nil or does not represent a well-formed subscription topic.
		#
		# A GmsecException is thrown if a redundant subscription is set up.
		#
		# A GmsecException is thrown if an error occurs at the middleware level.
		#
		# ==== See also
		#
		# Connection.unsubscribe
		#
		# Connection.receive
		#
		# Connection.dispatch
		#
		def subscribe(topic, callback = nil, config = nil)
		end


		##
		# Unsubscribes from a particular subject pattern as identified within the given SubscriptionInfo object.
		#
		# This will stop the reception of messages that match the subject pattern. It will also remove the
		# registration of any callbacks associated with this subscription.
		#
		# ==== Attributes
		#
		# * +info+ - A SubscriptionInfo object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if the given SubscriptionInfo object originated from a different Connection object.
		#
		# A GmsecException is thrown if an error occurs at the middleware level.
		# 
		# ==== See also
		#
		# Connection.subscribe
		#
		def unsubscribe(info)
		end


		##
		# Publishes the given message to the middleware.
		#
		# A Config object can be provided to enable or disable certain middleware-level publish functionalities
		# (e.g. ActiveMQ Durable Producer).
		#
		# Note: The actual message sent to the middleware will contain tracking fields; to disable this feature,
		# create a Connection or Message object with the <i>tracking=off</i> configuration option.
		#
		# ==== Attributes
		#
		# * +msg+    - The Message to publish.
		# * +config+ - Option Config object for configuring middleware-level options.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if an error occurs while attempting to publish the message.
		# 
		# ==== See also
		#
		# Connection.subscribe
		#
		# Connection.receive
		#
		def publish(msg, config = nil)
		end


		##
		# Sends a request message, and waits for the specified timeout to receive a response message.
		#
		# A call to this method will block until a response message is received or the timeout period has lapsed.
		# The timeout value is expressed in milliseconds.
		#
		# Note: The actual message sent to the middleware will contain tracking fields; to disable this feature,
		# create a Connection or Message object with the <i>tracking=off</i> configuration option.
		#
		# ==== Attributes
		#
		# * +request_msg+  - The request Message to send.
		# * +timeout+      - The maximum time to wait for a response (in milliseconds)
		# * +republish_ms+ - Optional request message resubmission interval (in milliseconds). If set to a negative
		# value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
		# the period will default to 60000ms, unless the user has provided an alternate time period via the
		# Config object used to create the Connection object. The minimum republish period allowed is 100ms.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if an error occurs while attempting to send the message.
		# 
		# ==== See also
		#
		# Message.destroy
		#
		def request(request_msg, timeout, republish_ms = Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)
		end


		##
		# Sends a reply message.
		#
		# Note: The actual message sent to the middleware will contain tracking fields; to disable this feature,
		# create a Connection or Message object with the <i>tracking=off</i> configuration option.
		#
		# ==== Attributes
		#
		# * +request_msg+  - The request message that was received, and to which a response is being sent.
		# * +response_msg+ - The reply Message which to send.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if an error occurs while attempting to send the message.
		#
		# ==== See also
		#
		# Connection.receive
		# Connection.request
		#
		def reply(request_msg, response_msg)
		end


		##
		# Summons any custom Callback objects that are subscribed with subject patterns that match
		# the subject of the given Message.
		#
		# ==== Attributes
		#
		# * +msg+ - The Message to dispatch to Callback objects.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if the message object is nil.
		#
		def dispatch(msg)
		end


		##
		# Returns the next message received from the GMSEC Bus within the specified timeout.
		#
		# Received messages are determined by the message subscriptions set up with subscribe().
		#
		# ==== Attributes
		#
		# * +timeout+ - The maximum time (in milliseconds) to block waiting for a message; if -1
		# (or Libgmsec_ruby::GMSEC_WAIT_FOREVER) is specified, then the call will block indefinitely until a message is received.
		#
		# ==== Returns
		#
		# A Message object is returned, or nil if a timeout occurs.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if an error occurs while attempting to receive a message.
		#
		# ==== See also
		#
		# Connection.subscribe
		#
		# Connection.dispatch
		#
		# Message.destroy
		#
		def receive(timeout = Libgmsec_ruby::GMSEC_WAIT_FOREVER)
		end


		##
		# Exclude any incoming messages with the specified subject topic pattern.
		#
		# ==== Attributes
		#
		# * +subject_pattern+ - The subject pattern to use when filtering incoming messages.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if the subject pattern is nil or contains a malformed subject topic pattern.
		#
		def exclude_subject(subject_pattern)
		end


		##
		# Remove an excluded subject, allowing incoming messages with the matching subject to once again be received.
		#
		# ==== Attributes
		#
		# * +subject_pattern+ - The subject pattern to remove from filtering incoming messages.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if there is no connection to the GMSEC Bus.
		#
		# A GmsecException is thrown if the subject pattern is nil or contains a malformed subject topic pattern.
		#
		def remove_excluded_subject(subject_pattern)
		end


		##
		# Returns the name of the connection, which is automatically generated or user specified.
		#
		# ==== Returns
		# A string representing the connection name.
		#
		# ==== See also
		#
		# Connection.set_name
		#
		def get_name
		end


		##
		# Sets the name of this connection. This can be used for identifying connections within a client program.
		#
		# Usage of this method is optional; if not called, a connection name will be automatically generated.
		#
		# ==== Attributes
		#
		# * +name+ - The name to assign to the Connection object.
		#
		# ==== See also
		#
		# Connection.get_name
		#
		def set_name(name)
		end


		##
		# Returns the ID associated with the Connection.
		#
		# ==== Returns
		#
		# An ID string.
		#
		def get_id
		end


		##
		# Returns the middleware information.
		#
		# ==== Returns
		#
		# A string containing middleware information.
		#
		def get_mw_info
		end


		##
		# Returns the middleware broker connection information. This information can be useful for knowing where
		# a connection to the GMSEC Bus has been made.
		#
		# ==== Returns
		#
		# A string with the middleware broker connection information.
		#
		def get_connection_endpoint
		end
	end
end
