#!/usr/bin/env python

"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""


import libgmsec_python
import sys

class Example(object):

	@staticmethod
	def initialize(args):
		config = libgmsec_python.Config()
		for arg in args[1:]:
			value = arg.split('=')
			config.addValue(value[0], value[1])
		config.addValue("loglevel", config.getValue("loglevel", "info"))
		config.addValue("logfile", config.getValue("logfile", "stderr"))
		return config


	@staticmethod
	def addToConfigFromFile(config):
		# This assumes, for now, only one cfgfile=filename.xml arg is specified
		cfgFilename = config.getValue("cfgfile")
		if cfgFilename != None and cfgFilename != "":
			contents = ''

			try:
				with open(cfgFilename, 'r') as fileStream:
					contents += fileStream.read()
				fileStream.close()

				if contents != '':
					configFromFile = libgmsec_python.Config()
					configFromFile.fromXML(contents)

					cp = libgmsec_python.ConfigPair()
					status = configFromFile.getFirst(cp)
					while status:
						config.addValue(cp.getName(), cp.getValue())
						status = configFromFile.getNext(cp)
			except Exception:
				libgmsec_python.logWarning("Invalid filepath " + cfgFilename + " given; ignoring it.")


	@staticmethod
	def isOptionInValid(config):
		value = config.getValue("mw-id")
		if (value == None):
			value = config.getValue("connectionType")
			if (value == None):
				return False
		return True
