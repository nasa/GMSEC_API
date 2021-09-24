# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# The ConnectionManager is designed to provide several high-level management
	# functions for operation of a GMSEC connection object. These management functions 
	# include the application of message content validation and a set of functions which
	# streamline both log and heartbeat  message production.
	#
	# The ConnectionManager wraps the GMSEC Connection object, supplying both normal
	# middleware interaction functions and some additional quality of service. 
	# 
	# If the object is configured to validate messages, all messages sent from this
	# ConnectionManager to the GMSEC Bus will be compared to an available schema definition.
	# If a template definition for the message in question is available, it will be validated.
	# The message will <b>not</b> be sent over the middleware if validation fails. Messages
	# without a template definition will be sent without issue.
	#
	class ConnectionManager
		##
		# Constructor
		#
		# ==== Attributes
		#
		# * +config+   - The Config object used to construct the underlying Connection object.
		# * +validate+ - Boolean flag to indicate whether to validate messages; default is false.
		# * +version+  - The version of the message specification to use; default is GMSEC_ISD_CURRENT.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the configuration information cannot be used to deduce a GMSEC connection type.
		#
		# A GmsecException is thrown if an anomaly occurs while loading message schemas for the specified message specification.
		#
		def initialize(config, validate = false, version = Libgmsec_ruby::GMSEC_ISD_CURRENT)
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
		# Establishes a connection with the GMSEC middleware broker.
		#
		# Once this call successfully returns, the ConnectionManager is ready for message operations.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if an anomaly occurs while attempting to connect to the middleware broker.
		#
		# ==== See also
		#
		# ConnectionManager.cleanup
		#
		def connect
		end


		##
		# Disconnects and destroys the underlying Connection object managed by the ConnectionManager.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if an anomaly occurs while disconnecting from the middleware broker.
		#
		# ==== See also
		#
		# ConnectionManager.connect
		#
		def cleanup
		end


		##
		# Returns the current state of the connection to the middleware.
		#
		# ==== Returns
		#
		# An enumerated ConnectionState value.
		#
		def get_state
		end


		##
		# Identifies the root library name, ergo the connection type with which this Connection is associated.
		#
		# ==== Returns
		#
		# A string with the root library name.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		def get_library_root_name
		end


		##
		# Identifies the version information for this Connection's associated middleware.
		#
		# ==== Returns
		#
		# A string with the middleware version information.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		def get_library_version
		end


		##
		# Returns the Specification object associated with the ConnectionManager.
		#
		# ==== Returns
		#
		# A Specification object.
		#
		def get_specification
		end


		##
		# Sets the internal list of fields that are added to all messages that are created
		# using the ConnectionManager.
		#
		# The supplied set of fields will not be validated here; validation occurs at the
		# time a message is to be sent to the GMSEC Bus.
		#
		# ==== Attributes
		#
		# * +standard_fields+ - A FieldArray object containing Field instances.
		#
		def set_standard_fields(standard_fields)
		end


		##
		# Returns a reference to the standard fields that have been set within the ConnectionManager.
		#
		# ==== Returns
		#
		# A FieldArray object.
		#
		def get_standard_fields
		end


		##
		# Adds the standard fields (if any) to the given Message object.
		#
		# ==== Attributes
		#
		# * +msg+ - A Message object.
		#
		# ==== See also
		#
		# ConnectionManager.set_standard_fields
		#
		def add_standard_fields(msg)
		end


		##
		# Registers the given MessageValidator to be used when message validation takes place.
		#
		# Note: Avoid using an anonymous MessageValidator object; retain a reference to ensure it
		# is not garbage collected. Otherwise the application will crash if the object has been destroyed.
		#
		# ==== Attributes
		#
		# * +validator+ - A custom MessageValidator object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the validator object is nil.
		#
		def register_message_validator(validator)
		end


		##
		# Uses the given event callback to notify applications of notable events occuring with
		# the ConnectionManager or the connection to the GMSEC Bus.
		#
		# ==== Attributes
		#
		# * +event+          - An ConnectionEvent enumeration value.
		# * +event_callback+ - The custom ConnectionManagerEventCallback to register for the given event.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the custom ConnectionManagerEventCallback object is nil.
		#
		def register_event_callback(event, event_callback)
		end


		##
		# Subscribes to a particular subject or pattern. If a custom ConnectionManagerCallback object is
		# provided, messages that are received by the ConnectionManager object can be distributed to it
		# by using ConnectionManager.dispatch().
		#
		# Multiple subscriptions can be set up, however applications will not be permitted to set up redundant
		# subscriptions using the same topic and/or custom ConnectionManagerCallback.
		#
		# ==== Attributes
		#
		# * +topic+    - The topic to which to subscribe.
		# * +callback+ - Optional custom ConnectionManagerCallback object.
		# * +config+   - Optional Config object for configuring middleware.
		#
		# ==== Returns
		#
		# A MistSubscriptionInfo object is returned.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the topic is nil or does not represent a well-formed subscription topic.
		#
		# A GmsecException is thrown if a redundant subscription is set up.
		#
		# A GmsecException is thrown if an error occurs at the middleware level.
		#
		# ==== See also
		#
		# ConnectionManager.unsubscribe
		#
		# ConnectionManager.receive
		#
		# ConnectionManager.dispatch
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
        # * +info+ - A MistSubscriptionInfo object.
        #
        # ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
        #
        # A GmsecException is thrown if the given SubscriptionInfo object originated from a different ConnectionManager object.
		#
        # A GmsecException is thrown if an error occurs at the middleware level.
        # 
        # ==== See also
        #
        # ConnectionManager.subscribe
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
		# create a ConnectionManager or Message object with the <i>tracking=off</i> configuration option.
		#
		# ==== Attributes
		#
		# * +msg+    - The Message to publish.
		# * +config+ - Option Config object for configuring middleware-level options.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if an error occurs while attempting to publish the message.
		# 
		# ==== See also
		#
		# ConnectionManager.subscribe
		#
		# ConnectionManager.receive
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
		# create a ConnectionManager or Message object with the <i>tracking=off</i> configuration option.
		#
		# ==== Attributes
		#
		# * +request_msg+  - The request Message to send.
		# * +timeout+      - The maximum time to wait for a response (in milliseconds)
		# * +republish_ms+ - Optional request message resubmission interval (in milliseconds). If set to a negative
		# value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
		# the period will default to 60000ms, unless the user has provided an alternate time period via the
		# Config object used to create the ConnectionManager object. The minimum republish period allowed is 100ms.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if an error occurs while attempting to send the message.
		# 
		# ==== See also
		#
		# ConnectionManager.release
		#
		def request(request_msg, timeout, republish_ms = Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)
		end


		##
		# Sends a reply message in response to a request message.
		#
		# Note: The actual message sent to the middleware will contain tracking fields; to disable this feature,
		# create a ConnectionManager or Message object with the <i>tracking=off</i> configuration option.
		#
		# ==== Attributes
		#
		# * +request_msg+  - The request Message that was received.
		# * +response_msg+ - The reply Message with which to respond.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if an error occurs while attempting to send the message.
		#
		# ==== See also
		#
		# ConnectionManager.receive
		#
		# ConnectionManager.request
		#
		def reply(request_msg, response_msg)
		end


		##
		# Summons any custom ConnectionManagerCallback objects that are subscribed with subject patterns that match
		# the subject of the given Message.
		#
		# ==== Attributes
		#
		# * +msg+ - The Message to dispatch to ConnectionManagerCallback objects.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
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
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if an error occurs while attempting to receive a message.
		#
		# ==== See also
		#
		# ConnectionManager.subscribe
		#
		# ConnectionManager.dispatch
		#
		# ConnectionManager.release
		#
		def receive(timeout = Libgmsec_ruby::GMSEC_WAIT_FOREVER)
		end


		##
		# Releases the resources associated with a Message object that was passed to an application by the ConnectionManager object.
		#
		# Note: This method should <b>not</b> be used to destroy Message object that are not created by the ConnectionManager object.
		#
		# ==== Attributes
		#
		# * +msg+ - The Message object to release.
		#
		# ==== See also
		#
		# ConnectionManager.receive
		#
		# ConnectionManager.request
		#
		def release(msg)
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
		# ConnectionManager.set_name
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
		# ==== Raises
		#
		# A GmsecException is thrown if the name is nil.
		#
		# ==== See also
		#
		# ConnectionManager.get_name
		#
		def set_name(name)
		end


		##
		# Returns the ID associated with the ConnectionManager.
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


		##
		# Creates a Heartbeat Message and passes ownership to the user.
		#
		# This message is populated with the standard set of required and optional heartbeat fields, as well as the
		# required common fields defined in set_standard_fields().
		#
		# ==== Attributes
		#
		# * +subject+         - The topic (subject) that will be applied to the heartbeat message.
		# * +heartbeat_fields - A FieldArray of specialized Fields to be added to the heartbeat message.
		#
		# ==== Returns
		#
		# A heartbeat Message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# ==== See also
		#
		# ConnectionManager.set_standard_fields
		#
		def create_heartbeat_message(subject, heartbeat_fields)
		end


		##
		# Creates a Log Message and passes ownership to the user.
		#
		# This message is populated with the standard set of required and optional log fields, as well as the
		# required common fields defined in set_standard_fields().
		#
		# Tje EVENT-TIME field within the message defaults to the time which this method was invoked; the user
		# may overwrite this time once the message has been returned.
		#
		# ==== Attributes
		#
		# * +subject+   - The topic (subject) that will be applied to the log message.
		# * +log_fields - A FieldArray of specialized Fields to be added to the log message.
		#
		# ==== Returns
		#
		# A log Message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# ==== See also
		#
		# ConnectionManager.set_standard_fields
		#
		def create_log_message(subject, log_fields)
		end


		##
		# Sets the standard default fields which will be added to a new log message when publish_log() is called.
		#
		# ==== Attributes
		#
		# * +subject+   - The topic (subject) that will be applied to the log message.
		# * +log_fields - A FieldArray of specialized Fields to be added to the log message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# ==== See also
		#
		# ConnectionManager.publish_log
		#
		def set_logging_defaults(subject, log_fields)
		end


		##
		# Publishes a Log Message to the GMSEC Bus.
		#
		# Any fields set up with set_logging_defaults and/or with set_standard_fields are also added to the message.
		#
		# If message validation is enabled, the message will be validated prior to being sent. If validation fails,
		# then an exception will be raised.
		#
		# ==== Attributes
		#
		# * +msg_text+       - The string text that will be included within the MSG-TEXT field.
		# * +severity_field+ - The SEVERITY Field object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the Log Message cannot be validated, or if other severe error occurs while
		# attempting to publish the message.
		#
		# ==== See also
		#
		# ConnectionManager.set_logging_defaults
		#
		# ConnectionManager.set_standard_fields
		#
		def publish_log(msg_text, severity_field)
		end


		##
		# Creates and sends a Request Directive Message onto the GMSEC Bus.
		#
		# The Message is populated with the fields that are defined in the message specification, and with the
		# standard fields (if any) associated with the ConnectionManager. The additional fields provided will
		# also be added to the message.
		#
		# The message will be sent using the ConnectionManager.request method to place the message on the GMSEC bus.
		# This implicitly sets the message's RESPONSE field to true, so that receivers of the message will 
		# know a response is necessary.
		#
		# ==== Attributes
		#
		# * +subject+                - The subject which to include with the message.
		# * +directive_string_field+ - A StringField containing the string directive that this message is intended to convey.
		# * +addt_fields+            - A FieldArray of additional Fields that the user wishes to include within the message.
		# * +timeout+                - The amount of time (in milliseconds) to wait for a response.
		# * +republish_ms+           - The equest message resubmission interval (in milliseconds). If set to a negative
		# value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message.  If set to 0,
		# the period will default to 60000ms, unless the user has provided an alternate time period via the
		# Config object used to create the Connection object. The minimum republish period allowed is 100ms.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# A GmsecException is thrown if the Request Directive Message cannot be validated, or if other severe error occurs while
		# attempting to send the message.
		#
		# ==== See also
		#
		# ConnectionManager.set_standard_fields
		#
		# ==== Note
		#
		# Another variation of this method exists, but does not return a response message (because the RESPONSE field will be
		# set to false). An example usage is:
		#
		#    conn_mgr.request_directive(subject, directive_string_field, addt_fields)
		#
		def request_directive(subject, directive_string_field, addt_fields, timeout, republish_ms = Libgmsec_ruby::GMSEC_REQUEST_REPUBLISH_NEVER)
		end


		##
		# Creates and sends a Response Directive Message onto the GMSEC Bus.
		#
		# The Message is populated with the fields that are defined in the message specification, and with the
		# standard fields (if any) associated with the ConnectionManager. The additional fields provided will
		# also be added to the message.
		#
		# The message will be sent using the ConnectionManager.reply method to place the message on the GMSEC bus
		# and route it to the original requestor.
		#
		# ==== Attributes
		#
		# * +subject+         - The subject which to include with the message.
		# * +request_msg+     - The original Request Directive message.
		# * +response_status+ - The ResponseStatus enumerated value for the response message.
		# * +addt_fields+     - A FieldArray of additional Fields that the user wishes to include within the message.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# A GmsecException is thrown if the Response Directive Message cannot be validated, or if other severe error occurs while
		# attempting to send the message.
		#
		# ==== See also
		#
		# ConnectionManager.set_standard_fields
		#
		def acknowledge_directive_request(subject, request_msg, response_status, addt_fields)
		end


		##
		# Creates a Resource Message and publishes it onto the GMSEC Bus.
		#
		# ==== Attributes
		#
		# * +subject+          - The subject to apply to the resource message.
		# * +sample_interval+  - The distance (in seconds) between collecting system resource information.
		# * +average_interval+ - The time interval (in seconds) over which to make a moving average of samples.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# A GmsecException is thrown if a severe error occurs while attempting to send the message.
		#
		def publish_resource_message(subject, sample_interval = 1, average_interval = 10)
		end


		##
		# Returns a Resource Message.
		#
		# ==== Attributes
		#
		# * +subject+          - The subject to apply to the resource message.
		# * +sample_interval+  - The distance (in seconds) between collecting system resource information.
		# * +average_interval+ - The time interval (in seconds) over which to make a moving average of samples.
		#
		# ==== Returns
		#
		# A Resource Message object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# A GmsecException is thrown if a severe error occurs while attempting to send the message.
		#
		def create_resource_message(subject, sample_interval = 1, average_interval = 10)
		end


		##
		# Acknowledges a Simple Service Response Directive Message.
		#
		# The acknowledgment (response) message is populated with the standard
		# fields for this ConnectionManager, as well as required versioning and message description
		# information as described in the message specification.
		# 
		# This method then invokes ConnectionManager.reply to place that message on the GMSEC Bus,
		# and route it to the original requestor.
		#
		# ==== Attributes
		#
		# * +subject+         - The subject to apply to the Response Directive Message.
		# * +request_msg+     - The original Request Directive Message.
		# * +response_status+ - The ResponseStatus to apply to the Response Directive.
		# * +addt_fields+     - A FieldArray of additional Fields that the user wishes to include within the Response Directive.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# A GmsecException is thrown if the message cannot be validated, or if a severe error occurs while attempting to send the message.
		#
		def acknowledge_simple_service(subject, request_msg, response_status, addt_fields)
		end


		##
		# Creates and issues Simple Service Request Directive Message.
		#
		# The request message is populated with the standard fields for this ConnectionManager,
		# as well as required versioning and message description information as described
		# in the message specification.
		#
		# The message will be sent using the ConnectionManager.request method to place the message on the GMSEC bus.
		# This implicitly sets the message's RESPONSE field to true, so that receivers of the message will 
		# know a response is necessary.
		#
		# ==== Attributes
		#
		# * +subject+        - The subject to apply to the Request Directive Message.
		# * +op_name+        - The name of the operation of the service.
		# * +op_number+      - The Field object containing the operation number of the service.
		# * +addt_fields+    - A FieldArray of additional Fields that the user wishes to include within the Request Directive.
		# * +service_params+ - A ServiceParamArray to include within the Request Directive.
		# * +timeout+        - The maximum time to wait for a response (in milliseconds)
		# * +republish_ms+   - Optional request message resubmission interval (in milliseconds). If set to a negative
		# value (eg. GMSEC_REQUEST_REPUBLISH_NEVER) it will never republish a request message. If set to 0,
		# the period will default to 60000ms, unless the user has provided an alternate time period via the
		# Config object used to create the ConnectionManager object. The minimum republish period allowed is 100ms.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the ConnectionManager has not been initialized.
		#
		# A GmsecException is thrown if the subject is nil or is an empty string.
		#
		# A GmsecException is thrown if the message cannot be validated, or if a severe error occurs while attempting to send the message.
		#
		# ==== Note
		#
		# Another variation of this method exists, but does not return a response message (because the RESPONSE field will be
		# set to false). An example usage is:
		#
		#    conn_mgr.request_simple_service(subject, op_name, op_number, addt_fields, service_params)
		#
		def request_simple_service(subject, op_name, op_number, addt_fields, service_params, timeout, republish_ms)
		end
	end
end
