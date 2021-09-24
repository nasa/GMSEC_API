# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# This file is used for generating Ruby documentation; it only contains
# stubs of constructors and methods associated with the class.


module Libgmsec_ruby #:nodoc: don't document this

	##
	# This class is a container for holding information on a Device.
	#
	class Device
		class DeviceStatus
			##
			# ==== Enumerated values
			#
			# DEBUG = 0
			#
			# NOMINAL_GREEN = 1
			#
			# YELLOW = 2
			#
			# ORANGE =3
			#
			# RED = 4
			#
			def DeviceStatus_Enum
			end
		end


		##
		# Constructor.
		#
		# ==== Attributes
		#
		# * +name+          - The name of this device.
		# * +device_status+ - The DeviceStatus for this device.
		# * +device_params+ - The DeviceParamArray with DeviceParam objects.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device name is nil or is an empty string.
		#
		# ==== Other constructors
		#
		# * Copy constructor.
		#
		#     Libgmsec_ruby::Device.new(other_device)
		#
		def initialize(name, device_status, device_params)
		end


		##
		# Returns the name of the device.
		#
		# ==== Returns
		#
		# The device name as a string.
		#
		def get_name
		end


		##
		# Returns the availability of a device number.
		#
		# ==== Returns
		#
		# Returns true if a device number is available; false otherwise.
		#
		def number_available
		end


		##
		# Returns the device number Field associated with the device.
		#
		# ==== Returns
		#
		# A Field object representing the device number.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device number has not been set.
		#
		def get_number
		end


		##
		# Assigns the device a number represented as a Field.
		#
		# ==== Attributes
		#
		# * +number_field+ - Field containing device number.
		#
		def set_number(number_field)
		end


		##
		# Returns the availability of a model number.
		#
		# ==== Returns
		#
		# Returns true if a model number is available; false otherwise.
		#
		def model_available
		end


		##
		# Returns the model number associated with the device.
		#
		# ==== Returns
		#
		# A string representing the device model number.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device model number has not been set.
		#
		def get_model
		end


		##
		# Assigns the device a model number.
		#
		# ==== Attributes
		#
		# * +model_number+ - The device model number as represented by a string.
		#
		def set_model(model_number)
		end


		##
		# Returns the availability of a serial number.
		#
		# ==== Returns
		#
		# Returns true if a serial number is available; false otherwise.
		#
		def serial_available
		end


		##
		# Returns the serial number associated with the device.
		#
		# ==== Returns
		#
		# A string representing the device serial number.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device serial number has not been set.
		#
		def get_serial
		end


		##
		# Assigns the device a serial number.
		#
		# ==== Attributes
		#
		# * +serial_number+ - The device serial number as represented by a string.
		#
		def set_serial(serial_number)
		end


		##
		# Returns the availability of a version number.
		#
		# ==== Returns
		#
		# Returns true if a version number is available; false otherwise.
		#
		def version_available
		end


		##
		# Returns the version number associated with the device.
		#
		# ==== Returns
		#
		# A string representing the device version number.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device version number has not been set.
		#
		def get_version
		end


		##
		# Assigns the device a version number.
		#
		# ==== Attributes
		#
		# * +version_number+ - The device version number as represented by a string.
		#
		def set_version(version_number)
		end


		##
		# Returns the availability of a device group.
		#
		# ==== Returns
		#
		# Returns true if a device group is available; false otherwise.
		#
		def group_available
		end


		##
		# Returns the group name associated with the device.
		#
		# ==== Returns
		#
		# A string representing the device group.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device group has not been set.
		#
		def get_group
		end


		##
		# Assigns the device a group name.
		#
		# ==== Attributes
		#
		# * +group+ - The device group name as represented by a string.
		#
		def set_group(group)
		end


		##
		# Returns the availability of a device role.
		#
		# ==== Returns
		#
		# Returns true if a device role is available; false otherwise.
		#
		def role_available
		end


		##
		# Returns the role associated with the device.
		#
		# ==== Returns
		#
		# A string representing the device role.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device role has not been set.
		#
		def get_role
		end


		##
		# Assigns the device a role name.
		#
		# ==== Attributes
		#
		# * +role+ - The device role name as represented by a string.
		#
		def set_role(role)
		end


		##
		# Returns the status associated with the device.
		#
		# ==== Returns
		#
		# A DeviceStatus enumerated value.
		#
		def get_status
		end


		##
		# Assigns the device a status.
		#
		# ==== Attributes
		#
		# * +device_status+ - A DeviceStatus enumerated value.
		#
		def set_status(device_status)
		end


		##
		# Returns the availability of a device information.
		#
		# ==== Returns
		#
		# Returns true if a device information is available; false otherwise.
		#
		def info_available
		end


		##
		# Returns the info associated with the device.
		#
		# ==== Returns
		#
		# A Field containing device information.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the device information field has not been set.
		#
		def get_info
		end


		##
		# Assigns the device an information field.
		#
		# ==== Attributes
		#
		# * +info_field+ - A Field containing device information.
		#
		def set_info(info_field)
		end


		##
		# Returns the number of device parameters held by the device.
		#
		# ==== Returns
		#
		# The number of device parameters.
		#
		def get_param_count
		end


		##
		# Returns a reference to a DeviceParam held by the device.
		#
		# ==== Attributes
		#
		# * +index+ - A zero-based index of the DeviceParam object to reference.
		#
		# ==== Returns
		#
		# A DeviceParam object.
		#
		# ==== Raises
		#
		# A GmsecException is thrown if the supplied index is out of bounds.
		#
		def get_param(index)
		end


		##
		# Adds a single DeviceParam object to the device.
		#
		# ==== Attributes
		#
		# + *device_param* - The DeviceParam object to add to the device.
		#
		def add_param(device_param)
		end
	end
end
