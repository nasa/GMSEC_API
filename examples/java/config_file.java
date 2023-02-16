/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.ConfigFile;
import gov.nasa.gsfc.gmsec.api5.ConfigFile.SubscriptionEntry;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;

import java.util.Map.Entry;


/**
 * Demonstrate reading the contents of a ConfigFile object from a config file xml.
 */
public class config_file {
	
	private ConfigFile configFile;
	
	private String configFileName;

	/** 
	 * Constructor, provide the file name of interest.
	 * 
	 * @param configFilename the filename.  All filename checking is done by the API.
	 */
	public config_file(String configFilename) throws GmsecException {
		this.configFileName = configFilename;

		configFile = new ConfigFile();

		// generally all the gmsec-api fail with exceptions

		configFile.load(configFileName);
		System.out.println("Using config file --> " + configFilename);

	}
	

	
	/**
	 * Print the named subscription entry contents or print that it does not exist, or there is an error.
	 * @param subscriptionName a String
	 */
	public void showSubscription(String subscriptionName) throws GmsecException {
	
			SubscriptionEntry sub = configFile.lookupSubscriptionEntry(subscriptionName);
			
			System.out.println("Subscription <" + subscriptionName + ">:");
			System.out.println("  -> name: " + sub.getName() + " -> pattern: " + sub.getPattern());
		
			// if a subscription has an exclude pattern, it can be iterated through like this
			while (sub.hasNextExcludedPattern()) {
				String exclude = sub.nextExcludedPattern();
				System.out.println("     -> exclude: " + exclude);
			}
			
			System.out.println();

	}
	
	/**
	 * Print the named config contents or print that it does not exist, or there is an error.
	 * @param configName a String
	 */
	public void showConfig(String configName) throws GmsecException {


			Config config = configFile.lookupConfig(configName);
			
			System.out.println("Config <" + configName + ">:");
			
			// the way to get the guts of the config is using entrySet
			for (Entry<String, String> entry: config.entrySet()) {
				System.out.println("  -> key: " + entry.getKey() + " -> value: " + entry.getValue());
			}
			
			// but individual values may be found also
			System.out.println();
			String mwid = config.getValue("mw-id");
			if (mwid != null) {
				System.out.println("  -> find 'mw-id': " + config.getValue("mw-id"));
			} else {
				System.out.println("  -> 'mw-id'is not defined in this config.");
			}
			
			// or the entire thing can be printed as XML (or JSON)
			System.out.println();
			System.out.println(config.toXML());
			System.out.println();

	}
	
	
	/**
	 * Print the named message contents or print that it does not exist, or there is an error.
	 * @param configName a String
	 */
	public void showMessage(String messageName) throws GmsecException {

			// the Message is a core gmsec type or class
			Message msg = configFile.lookupMessage(messageName);
			
			System.out.println("Message <" + messageName + ">:");
			
			// there are number getters such as this one
			System.out.println("  -> find 'STRING-FIELD': " + msg.getStringField("STRING-FIELD").getStringValue());
			System.out.println();
			
			// or the entire thing can be printed as XML (or JSON)
			System.out.println(msg.toXML());
			System.out.println();

	}
	

	public static void main(String[] args) {
		if (args.length != 1) {
			System.err.println("Usage: Provide an example XML file of interest.");
			System.exit(1);
		}

		try {
			config_file demo = new config_file(args[0]);

			demo.showSubscription("all-messages");
			demo.showSubscription("custom1");

			demo.showConfig("config1");
			demo.showConfig("config2");

			demo.showMessage("msg1");
			demo.showMessage("msg2");

		} catch (GmsecException e) {
			System.out.println("ConfigFileDemo failed [" + args[0] + "] error -- " + e.getMessage());
		}

	}

}
