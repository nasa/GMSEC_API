# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.
#


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class contains information the user has supplied when setting up a subscription.
	#
	# ==== See also
	#
	# Connection.subscribe
	#
	# Connection.unsubscribe
	#
	class SubscriptionInfo
		##
		# Returns the subscription topic used to set up a subscription.
		#
		def get_subject
		end


		##
		# Returns the address of the underlying C++ callback.
		#
		# Note: Calling this method from a Ruby context serves no purpose.
		#
		def get_callback
		end
	end
end
