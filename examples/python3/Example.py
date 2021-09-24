#!/usr/bin/env python3


# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


import libgmsec_python3
import sys

class Example(object):

	@staticmethod
	def initialize(args):
		config = libgmsec_python3.Config(args)
		config.add_value("loglevel", config.get_value("loglevel", "info"))
		config.add_value("logfile", config.get_value("logfile", "stderr"))
		return config


	@staticmethod
	def addToConfigFromFile(config):
		# This assumes, for now, only one cfgfile=filename.xml arg is specified
		cfgFilename = config.get_value("cfgfile")
		if cfgFilename != None and cfgFilename != "":
			contents = ''

			try:
				with open(cfgFilename, 'r') as fileStream:
					contents += fileStream.read()
				fileStream.close()

				if contents != '':
					configFromFile = libgmsec_python3.Config()
					configFromFile.from_XML(contents)

					cp = libgmsec_python3.ConfigPair()
					status = configFromFile.getFirst(cp)
					while status:
						config.add_value(cp.get_name(), cp.get_value())
						status = configFromFile.getNext(cp)
			except GmsecError:
				libgmsec_python3.log_warning("Invalid filepath " + cfgFilename + " given; ignoring it.")


	@staticmethod
	def isOptionInValid(config):
		value = config.get_value("mw-id")
		if (value == None):
			value = config.get_value("connectionType")
			if (value == None):
				return False
		return True
