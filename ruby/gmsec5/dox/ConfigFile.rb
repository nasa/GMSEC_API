# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	# This class provides the means for managing a configuration file.
	#
	# A configuration file must have a format similar to the following (the ordering of the definitions does not matter):
	#
	#     <DEFINITIONS>
	#         <CONFIG NAME="Bolt">
	#             <PARAMETER NAME="mw-id">bolt</PARAMETER>
	#             <PARAMETER NAME="server">localhost</PARAMETER>
	#         </CONFIG>
	#
	#         <CONFIG NAME="ActiveMQ">
	#             <PARAMETER NAME="mw-id">activemq39</PARAMETER>
	#             <PARAMETER NAME="server">tcp:://localhost:61616</PARAMETER>
	#         </CONFIG>
	#
	#         <SUBSCRIPTION NAME="AllMessages" PATTERN="GMSEC.&amp;"/>
	#
	#         <SUBSCRIPTION NAME="AllMessagesExceptHB" PATTERN="GMSEC.&amp;">
	#             <EXCLUDE PATTERN="GMSEC.*.*.MSG.HB.+">
	#         </SUBSCRIPTION>
	#
	#         <MESSAGE NAME="CustomMessage" SUBJECT="GMSEC.FOO.BAR" KIND="PUBLISH">
	#             <FIELD NAME="FIELD-1" TYPE="STRING">Field One</FIELD>
	#             <FIELD NAME="FIELD-2" TYPE="I16">1234</FIELD>
	#         </MESSAGE>
	#
	#         <CUSTOM NAME="StandardFields">
	#             <FIELD NAME="MISSION-ID" TYPE="STRING">MY-MISSION</FIELD>
	#             <FIELD NAME="CONSTELLATION-ID" TYPE="STRING">MY-CONST-ID</FIELD>
	#             <FIELD NAME="SAT-ID-PHYSICAL" TYPE="STRING">MY-SAT-ID</FIELD>
	#         </CUSTOM>
	#     </DEFINITIONS>
	#
	class ConfigFile
		##
        # ==== Basic Constructor: new()
        # Constructor that accepts no arguments.
        # The internal held MessageFactory will be configured with the default message specification used by the API.
        #
        # ==== Alternative Constructor: new(Config)
		# Constructor that accepts a Config object.
        # The given configuration will be applied to the internally held MessageFactory.
        #
		# ==== Attributes
		# * +config+ - The configuration to apply to the internally held MessageFactory.
        #
        # Configuration options referenced are:
        #       gmsec-specification-version
        #       gmsec-schema-level
        #       gmsec-msg-content-validate
		#
        # ==== Returns
        # A ConfigFile object containing no definitions.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the MessageFactory cannot be instantiated.
        #
		def initialize()
		end


		##
		# Reads the given configuration file and parses the XML content.
		#
		# ==== Attributes
		#
		# * +filename+ - The file to parse.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if an error occurs while trying to parse the file.
		#
		def load(filename)
		end


		##
		# Saves the currently open configuration file to the location specified.
		#
		# ==== Attributes
		#
		# * +filename+ - The file to which the configuration file will be saved; if the path is nil, then the path, if any, that was used with load() will be used.
		# * +compact+  - Boolean flag to indicate whether the XML output will be compacted or not; default is false.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if an error occurs while trying to write the file.
		#
		def save(filename = nil, compact = false)
		end


		##
		# Parses the given XML string representing a configuration file.
		#
		# ==== Attributes
		#
		# * +xml+ - XML string that represents a configuration file.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if an error occurs while parsing the XML string.
		#
		def from_xml(xml)
		end


		##
		# Produces the XML string representation of the loaded configuration file.
		#
		# ==== Returns
		#
		# An XML string that represents the loaded configuration file.
		#
		def to_xml
		end


		##
		# Reports whether a configuration file has been loaded.
		#
		# ==== Returns
		#
		# A boolean value of true is returned if a configuration file has been loaded; false otherwise.
		#
		def is_loaded
		end


		##
		# Adds a CONFIG definition to the configuration file.
		#
		# Note: Adding a CONFIG entry with a name that currently exists will overwrite the existing entry.
		#
		# ==== Attributes
		#
		# * +name+   - The name of the CONFIG entry to add.
		# * +config+ - A Config object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the given name is nil or contains an empty string.
		#
		def add_config(name, config)
		end


		##
		# Returns a Config object for the named CONFIG entry.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the CONFIG entry to look up.
		#
		# ==== Returns
		#
		# A Config object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a CONFIG entry cannot be found for the given name.
		#
		def lookup_config(name)
		end


		##
		# Removes the CONFIG definition from the configuration file.
		#
		# ==== Attributes
		#
		# * +name+ - The name identifying the CONFIG element to remove.
		#
		# ==== Returns
		#
		# Returns true if the removal was successful, false if not.
		#
		def remove_config(name)
		end


		##
		# Adds a MESSAGE definition to the configuration file.
		#
		# Note: Adding a MESSAGE entry with a name that currently exists will overwrite the existing entry.
		#
		# ==== Attributes
		#
		# * +name+    - The name of the MESSAGE entry to add.
		# * +message+ - A Message object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the given name is nil or contains an empty string.
		#
		def add_message(name, message)
		end


		##
		# Returns a Message object for the named MESSAGE entry.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the MESSAGE entry to look up.
		#
		# ==== Returns
		#
		# A Message object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a MESSAGE entry cannot be found for the given name.
		#
		def lookup_message(name)
		end


		##
		# Removes the MESSAGE definition from the configuration file.
		#
		# ==== Attributes
		#
		# * +name+ - The name identifying the MESSAGE element to remove.
		#
		# ==== Returns
		#
		# Returns true if the removal was successful, false if not.
		#
		def remove_message(name)
		end


		##
		# Adds a SUBSCRIPTION definition to the configuration file.
		#
		# Note: Adding a SUBSCRIPTION entry with a name that currently exists will overwrite the existing entry.
		#
		# ==== Attributes
		#
		# * +name+  - The name of the SUBSCRIPTION entry to add.
		# * +entry+ - A SubscriptionEntry object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if either name is nil or if it contains an empty string.
		#
		def add_subscription_entry(name, entry)
		end


		##
		# Returns a SubscriptionEntry object for the named SUBSCRIPTION entry.
		#
		# ==== Attributes
		#
		# * +name+ - The name of the SUBSCRIPTION entry to look up.
		#
		# ==== Returns
		#
		# A SubscriptionEntry object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if a SUBSCRIPTION entry cannot be found for the given name.
		#
		def lookup_subscription_entry(name)
		end


		##
		# Removes the subscription definition which is mapped to the given name.
		#
		# ==== Attributes
		#
		# * +name+ - The name identifying the SUBSCRIPTION element to remove.
		#
		# ==== Returns
		#
		# Returns true if the removal was successful, false if not.
		#
		def remove_subscription_entry(name)
		end


		##
		# Provides access to the ConfigFileIterator associated with the configuration file.
		#
		# This iterator will allow for an application to iterate over the entries within the ConfigFile object.
		# This iterator is reset each time get_iterator() is called. The iterator itself is destroyed when the
		# ConfigFile object is destroyed.
		#
		# Note: Only one ConfigFileIterator object is associated with the ConfigFile object; multiple calls to
		# get_iterator() will return a reference to the same object, and each call will reset the iterator.
		#
		# ==== Returns
		#
		# A reference to a ConfigFileIterator.
		#
		def get_iterator
		end
	end
end
