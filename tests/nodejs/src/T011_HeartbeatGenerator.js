#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T011_HeartbeatGenerator extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor_1();
		this.testConstructor_2();
		this.testStart();
		this.testStop();
		this.testChangePublishRate();
		this.testSetField();
	}

	testConstructor_1()
	{
		gmsec.Log.info("Test constructor w/ no fields");

		var config1 = new gmsec.Config(this.getConfig());
		var config2 = new gmsec.Config(this.getConfig());
		var config3 = new gmsec.Config("mw-id=bogus", gmsec.DataType_KEY_VALUE_DATA);

		config2.addValue("gmsec-msg-content-validate", "true");

		// Nominal test
		try {
			var hbgen = new gmsec.HeartbeatGenerator(config1, 5);
			this.check("HeartbeatGenerator should not be running", hbgen.isRunning() == false);
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal tests
		// TODO: Not able to test because SWIG-generated code does not propagate
		// GMSEC API exceptions on overloaded functions.
		/*
		try {
			// Missing standard fields to make RSRC message compliant
			var hbgen = new gmsec.HeartbeatGenerator(config2, 5);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Bogus middleware
			var hbgen = new gmsec.HeartbeatGenerator(config3, 5);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Unable to load"));
		}
		*/
	}

	testConstructor_2()
	{
		gmsec.Log.info("Test constructor w/ fields");

		var config1 = new gmsec.Config(this.getConfig());
		var config2 = new gmsec.Config(this.getConfig());
		var config3 = new gmsec.Config("mw-id=bogus", gmsec.DataType_KEY_VALUE_DATA);

		config2.addValue("gmsec-msg-content-validate", "true");

		var emptyFieldArray = new gmsec.FieldArray();
		var standardFields  = this.getStandardFields();

		try {
			var hbgen1 = new gmsec.HeartbeatGenerator(config1, 5, emptyFieldArray);
			var hbgen2 = new gmsec.HeartbeatGenerator(config1, 5, standardFields);

			this.check("HB Gen 1 should not be running", hbgen1.isRunning() == false);
			this.check("HB Gen 2 should not be running", hbgen2.isRunning() == false);
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal tests
		// TODO: Not able to test because SWIG-generated code does not propagate
		// GMSEC API exceptions on overloaded functions.
		/*
		try {
			// Missing standard fields to make RSRC message compliant
			var hbgen = new gmsec.HeartbeatGenerator(config2, 5, emptyFieldArray);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Bogus middleware
			var hbgen = new gmsec.HeartbeatGenerator(config3, 5, emptyFieldArray);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Unable to load"));
		}
		*/
	}

	testStart()
	{
		gmsec.Log.info("Test start()");

		try {
			var hbgen = new gmsec.HeartbeatGenerator(this.getConfig(), 1, this.getStandardFields());

			// To ensure the HB generator encapsulated message is unique, we set the COMPONENT field
			hbgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			// Start the HB generator
			hbgen.start();
			this.check("HB Gen should be running", hbgen.isRunning());

			this.verifyHeartbeatMessage(this.getConfig(), 1);

			// Stop the HB generator
			hbgen.stop();
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testStop()
	{
		gmsec.Log.info("Test stop()");

		try {
			var hbgen = new gmsec.HeartbeatGenerator(this.getConfig(), 1, this.getStandardFields());

			// To ensure the HB generator encapsulated message is unique, we set the COMPONENT field
			hbgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			// Start the HB generator
			hbgen.start();
			this.check("HB Gen should be running", hbgen.isRunning());

			// Delay before stopping the HB generator
			gmsec.TimeUtil.millisleep(2000);

			// Stop the HB generator
			hbgen.stop();
			this.check("HB Gen should be running", hbgen.isRunning() == false);

			// Allow time for the HB to truly stop
			gmsec.TimeUtil.millisleep(2000);

			// Verify no HB messages are being published
			try {
				var conn = new gmsec.Connection(this.getConfig());
				conn.connect();
				conn.setupSubscription("C2MS.*.*.*.*.*.MSG.HB." + this.getUniqueComponent() + ".+");

				for (var i = 0; i < 3; i++) {
					var hbmsg = conn.receive(5000);

					if (hbmsg != null) {
						this.check("Received unexpected HB message", false);
					}
				}

				conn.disconnect();
			}
			catch (e) {
				this.require(e.message, false);
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testChangePublishRate()
	{
		gmsec.Log.info("Test changePublishRate()");

		try {
			var hbgen = new gmsec.HeartbeatGenerator(this.getConfig(), 1, this.getStandardFields());

			// To ensure the HB generator encapsulated message is unique, we set the COMPONENT field
			hbgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			// To ensure the HB generator encapsulated message is unique, we set the COMPONENT field
			hbgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			hbgen.start();
			hbgen.changePublishRate(2);

			gmsec.TimeUtil.millisleep(2000);

			this.verifyHeartbeatMessage(this.getConfig(), 2);

			var conn = new gmsec.Connection(this.getConfig());
			conn.connect();
			conn.setupSubscription("C2MS.*.*.*.*.*.MSG.HB." + this.getUniqueComponent() + ".+");

			hbgen.changePublishRate(0);

			var lastMessage = false;
			for (var i = 0; i < 3; i++) {
				var hbmsg = conn.receive(5000);
				if (hbmsg == null) {
					this.check("Failed to receive HB message", false);
				}
				else if (hbmsg.getIntegerValue("PUB-RATE") != 0) {
					// ignore message
				}
				else {
					lastMessage = true;
					break;
				}
			}

			this.check("Failed to receive HB message with PUB-RATE of 0", lastMessage);

			conn.disconnect();

			hbgen.stop();
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetField()
	{
		gmsec.Log.info("Test setField()");

		// Nominal tests
		try {
			var hbgen = new gmsec.HeartbeatGenerator(this.getConfig(), 1, this.getStandardFields());

			// To ensure the HB generator encapsulated message is unique, we set the COMPONENT field
			hbgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			hbgen.start();

			gmsec.TimeUtil.millisleep(2000);

			gmsec.Log.info("Test setField() with a Field object");
			hbgen.setField(new gmsec.U16Field("PUB-RATE", 2));
			this.verifyHeartbeatMessage(this.getConfig(), 2);

			gmsec.Log.info("Test setField() with a Field name and long value");
			hbgen.setField("PUB-RATE", 3);
			this.verifyHeartbeatMessage(this.getConfig(), 3);

			gmsec.Log.info("Test setField() with a Field name and a double value");
			hbgen.setField("PUB-RATE", 2.0);
			this.verifyHeartbeatMessage(this.getConfig(), 2);

			gmsec.Log.info("Test setField() with a Field name and a string value");
			hbgen.setField("PUB-RATE", "3");
			this.verifyHeartbeatMessage(this.getConfig(), 3);

			hbgen.stop();
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal tests
		// TODO: Not able to test because SWIG-generated code does not propagate
		// GMSEC API exceptions on overloaded functions.
		/*
		var config = new gmsec.Config(this.getConfig());
		config.addValue("gmsec-msg-content-validate", "true");

		try {
			// Add bogus field using a Field object
			var hbgen = new gmsec.HeartbeatGenerator(config, 1, this.getStandardFields());
			hbgen.setField(new gmsec.U16Field("BOGUS-FIELD", 2));
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Add bogus field using a long value
			var hbgen = new gmsec.HeartbeatGenerator(config, 1, this.getStandardFields());
			hbgen.setField("BOGUS-FIELD", 2);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Add bogus field using a double value
			var hbgen = new gmsec.HeartbeatGenerator(config, 1, this.getStandardFields());
			hbgen.setField("BOGUS-FIELD", 2.0);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Add bogus field using a string value
			var hbgen = new gmsec.HeartbeatGenerator(config, 1, this.getStandardFields());
			hbgen.setField("BOGUS-FIELD", "2");
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}
		*/
	}

	verifyHeartbeatMessage(config, pubRate)
	{
		try {
			var conn = new gmsec.Connection(config);
			conn.connect();
			conn.setupSubscription("C2MS.*.*.*.*.*.MSG.HB." + this.getUniqueComponent() + ".+");

			var t1 = 0;
			var t2 = 0;

			for (var i = 0; i < 8; i++) {
				var hbmsg = conn.receive(5000);

				// Ignore the first few incoming messages (if any)
				if (i < 3) continue;

				if (hbmsg != null) {
					if (t1 == 0) {
						t1 = gmsec.TimeUtil.getCurrentTime_s();
					}
					else {
						t2 = gmsec.TimeUtil.getCurrentTime_s();

						var delta = t2 - t1;
						if (delta < pubRate) {
							delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
						}

						gmsec.Log.info("Expected Rate: " + pubRate + ", delta is: " + delta);

						this.check("Unexpected publish rate", Math.floor(delta) == pubRate);

						t1 = t2;
					}
				}
				else {
					this.check("Failed to receive HB message", false);
				}
			}

			conn.disconnect();
		}
		catch (e) {
			this.require(e.message, false);
		}
	}
}

test = new T011_HeartbeatGenerator();
test.setDescription("Test HeartbeatGenerator");
test.run(process.argv);
