# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class is used to access elements of a ConfigFile object.
	#
	class ConfigFileIterator
		# Reports whether there is a next CONFIG entry available via the iterator.
		#
		# ==== Returns
		#
		# Returns true if another CONFIG entry is available, false otherwise.
		#
		def has_next_config
		end


		# Reports whether there is a next MESSAGE entry available via the iterator.
		#
		# ==== Returns
		#
		# Returns true if another MESSAGE entry is available, false otherwise.
		#
		def has_next_message
		end


		# Reports whether there is a next SUBSCRIPTION entry available via the iterator.
		#
		# ==== Returns
		#
		# Returns true if another SUBSCRIPTION entry is available, false otherwise.
		#
		def has_next_subscription
		end


		# Reports whether there is a next SUBSCRIPTION entry available via the iterator.
		#
		# ==== Returns
		#
		# Returns true if another SUBSCRIPTION entry is available, false otherwise.
		#
		def has_next_subscription
		end


		# Reports whether there is a next CUSTOM entry available via the iterator.
		#
		# ==== Returns
		#
		# Returns true if another CUSTOM entry is available, false otherwise.
		#
		def has_next_custom_element
		end


		##
		# Provides a reference to a ConfigEntry object that represents the next CONFIG entry.
		#
		# ==== Returns
		#
		# A reference to a ConfigEntry.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if no more CONFIG entries are available via the iterator.
		#
		# ==== See also
		#
		# ConfigFileIterator.has_next_config
		#
		def next_config
		end


		##
		# Provides a reference to a MessageEntry object that represents the next MESSAGE entry.
		#
		# ==== Returns
		#
		# A reference to a MessageEntry.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if no more MESSAGE entries are available via the iterator.
		#
		# ==== See also
		#
		# ConfigFileIterator.has_next_message
		#
		def next_message
		end


		##
		# Provides a reference to a SubscriptionEntry object that represents the next SUBSCRIPTION entry.
		#
		# ==== Returns
		#
		# A reference to a SubscriptionEntry.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if no more SUBSCRIPTION entries are available via the iterator.
		#
		# ==== See also
		#
		# ConfigFileIterator.has_next_subscription
		#
		def next_subscription
		end


		##
		# Returns an XML string representing the next available CUSTOM entry.
		#
		# ==== Returns
		#
		# An XML string.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if no more CUSTOM entries are available via the iterator.
		#
		# ==== See also
		#
		# ConfigFileIterator.has_next_custom_element
		#
		def next_custom_element
		end


		##
		# Resets all iterators to the beginning of the respective lists that are maintained by the ConfigFile.
		#
		def reset
		end
	end
end
