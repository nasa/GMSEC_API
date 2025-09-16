#!/usr/bin/env node


/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * config_file.js
 *
 * Demonstrate reading the contents of a ConfigFile object from a configuration XML file.
 */

const gmsec_api_home = process.env.GMSEC_API_HOME
const gmsec = require(gmsec_api_home + "/bin/gmsec_nodejs.node");

const utility = require("./utility.js");


class config_file
{
	constructor()
	{
		this.cfgFile = new gmsec.ConfigFile();
	}

	run()
	{
		try
		{
			console.log("Loading configuration file: config_file_example.xml\n");
			this.cfgFile.load("config_file_example.xml");

			this.showSubscription("all-messages");
			this.showSubscription("custom1");

			this.showConfig("config1");
			this.showConfig("config2");

			this.showMessage("msg1");
			this.showMessage("msg2");
		}
		catch (e)
		{
			gmsec.Log.error("Exception: " + e.message);
		}
	}

	/**
	 * Print the named subscription entry content.
	 * @param {String} name Subscription name
	 */
	showSubscription(name)
	{
		const sub = this.cfgFile.lookupSubscriptionEntry(name);
			
		console.log("-> Subscription name: " + sub.getName() + "\n-> Subscription pattern: " + sub.getPattern());
		
		// if a subscription has an exclude pattern, it can be iterated through like this
		while (sub.hasNextExcludedPattern()) {
			const exclude = sub.nextExcludedPattern();
			console.log("-> Exclude pattern: " + exclude);
		}
			
		console.log("");
	}

	/**
	 * Print the named config contents
	 * @param {String} name Configuration name
	 */
	showConfig(name)
	{
		const config = this.cfgFile.lookupConfig(name);
			
		console.log("Config <" + name + ">:");
		console.log(config.toXML());
		console.log("");
			
		// Get individual values
		console.log("-> find 'mw-id'    : " + config.getValue("mw-id", "unknown"));
		console.log("-> find 'mw-server': " + config.getValue("mw-server", "unknown"));

		console.log("");
	}

	/**
	 * Print the named Message contents
	 * @param {String} name Message name
	 */
	showMessage(name)
	{
		// the Message is a core gmsec type or class
		const msg = this.cfgFile.lookupMessage(name);
			
		console.log("Message <" + name + ">:\n" + msg.toXML());
			
		// Get fields from message
		console.log("  -> find MISSION-ID: " + msg.getStringValue("MISSION-ID"));
		console.log("  -> find COMPONENT : " + msg.getStringValue("COMPONENT"));

		if (msg.getStringValue("MESSAGE-SUBTYPE") === "HB") {
			console.log("  -> find PUB-RATE  : " + msg.getIntegerValue("PUB-RATE"));
		}
		else if (msg.getStringValue("MESSAGE-SUBTYPE") === "LOG") {
			console.log("  -> find EVENT-TIME: " + msg.getStringValue("EVENT-TIME"));
		}

		console.log("");
	}
}


// Instantiate the config_file example, and run it.
const example = new config_file();
example.run();
