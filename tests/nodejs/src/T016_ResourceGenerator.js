#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T016_ResourceGenerator extends TestCase
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
		this.testSetField();
		this.testCreateResourceMessage();
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
			var rsrcgen = new gmsec.ResourceGenerator(config1, 5, 1, 10);
			this.check("ResourceGenerator should not be running", rsrcgen.isRunning() == false);
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
			var rsrcgen = new gmsec.ResourceGenerator(config2, 5, 1, 10);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Bogus middleware
			var rsrcgen = new gmsec.ResourceGenerator(config3, 5, 1, 10);
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
			var rsrcgen1 = new gmsec.ResourceGenerator(config1, 5, 1, 10, emptyFieldArray);
			var rsrcgen2 = new gmsec.ResourceGenerator(config1, 5, 1, 10, standardFields);

			this.check("RSRC Gen 1 should not be running", rsrcgen1.isRunning() == false);
			this.check("RSRC Gen 2 should not be running", rsrcgen2.isRunning() == false);
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
			var rsrcgen = new gmsec.ResourceGenerator(config2, 5, 1, 10, emptyFieldArray);
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}

		try {
			// Bogus middleware
			var rsrcgen = new gmsec.ResourceGenerator(config3, 5, 1, 10, emptyFieldArray);
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
			var pubRate = 1;
			var rsrcgen = new gmsec.ResourceGenerator(this.getConfig(), pubRate, 1, 10, this.getStandardFields());

			// To ensure the RSRC generator encapsulated message is unique, we set the COMPONENT field
			rsrcgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			// Start the RSRC generator
			rsrcgen.start();
			this.check("RSRC Gen should be running", rsrcgen.isRunning());

			this.verifyResourceMessage(this.getConfig(), pubRate);

			// Stop the RSRC generator
			rsrcgen.stop();
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testStop()
	{
		gmsec.Log.info("Test stop()");

		try {
			var pubRate = 1;
			var rsrcgen = new gmsec.ResourceGenerator(this.getConfig(), pubRate, 1, 10, this.getStandardFields());

			// To ensure the RSRC generator encapsulated message is unique, we set the COMPONENT field
			rsrcgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			// Start the RSRC generator
			rsrcgen.start();
			this.check("RSRC Gen should be running", rsrcgen.isRunning());

			// Delay before stopping the RSRC generator
			gmsec.TimeUtil.millisleep(2000);

			// Stop the RSRC generator
			rsrcgen.stop();
			this.check("RSRC Gen should be running", rsrcgen.isRunning() == false);

			// Allow time for the RSRC (thread) to truly stop
			gmsec.TimeUtil.millisleep(2000);

			// Verify no RSRC messages are being published
			try {
				var conn = gmsec.Connection.create(this.getConfig());
				conn.connect();
				conn.setupSubscription("C2MS.*.*.*.*.*.MSG.RSRC." + this.getUniqueComponent() + ".+");

				for (var i = 0; i < 3; i++) {
					var hbmsg = conn.receive(5000);

					if (hbmsg != null) {
						this.check("Received unexpected RSRC message", false);
					}
				}

				conn.disconnect();

				gmsec.Connection.destroy(conn);
			}
			catch (e) {
				this.require(e.message, false);
			}
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
			var pubRate = 1;
			var rsrcgen = new gmsec.ResourceGenerator(this.getConfig(), pubRate, 1, 10, this.getStandardFields());

			// To ensure the RSRC generator encapsulated message is unique, we set the COMPONENT field
			rsrcgen.setField(new gmsec.StringField("COMPONENT", this.getUniqueComponent()));

			rsrcgen.start();

			gmsec.TimeUtil.millisleep(2000);

			gmsec.Log.info("Test setField() with a Field object");
			var newPubRate = 2;
			rsrcgen.setField(new gmsec.U16Field("PUB-RATE", newPubRate));
			this.verifyResourceMessage(this.getConfig(), newPubRate);

			rsrcgen.stop();
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
			var rsrcgen = new gmsec.ResourceGenerator(config, 1, 1, 10, this.getStandardFields());
			rsrcgen.setField(new gmsec.U16Field("BOGUS-FIELD", 2));
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message Validation Failed"));
		}
		*/
	}

	testCreateResourceMessage()
	{
		gmsec.Log.info("Test createResourceMessage()");

		var factory = new gmsec.MessageFactory(this.getConfig());
		var samInt  = 1;
		var avgInt  = 10;

		this.setStandardFields(factory);

		var rsrcMsg = gmsec.ResourceGenerator.createResourceMessage(factory, samInt, avgInt);
		this.check("Unexpected MESSAGE-TYPE", rsrcMsg.getStringValue("MESSAGE-TYPE") === "MSG");
		this.check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.getStringValue("MESSAGE-SUBTYPE") === "RSRC");
	}

	verifyResourceMessage(config, pubRate)
	{
		try {
			var conn = gmsec.Connection.create(config);
			conn.connect();
			conn.setupSubscription("C2MS.*.*.*.*.*.MSG.RSRC." + this.getUniqueComponent() + ".+");

			var t1 = 0;
			var t2 = 0;

			for (var i = 0; i < 8; i++) {
				// Start time t1 is at the time start to receive data
				t1 = gmsec.TimeUtil.getCurrentTime_s();
				var hbmsg = conn.receive(5000);

				// The end time t2 has to be measured immediately right after it completes receiving data.
				t2 = gmsec.TimeUtil.getCurrentTime_s();

				// Ignore the first few incoming messages (if any)
				if (i < 3) continue;

				if (hbmsg != null) {
					//if (t1 == 0) {
					//	t1 = gmsec.TimeUtil.getCurrentTime_s();
					//}
					//else {
					//	t2 = gmsec.TimeUtil.getCurrentTime_s();

						var delta = t2 - t1;
						roundUpPubRateToSecond = Math.floor(delta);
						if (delta < pubRate) {
							//delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
							roundUpPubRateToSecond = Math.floor(delta + 0.5);    // round up to second
						}

						gmsec.Log.info("Expected Rate: " + pubRate + ", delta is: " + delta);

						//this.check("Unexpected publish rate", Math.floor(delta) == pubRate);
						this.check("Unexpected publish rate", roundUpPubRateToSecond == pubRate);

					//	t1 = t2;
					//}
				}
				else {
					this.check("Failed to receive RSRC message", false);
				}
			}

			conn.disconnect();

			gmsec.Connection.destroy(conn);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}
}

test = new T016_ResourceGenerator();
test.setDescription("Test ResourceGenerator");
test.run(process.argv);
