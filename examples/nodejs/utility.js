#!/usr/bin/env node


/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * utility.js
 *
 * Defines common utility functions for the JavaScript example programs.
 */


const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");


/**
 * Creates and returns a Config object from the given array of arguments
 *
 * @param {array} argv       An array of strings (e.g., process.argv)
 * @param {bool}  needMwId   Boolean value to indicate whether to check for mw-id (default is true)
 *
 * @return {Config} A GMSEC API Config object
 *
 * @throws {Error} Will throw an Error if the mw-id configuration option is not provided
 */
function createConfig(argv, needMwId = true)
{
	const config = (argv != null && argv.length > 2 ? new gmsec.Config(JSON.stringify(argv)) : new gmsec.Config());

	if (needMwId && config.getValue("mw-id") == null) {
		throw new Error("Missing mw-id configuration option!");
	}

	return config;
}


/**
 * Initializes the GMSEC API's log level to the info level, unless the user has indicated otherwise
 *
 * @param {Config} config   A GMSEC API Config object
 *
 * @throws {Error} Will throw an error if the Config object is null
 */
function initializeLogLevel(config)
{
	if (config == null) {
		throw new Error("Config object is null!");
	}

	const level = gmsec.Log.fromString( config.getValue("loglevel", "info") );
	gmsec.Log.setReportingLevel(level);
}


/**
 * Creates and applies standard fields to the provided GMSEC API MessageFactory object
 *
 * @param {MessageFactory} factory     A GMSEC API MessageFactory object
 * @param {String}         component   The name of the example program
 *
 * @throws {Error} Will throw an error if the MessageFactory object is null
 * @throws {Error} Will throw an error if the component name is null or is an empty string
 */
function setStandardFields(factory, component)
{
	if (factory == null) {
		throw new Error("MessageFactory object is null!");
	}

	if (component == null || component.length == 0) {
		throw new Error("Missing component name");
	}

	factory.setStandardFields( getStandardFields(component) );
}


function getStandardFields(component)
{
	const standardFields = new gmsec.FieldArray();

	standardFields.add(new gmsec.StringField("MISSION-ID", "MY-MISSION", true));
	standardFields.add(new gmsec.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true));
	standardFields.add(new gmsec.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true));
	standardFields.add(new gmsec.StringField("CONSTELLATION-ID", "MY-CNST", true));
	standardFields.add(new gmsec.StringField("FACILITY", "MY-FACILITY", true));
	standardFields.add(new gmsec.StringField("DOMAIN1", "MY-DOMAIN-1", true));
	standardFields.add(new gmsec.StringField("DOMAIN2", "MY-DOMAIN-2", true));
	standardFields.add(new gmsec.StringField("COMPONENT", component.toUpperCase(), true));

	return standardFields;
}


module.exports = { createConfig, initializeLogLevel, setStandardFields, getStandardFields };
