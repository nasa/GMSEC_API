#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T009_ConfigFileIterator extends TestCase
{
	constructor()
	{
		super();
		this.cfgFile = null;
		this.msgFactory = null;
	}

	testCase()
	{
		this.setup();

		this.testIterator();
		this.testReset();
	}

	testIterator()
	{
		var iter = this.cfgFile.getIterator();

		gmsec.Log.info("Test hasNextSubscription() and nextSubscription()");
		var numSubscriptions = 0;
		while (iter.hasNextSubscription()) {
			var entry = iter.nextSubscription();

			numSubscriptions += 1;

			if (numSubscriptions == 1) {
				this.check("Unexpected subscription entry 1 name", entry.getName() === "AllMessages");
				this.check("Unexpected subscription entry 1 pattern", entry.getPattern() === "GMSEC.>");
			}
			else {
				this.check("Unexpected subscription entry 2 name", entry.getName() === "LogMessages");
				this.check("Unexpected subscription entry 2 pattern", entry.getPattern() === "GMSEC.*.*.MSG.LOG.+");
			}
		}

		this.check("Unexpected number of subscription entries", numSubscriptions == 2);


		gmsec.Log.info("Test hasNextConfig() and nextConfig()");
		var numConfigs = 0;
		while (iter.hasNextConfig()) {
			var entry = iter.nextConfig();

			numConfigs += 1;

			if (numConfigs == 1) {
				this.check("Unexpected config entry 1 name", entry.getName() === "ActiveMQ");
				this.check("Unexpected config entry mw-id value", entry.getConfig().getValue("mw-id") === "activemq39");
				this.check("Unexpected config entry mw-server value", entry.getConfig().getValue("mw-server") === "tcp://localhost:61616");
			}
			else {
				this.check("Unexpected config entry 2 name", entry.getName() === "Bolt");
				this.check("Unexpected config entry mw-id value", entry.getConfig().getValue("mw-id") === "bolt");
				this.check("Unexpected config entry mw-server value", entry.getConfig().getValue("mw-server") === "localhost");
			}
		}

		this.check("Unexpected number of config entries", numConfigs == 2);


		gmsec.Log.info("Test hasNextMessage() and nextMessage()");
		var numMessages = 0;
		while (iter.hasNextMessage()) {
			var entry = iter.nextMessage();

			numMessages += 1;

			if (numMessages == 1) {
				this.check("Unexpected message entry 1 name", entry.getName() === "Heartbeat");
				this.check("Unexpected message entry 1", entry.getMessage().toXML() === this.msgFactory.createMessage("HB").toXML());
			}
			else {
				this.check("Unexpected message entry 2 name", entry.getName() === "Log");
				this.check("Unexpected message entry 2", entry.getMessage().toXML() === this.msgFactory.createMessage("LOG").toXML());
			}
		}

		this.check("Unexpected number of message entries", numMessages == 2);
	}

	testReset()
	{
		gmsec.Log.info("Test reset()");

		var iter = this.cfgFile.getIterator();

		this.check("Expected to have subscriptions", iter.hasNextSubscription());
		this.check("Expected to have configs", iter.hasNextConfig());
		this.check("Expected to have messages", iter.hasNextMessage());

		while (iter.hasNextSubscription()) {
			iter.nextSubscription();
		}
		while (iter.hasNextConfig()) {
			iter.nextConfig();
		}
		while (iter.hasNextMessage()) {
			iter.nextMessage();
		}

		this.check("Expected to have subscriptions", iter.hasNextSubscription() == false);
		this.check("Expected to have configs", iter.hasNextConfig() == false);
		this.check("Expected to have messages", iter.hasNextMessage() == false);

		iter.reset();

		this.check("Expected to have subscriptions", iter.hasNextSubscription());
		this.check("Expected to have configs", iter.hasNextConfig());
		this.check("Expected to have messages", iter.hasNextMessage());
	}

	setup()
	{
		if (!this.cfgFile) {
			this.cfgFile = new gmsec.ConfigFile();
		}
		if (!this.msgFactory) {
			this.msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(this.msgFactory);
		}

		this.cfgFile.addSubscriptionEntry(new gmsec.SubscriptionEntry("AllMessages", "GMSEC.>"));
		this.cfgFile.addSubscriptionEntry(new gmsec.SubscriptionEntry("LogMessages", "GMSEC.*.*.MSG.LOG.+"));

		this.cfgFile.addConfig("ActiveMQ", new gmsec.Config("mw-id=activemq39 mw-server=tcp://localhost:61616", gmsec.DataType_KEY_VALUE_DATA));
		this.cfgFile.addConfig("Bolt", new gmsec.Config("mw-id=bolt mw-server=localhost", gmsec.DataType_KEY_VALUE_DATA));

		this.cfgFile.addMessage("Heartbeat", this.msgFactory.createMessage("HB"));
		this.cfgFile.addMessage("Log", this.msgFactory.createMessage("LOG"));
	}
}

test = new T009_ConfigFileIterator();
test.setDescription("Test ConfigFileIterator");
test.run(process.argv);
