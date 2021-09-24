# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	# This class is an abstract base class for defining a custom message validator.
	#
	# A user created class, derived from this class, can be passed into register_message_validator()
	# to have user code executed when a message is being validated.
	#
	# This custom message validator will be used to supplement the already built-in message validator
	# offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
	# call on the custom message validator, and if the returned status is nominal, then the API's
	# message validator will be called.
	#
	# ==== Example usage
	#
	#     class MyMessageValidator < Libgmsec_ruby::MessageValidator
	#         def initialize
	#             super
	#         end
	#
	#         def validate_message(msg)
	#             status = Libgmsec_ruby::Status.new
	#
	#             # Validate message (e.g. message subject)...
	#
	#             if error
	#                 status = Libgmsec_ruby::Status.new(Libgmsec_ruby::MSG_ERROR, Libgmsec_ruby::MESSAGE_FAILED_VALIDATION, "Invalid subject")
	#             end
	#
	#             return status
	#         end
	#     end
    #
    #     begin
	#         ...
    #         validator = MyMessageValidator.new
    #         conn_mgr.register_message_validator(validator)
	#         ...
    #     end
	#
	# ==== See also
	#
	# * Libgmsec_ruby::Message
	# * Libgmsec_ruby::Status
	# * Libgmsec_ruby::ConnectionManager
	# * Libgmsec_ruby::Specification
	#
	class MessageValidator
	    ##
		# This method is called by the API to perform a user-defined message validation.
		#
		# This custom message validator will be used to supplement the already built-in message validator
		# offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
		# call on the custom message validator, and if the returned status is nominal, then the API's
		# message validator will be called.
		#
		# Note: <b>DO NOT STORE</b> the Message object for use beyond the scope of the function.
		#
		# ==== Attributes
		#
	    # * +msg+ - The Libgmsec_ruby::Message to validate.
		#
		# ==== See also
		#
		# * Libgmsec_ruby::ConnectionManager.register_message_validator
		# * Libgmsec_ruby::Specification.register_message_validator
		#
		def validate_message(msg)
		end
	end
end
