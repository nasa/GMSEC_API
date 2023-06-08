#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');
const {fork} = require('child_process');


class T010_Connection extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor();
		this.testGetAPIVersion();
		this.testConnect();
		this.testDisconnect();
		this.testGetLibraryName();
		this.testGetLibraryVersion();
		this.testGetConfig()
		this.testGetMessageFactory();
		this.testSetupSubscription();
		this.testUnsubscribe();
		this.testPublish();
		this.testPublishWithConfig();
		this.testRequest();
		this.testReply();
		this.testReceive();
		this.testExcludeSubject();
		this.testRemoveExcludedSubject();
		this.testGetSetName();
		this.testGetID();
		this.testGetMWInfo();
		this.testGetConnectionEndpoint();
	}

	testConstructor()
	{
		gmsec.Log.info("Test constructor");

		// Nominal test
		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.check("Okay", true);
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal test
		try {
			// missing mw-id
			var conn = new gmsec.Connection(new gmsec.Config());
			this.check("Expected an exception", false);
		}
		catch (e) {
			//SWIG BUG this.check(e.message, e.message.includes("mw-id in Config is not specified"));
			this.check(e.message, e.message.includes("Illegal arguments for construction of _exports_Connection"));
		}

		try {
			// bogus middleware
			var conn = new gmsec.Connection(new gmsec.Config("mw-id=bogus", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			//SWIG BUG this.check(e.message, e.message.includes("Unable to load"));
			this.check(e.message, e.message.includes("Illegal arguments for construction of _exports_Connection"));
		}
	}

	testGetAPIVersion()
	{
		gmsec.Log.info("Test getAPIVersion()");
		var version = "GMSEC API v" + gmsec.GMSEC_VERSION_NUMBER;
		this.check("Unexpected version info", gmsec.Connection.getAPIVersion().includes(version));
	}

	testConnect()
	{
		gmsec.Log.info("Test connect()");

		// Nominal test
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			this.check("Okay", true);
			conn.disconnect();
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal test
		try {
			var config = new gmsec.Config("mw-id=bolt server=gs580s-gc764mw1:1234", gmsec.DataType_KEY_VALUE_DATA);
			var conn = new gmsec.Connection(config);
			conn.connect();
			this.check("Expected error to indicate unable to connect", false);
		}
		catch (e) {
			var ex = new gmsec.GmsecException(e.message);
			var result = (ex.getErrorClass() == gmsec.MIDDLEWARE_ERROR)  &&
                         (ex.getErrorCode()  == gmsec.CUSTOM_ERROR_CODE) &&
			             (ex.getCustomCode() == 7);
			this.check(e.message, result);
		}
	}

	testDisconnect()
	{
		gmsec.Log.info("Test disconnect()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.disconnect();
			this.check("Okay", true);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetLibraryName()
	{
		gmsec.Log.info("Test getLibraryName()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.check("Unexpected library name", conn.getLibraryName().includes(this.getConfig().getValue("mw-id", "unknown")));
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetLibraryVersion()
	{
		gmsec.Log.info("Test getLibraryVersion()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.check("Unexpected library version", conn.getLibraryVersion() != null);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetConfig()
	{
		gmsec.Log.info("Test getConfig()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.check("Unexpected Config", conn.getConfig() != null);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetMessageFactory()
	{
		gmsec.Log.info("Test getMessageFactory()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.check("Expected MessageFactory for the CURRENT specification", conn.getMessageFactory().getSpecification().getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetupSubscription()
	{
		gmsec.Log.info("Test setupSubscription()");

		// Nominal tests
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.setupSubscription(this.getSubject("*.BAR"));
			conn.setupSubscription(this.getSubject("FOO.BAZ"));

			var msg1 = conn.getMessageFactory().createMessage("LOG");
			var msg2 = conn.getMessageFactory().createMessage("LOG");
			var msg3 = conn.getMessageFactory().createMessage("LOG");

			msg1.setSubject(this.getSubject("GIN.BAR"));
			msg2.setSubject(this.getSubject("FOO.BAZ"));
			msg3.setSubject(this.getSubject("FOO.GAZ"));

			conn.publish(msg1);
			var msg = conn.receive(5000);
			this.require("Was expecting to receive a message", msg != null);
			this.check("Unexpected message subject", msg.getSubject() == msg1.getSubject());

			conn.publish(msg2);
			msg = conn.receive(5000);
			this.require("Was expecting to receive a message", msg != null);
			this.check("Unexpected message subject", msg.getSubject() == msg2.getSubject());

			conn.publish(msg3);
			msg = conn.receive(5000);
			this.require("Was expecting to receive a message", msg == null);

			conn.disconnect();
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal tests
		try {
			// duplicate subscription
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.setupSubscription("GMSEC.>");
			conn.setupSubscription("GMSEC.>");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Duplicate subscription"));
		}

		try {
			// subscribe before connect
			var conn = new gmsec.Connection(this.getConfig());
			conn.setupSubscription("GMSEC.>");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Connection has not been initialized"));
		}

		try {
			// illegal subscription topic
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.setupSubscription("GMSEC.MY MISSION.*.*.>");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Subject is invalid"));
		}
	}

	testUnsubscribe()
	{
		gmsec.Log.info("Test unsubscribe()");

		// Nominal test
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			var info = conn.setupSubscription(this.getSubject("FOO.BAR"));
			conn.unsubscribe(info);

			var msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject(this.getSubject("FOO.BAR"));

			conn.publish(msg);

			msg = conn.receive(5000);
			this.check("Did not expect to receive a message", msg == null);
		}
		catch (e) {
			test.require(e.message, false);
		}

		// Off-nominal test
		try {
			var conn1 = new gmsec.Connection(this.getConfig());
			var conn2 = new gmsec.Connection(this.getConfig());

			conn1.connect();
			conn2.connect();

			var info1 = conn1.setupSubscription("GMSEC.FOO.BAR");

			conn2.unsubscribe(info1);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("SubscriptionInfo object is not associated with this Connection"));
		}
	}

	testPublish()
	{
		gmsec.Log.info("Test publish()");

		// Nominal test
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.setupSubscription(this.getSubject(">"));

			this.setStandardFields(conn.getMessageFactory());

			var msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject(this.getSubject("HB"));

			conn.publish(msg);

			var rcvd = conn.receive(5000);

			if (!rcvd) {
				this.check("Failed to receive published message", false);
			}
			else {
				this.check("Unexpected subject", rcvd.getSubject() === this.getSubject("HB"));
				this.check("Unexpected message kind", rcvd.getKind() == gmsec.Message.Kind_PUBLISH);
			}
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal test
		try {
			// Message validation fails (missing standard fields)
			var config = new gmsec.Config(this.getConfig());   // make a copy!
			config.addValue("gmsec-msg-content-validate", "true");

			var conn = new gmsec.Connection(config);
			var msg = conn.getMessageFactory().createMessage("HB");

			conn.connect();
			conn.publish(msg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Connection has not been initialized
			var conn = new gmsec.Connection(this.getConfig());
			var msg = conn.getMessageFactory().createMessage("HB");
			conn.publish(msg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Connection has not been initialized"));
		}

		try {
			// Connection has not been initialized
			var conn = new gmsec.Connection(this.getConfig());
			var msg = conn.getMessageFactory().createMessage("REQ.DIR");
			conn.connect();
			conn.publish(msg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Cannot publish message with non-PUBLISH message kind"));
		}
	}

	testPublishWithConfig()
	{
		gmsec.Log.info("Test publishWithConfig()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.setStandardFields(conn.getMessageFactory());

			var msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject(this.getSubject("MW-CONFIG"));

			var mwConfig = new gmsec.Config();

			conn.connect();
			conn.setupSubscription(this.getSubject("MW-CONFIG"));
			conn.publishWithConfig(msg, mwConfig);

			var rcvd = conn.receive(5000);

			if (!rcvd) {
				this.check("Failed to receive a message", false);
			}
			else {
				this.check("Received a message", true);
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testRequest()
	{
		gmsec.Log.info("Test request()");

		// Nominal case
		try {
			var responseSubject = this.getSubject("RESP.DIR");
			var requestSubject  = this.getSubject("REQ.DIR");
			var config = new gmsec.Config(this.getConfig());   // make a copy!
			config.addValue("tracking", "false");

			var conn = new gmsec.Connection(config);
			conn.connect();
			conn.setupSubscription(responseSubject);

			this.setStandardFields(conn.getMessageFactory());

			var reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			reqMsg.setSubject(requestSubject);
			reqMsg.addField(new gmsec.StringField("DIRECTIVE-STRING", "Do something!"));
			reqMsg.addField(new gmsec.StringField("DESTINATION-COMPONENT", "RESPONDER", true));
			reqMsg.addField(new gmsec.U16Field("REQUEST-ID", 0));

			var cfg = new gmsec.Config(config);   // make a copy
			cfg.addValue("req-subject", requestSubject);
			cfg.addValue("rep-subject", responseSubject);
			let childProcess = fork(this.getDataFile("T010_Responder.js"));
			childProcess.on('message', () => {childProcess.kill("SIGINT")});
			childProcess.on('exit', () => {});
			childProcess.send(cfg.toXML());

			// Allow time for forked process to start
			gmsec.TimeUtil.millisleep(10000);

			var repMsg1 = conn.request(reqMsg, 5000, gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);

			if (!repMsg1) {
				this.check("Expected to receive a response message", false);
			}
			else {
				this.check("Unexpected response message kind", repMsg1.getKind() == gmsec.Message.Kind_REPLY);
				this.check("Unexpected response message subject", repMsg1.getSubject().includes(this.getSubject("RESP.DIR")));
			}

			reqMsg.addField(new gmsec.U16Field("REQUEST-ID", 1));

			var repMsg2 = conn.request(reqMsg, 5000, gmsec.GMSEC_REQUEST_REPUBLISH_NEVER);

			if (repMsg2 != null) {
				this.check("Was not expecting to receive response message", false);
			}
			else {
				this.check("Okay, no message received", true);
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testReply()
	{
		gmsec.Log.info("Test reply()");

		// Nominal test for Connection.reply() tested earlier (via addons/T010_Responder.js)

		// Off-nominal tests
		try {
			// Connection not initialized
			var conn = new gmsec.Connection(this.getConfig());
			var reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			var repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
			conn.reply(reqMsg, repMsg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Connection has not been initialized"));
		}

		try {
			// Bad request message
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			var reqMsg = conn.getMessageFactory().createMessage("HB");
			var repMsg = conn.getMessageFactory().createMessage("RESP.DIR");
			conn.reply(reqMsg, repMsg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Cannot issue reply with non-REQUEST kind message"));
		}

		try {
			// Bad reply message
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			var reqMsg = conn.getMessageFactory().createMessage("REQ.DIR");
			var repMsg = conn.getMessageFactory().createMessage("HB");
			conn.reply(reqMsg, repMsg);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Cannot issue reply with non-REPLY kind message"));
		}
	}

	testReceive()
	{
		gmsec.Log.info("Test receive()");

		// Nominal tests
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();

			this.setStandardFields(conn.getMessageFactory());

			// Timeout occurs (no subscription)
			var rcvd = conn.receive(10);
			this.check("Received unexpected message", rcvd == null);

			// Receive message
			var msg = conn.getMessageFactory().createMessage("HB");
			msg.setSubject(this.getSubject("HB"));

			conn.setupSubscription(msg.getSubject());
			conn.publish(msg);

			rcvd = conn.receive(5000);
			this.require("Did not receive message", rcvd != null);
			this.check("Unexpected subject", rcvd.getSubject() == msg.getSubject());
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal test(s)
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.receive(10);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Connection has not been initialized"));
		}
	}

	testExcludeSubject()
	{
		gmsec.Log.info("Test excludeSubject()");

		// Nominal test
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
	
			this.setStandardFields(conn.getMessageFactory());

			var msg1 = conn.getMessageFactory().createMessage("HB");
			var msg2 = conn.getMessageFactory().createMessage("LOG");

			msg1.setSubject(this.getSubject("HB"));
			msg2.setSubject(this.getSubject("LOG"));

			msg2.addField(new gmsec.StringField("SUBCLASS", "NO-CLASS"));
			msg2.addField(new gmsec.StringField("OCCURRENCE-TYPE", "FREQUENTLY"));
			msg2.addField(new gmsec.I16Field("SEVERITY", 1));

			// Setup scubscription to receive all messages except LOG ones
			conn.setupSubscription(this.getSubject(">"));
			conn.excludeSubject(this.getSubject("LOG.+"));

			conn.publish(msg1);
			var rcvd = conn.receive(5000);
			this.check("Expected to receive an HB message", rcvd != null);

			conn.publish(msg2);
			rcvd = conn.receive(5000);
			this.check("Expected to NOT receive a LOG message", rcvd == null);

			// Remove excluded subject
			conn.removeExcludedSubject(this.getSubject("LOG.+"));

			conn.publish(msg2);
			rcvd = conn.receive(5000);
			this.check("Expected to receive a LOG message", rcvd != null);
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal tests
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.excludeSubject("GMSEC.FOO.BAR");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Connection has not been initialized"));
		}

		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.excludeSubject("GMSEC FOO BAR");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Subject is invalid"));
		}
	}

	testRemoveExcludedSubject()
	{
		gmsec.Log.info("Test removeExcludedSubject()");

		// Nominal test for removeExcludedSubject() was tested in testExcludeSubject()

		// Off-nominal tests
		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.removeExcludedSubject("GMSEC.FOO.BAR");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Connection has not been initialized"));
		}

		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.removeExcludedSubject("GMSEC FOO BAR");
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Subject is invalid"));
		}
	}

	testGetSetName()
	{
		gmsec.Log.info("Test getName() and setName()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			this.check("Expected a connection name", conn.getName() != null);

			conn.setName("FOOBAR");
			this.check("Expected connection name of FOOBAR", conn.getName() === "FOOBAR");
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetID()
	{
		gmsec.Log.info("Test getID()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			this.check("Expected a connection ID", conn.getID() != null);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetMWInfo()
	{
		gmsec.Log.info("Test getMWInfo()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			this.check("Expected mwInfo to contain mw-id", conn.getMWInfo().includes(this.getConfig().getValue("mw-id", "")));
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetConnectionEndpoint()
	{
		gmsec.Log.info("Test getConnectionEndpoint()");

		try {
			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			this.check("Unexpected connectioon endpoint", conn.getConnectionEndpoint() != null);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}
}

test = new T010_Connection();
test.setDescription("Test Connection");
test.run(process.argv);
