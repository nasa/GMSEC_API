/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/*
 * @file config_file.cs
 *
 * @brief An extended example of reading a ConfigFile XML file and
 * retrieving element information.
 */

using GMSEC.API5;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


/// <summary>
/// Demonstrate reading the contents of a ConfigFile object from a config file xml.
/// </summary>
public class config_file
{
	private readonly ConfigFile configFile;

	private readonly string configFileName;


	public static void Main(string[] args)
	{
		if (args.Length != 1)
		{
			Console.Error.WriteLine("Usage: Provide an example XML file of interest.");
			Environment.Exit(1);
		}

		try
		{
			config_file demo = new config_file(args[0]);

			demo.ShowSubscription("all-messages");
			demo.ShowSubscription("custom1");

			demo.ShowConfig("config1");
			demo.ShowConfig("config2");

			demo.ShowMessage("msg1");
			demo.ShowMessage("msg2");
		}
		catch (GmsecException e)
		{
			Console.WriteLine("config_file open failed [" + args[0] + "] error -- " + e.Message);
		}
	}


	/// <summary>
	/// Constructor, provide the file name of interest. Open the test file.  May result in a GMSECException.
	/// </summary>
	/// <param name="configFilename"> the filename. All filename checking is done by the API. </param>
	public config_file(string configFilename)
	{
		this.configFileName = configFilename;

		configFile = new ConfigFile();

		configFile.Load(configFileName);
		Console.WriteLine("Using config file --> " + configFilename);
	}


	/// <summary>
	/// Print the named subscription entry contents or print that it does not exist, or there is an error. </summary>
	/// <param name="subscriptionName"> a String </param>
	public virtual void ShowSubscription(string subscriptionName)
	{
		SubscriptionEntry sub = configFile.LookupSubscriptionEntry(subscriptionName);
		Console.WriteLine("Subscription <" + subscriptionName + ">:");
		Console.WriteLine("  -> name: " + sub.GetName() + " -> pattern: " + sub.GetPattern());

		// if a subscription has an exclude pattern, it can be iterated through like this
		while (sub.HasNextExcludedPattern())
		{
			string exclude = sub.NextExcludedPattern();
			Console.WriteLine("     -> exclude: " + exclude);
		}

		Console.WriteLine();
	}


	/// <summary>
	/// Print the named config contents or print that it does not exist, or there is an error. </summary>
	/// <param name="configName"> a String </param>
	public virtual void ShowConfig(string configName)
	{
		Config config = configFile.LookupConfig(configName);

		Console.WriteLine("Config <" + configName + ">:");

		// the way to get the guts of the config is using entrySet
		ConfigPair cp = new ConfigPair();
		bool hasPair = config.GetFirst(cp);
		while (hasPair)
		{
			Console.WriteLine("  -> key: " + cp.GetName() + " -> value: " + cp.GetValue());
			hasPair = config.GetNext(cp);
		}

		// but individual values may be found also
		Console.WriteLine();
		string mwid = config.GetValue("mw-id");
		if (mwid != null)
		{
			Console.WriteLine("  -> find 'mw-id': " + config.GetValue("mw-id"));
		}
		else
		{
			Console.WriteLine("  -> 'mw-id'is not defined in this config.");
		}

		// or the entire thing can be printed as XML (or JSON)
		Console.WriteLine();
		Console.WriteLine(config.ToXML());
		Console.WriteLine();
	}


	/// <summary>
	/// Print the named message contents or print that it does not exist, or there is an error. </summary>
	/// <param name="configName"> a String </param>
	public virtual void ShowMessage(string messageName)
	{
		// the Message is a core gmsec type or class
		Message msg = configFile.LookupMessage(messageName);

		Console.WriteLine("Message <" + messageName + ">:");

		// there are number getters such as this one
		Console.WriteLine("  -> find 'STRING-FIELD': " + msg.GetStringField("STRING-FIELD").GetStringValue());
		Console.WriteLine();

		// or the entire thing can be printed as XML (or JSON)
		Console.WriteLine(msg.ToXML());
		Console.WriteLine();
	}
}
