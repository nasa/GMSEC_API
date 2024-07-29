#!/usr/bin/env node

const gmsec    = require('../../../bin/gmsec_nodejs.node');
const TestCase = require('./TestCase.js');


class T005_Message extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor();
		this.testCopyConstructor();
		this.testGetSchemaID();
		this.testGetVersion();
		this.testGetSchemaLevel();
		this.testIsCompliant();
		this.testSetFieldValue();
		this.testSetConfig();
		this.testSetSubject();
		this.testSetSubjectElement();
		this.testGetSubject();
		this.testSetKind();
		this.testGetKind();
		this.testAddField();
		this.testAddFields();
		this.testClearFields();
		this.testClearField();
		this.testGetStringValue();
		this.testGetBooleanValue();
		this.testGetIntegerValue();
		this.testGetDoubleValue();
		this.testHasField();
		this.testGetField();
		this.testGetFieldType();
		this.testGetBinaryField();
		this.testGetBooleanField();
		this.testGetCharField();
		this.testGetF32Field();
		this.testGetF64Field();
		this.testGetI8Field();
		this.testGetI16Field();
		this.testGetI32Field();
		this.testGetI64Field();
		this.testGetU8Field();
		this.testGetU16Field();
		this.testGetU32Field();
		this.testGetU64Field();
		this.testGetStringField();
		this.testGetFieldCount();
		this.testCopyFields();
		this.testToXML();
		this.testToJSON();
		this.testGetSize();
		this.testGetFieldIterator();
	}

	testConstructor()
	{
		gmsec.Log.info("Test constructor");

		var msg = new gmsec.Message();

		this.check("Message has unexpected schema ID", msg.getSchemaID() == "");
		this.check("Message has unexpected field count", msg.getFieldCount() == 0);
		this.check("Message has unexpected kind", msg.getKind() == gmsec.Message.Kind_PUBLISH);
		this.check("Message has null subject", msg.getSubject() != null);
		this.check("Message as XML is null", msg.toXML() != null);
		this.check("Message as JSON is null", msg.toJSON() != null);
	}

	assertMessage(msg)
	{
		this.check("Message is missing PUB-RATE", msg.hasField("PUB-RATE"));
		this.check("PUB-RATE has unexpected value", msg.getField("PUB-RATE").getI32Value() == 30);
		this.check("PUB-RATE has unexpected value", msg.getField("PUB-RATE").getType() == gmsec.Field.Type_U16);

		this.check("Message has unexpected schema ID", msg.getSchemaID() === "MSG.HB");

		this.check("Message is missing DOMAIN1", msg.hasField("DOMAIN1"));
		this.check("Message is missing DOMAIN2", msg.hasField("DOMAIN2"));

		this.check("DOMAIN1 has unexpected value", msg.getField("DOMAIN1").getStringValue() === "MY-DOMAIN-1");
		this.check("DOMAIN2 has unexpected value", msg.getField("DOMAIN2").getStringValue() === "MY-DOMAIN-2");
	}

	testCopyConstructor()
	{
		gmsec.Log.info("Test copy constructor");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg1 = msgFactory.createMessage("HB");
			msg1.setFieldValue("PUB-RATE", 30);

			// Invoke copy constructor
			var msg2 = new gmsec.Message(msg1);

			this.assertMessage(msg2);
			this.check("Unexpected schema ID", msg1.getSchemaID() === msg2.getSchemaID());
			this.check("Unexpected version", msg1.getVersion() === msg2.getVersion());
			this.check("Unexpected schema level", msg1.getSchemaLevel() === msg2.getSchemaLevel());
			this.check("Unexpected XML content", msg1.toXML() === msg2.toXML());
			this.check("Unexpected JSON content", msg1.toJSON() === msg2.toJSON());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetSchemaID()
	{
		gmsec.Log.info("Test getSchemaID()");

		try {
			var msgFactory = new gmsec.MessageFactory();

			var msg1 = msgFactory.createMessage("HB");
			var msg2 = msgFactory.createMessage("MVAL");
			var msg3 = msgFactory.createMessage("TLMFRAME");

			this.check("Message 1 has unexpected schema ID", msg1.getSchemaID() === "MSG.HB");
			this.check("Message 2 has unexpected schema ID", msg2.getSchemaID() === "MSG.MVAL");
			this.check("Message 3 has unexpected schema ID", msg3.getSchemaID() === "MSG.TLMFRAME");
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetVersion()
	{
		gmsec.Log.info("Test getVersion()");

		try {
			var config = new gmsec.Config("gmsec-specification-version=201900", gmsec.DataType_KEY_VALUE_DATA);

			var msgFactory1 = new gmsec.MessageFactory();
			var msgFactory2 = new gmsec.MessageFactory(config);

			var msg1 = msgFactory1.createMessage("HB");
			var msg2 = msgFactory2.createMessage("HB");

			this.check("Message 1 has unexpected version", msg1.getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
			this.check("Message 2 has unexpected version", msg2.getVersion() == gmsec.GMSEC_MSG_SPEC_2019_00);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetSchemaLevel()
	{
		gmsec.Log.info("Test getSchemaLevel()");

		try {
			var config = new gmsec.Config("gmsec-schema-level=2", gmsec.DataType_KEY_VALUE_DATA);

			var msgFactory1 = new gmsec.MessageFactory();
			var msgFactory2 = new gmsec.MessageFactory(config);

			var msg1 = msgFactory1.createMessage("HB");
			var msg2 = msgFactory2.createMessage("HB");

			this.check("Message 1 has unexpected schema level", msg1.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_0);
			this.check("Message 2 has unexpected schema level", msg2.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_2);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testIsCompliant()
	{
		gmsec.Log.info("Test isCompliant()");

		try {
			var config = new gmsec.Config(this.getConfig());   // make a copy
			config.addValue("gmsec-msg-content-validate", "true");

			var msgFactory1 = new gmsec.MessageFactory();
			var msgFactory2 = new gmsec.MessageFactory(config);

			this.setStandardFields(msgFactory1);

			var msg1 = msgFactory1.createMessage("HB");
			var msg2 = msgFactory2.createMessage("HB");
			var msg3 = new gmsec.Message();
			msg3.setSubject("GMSEC.FOO.BAR");

			msg2.addField(new gmsec.I32Field("DOMAIN1", 1));   // illegal value

			var status1 = msg1.isCompliant();
			var status2 = msg2.isCompliant();
			var status3 = msg3.isCompliant();

			this.check("Message 1 should be compliant", status1.hasError() == false);

			this.check("Message 2 should not be compliant", status2.hasError());
			this.check("Message 2 error has unexpected reason", status2.getReason().includes("Message Validation Failed"));

			this.check("Message 3 should not be compliant", status3.hasError());
			this.check("Message 3 error has unexpected reason", status3.getReason().includes("No template available; unable to validate message [GMSEC.FOO.BAR]"));
		}
		catch (e) {
			test.require(e.message, false);
		}
	}

	testSetFieldValue()
	{
		gmsec.Log.info("Test setFieldValue()");

		try {
			var msgFactory = new gmsec.MessageFactory();

			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("HB");

			//o Test with string value
			msg.setFieldValue("PUB-RATE", "5");
			this.check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == gmsec.Field.Type_U16);
			this.check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 5);

			//o Test with int value
			msg.setFieldValue("PUB-RATE", 10);
			this.check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == gmsec.Field.Type_U16);
			this.check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 10);

			//o Test with double value
			msg.setFieldValue("PUB-RATE", 40.123);
			this.check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == gmsec.Field.Type_U16);
			this.check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 40);

			//o Verify result returned is 'true' when replacing a field
			this.check("Expected return value of true", msg.setFieldValue("PUB-RATE", 10) == true);

			//o Verify result returned is 'false' when adding a new field
			this.check("Expected return value of false", msg.setFieldValue("PUBLISH-RATE", 10) == false);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetConfig()
	{
		gmsec.Log.info("Test setConfig()");

		try {
			var config = new gmsec.Config("tracking=false", gmsec.DataType_KEY_VALUE_DATA);

			var msgFactory = new gmsec.MessageFactory();

			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("TLMPROC");

			msg.setConfig(config);

			this.check("Message has unexpected Config content", msg.getConfig().toXML() === config.toXML());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetSubject()
	{
		gmsec.Log.info("Test setSubject()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("TLMPROC");
			msg.setSubject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

			this.check("Message has unexpected subject", msg.getSubject() === "GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");
		}
		catch (e) {
			this.require(e.message, false);
		}

		//o Off-nominal tests

		try {
			//o zero-length subject
			var tmp = new gmsec.Message();
			tmp.setSubject("");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Subject cannot be NULL, nor an empty string"));
		}

		try {
			//o invalid subject content
			var tmp = new gmsec.Message();
			tmp.setSubject("Subject.Cannot.Contain.Lowercase.Letters");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Invalid message subject"));
		}

		try {
			//o invalid subject content
			var tmp = new gmsec.Message();
			tmp.setSubject("HELLO%WORLD");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Invalid message subject"));
		}
	}

	testSetSubjectElement()
	{
		gmsec.Log.info("Test setSubjectElement()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("TLMPROC");
			msg.setSubjectElement("ME4", "FOOEY");

			this.check("Message has unexpected subject", msg.getSubject() === "C2MS.MY-DOMAIN-1.MY-DOMAIN-2.MY-MISSION.MY-CNST-ID.MY-SAT-ID.MSG.TLMPROC.JS-TESTCASE.FILL.FILL.FOOEY");
			try {
				msg.setSubjectElement("ME9000", "FOOEY");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not have a subject element named ME9000"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}

		//o Off-nominal tests
		
		try {
			var tmp = new gmsec.Message();
			tmp.setSubjectElement("", "FOOEY");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Subject element name cannot be NULL or empty string"));
		}
		
		try {
			var tmp = new gmsec.Message();
			tmp.setSubjectElement("ME4", "FOOEY");
			this.check("An exception was expected", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Message does not have a message template"));
		}
	}

	testGetSubject()
	{
		gmsec.Log.info("Test getSubject()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("TLMPROC");
			msg.addField(new gmsec.StringField("STREAM-MODE", "RT"));   // add ME2 (stream mode)

			var subject = msg.getSubject();
			subject += ".2";   // add ME2 (virtual channel ID)
			subject += ".1";   // add ME3 (APP ID)

			msg.setSubject(subject);

			this.check("Message has unexpected subject", msg.getSubject() === subject);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testSetKind()
	{
		gmsec.Log.info("Test setKind()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("HB");
			this.check("Unexpected message kind", msg.getKind() == gmsec.Message.Kind_PUBLISH);

			//o Set to REQUEST-kind, and verify
			msg.setKind(gmsec.Message.Kind_REQUEST);
			this.check("Unexpected message kind", msg.getKind() == gmsec.Message.Kind_REQUEST);

			//o Set to REPLY-kind, and verify
			msg.setKind(gmsec.Message.Kind_REPLY);
			this.check("Unexpected message kind", msg.getKind() == gmsec.Message.Kind_REPLY);

			//o Set to PUBLISH-kind, and verify
			msg.setKind(gmsec.Message.Kind_PUBLISH);
			this.check("Unexpected message kind", msg.getKind() == gmsec.Message.Kind_PUBLISH);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetKind()
	{
		gmsec.Log.info("Test getKind()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg1 = msgFactory.createMessage("HB");
			var msg2 = msgFactory.createMessage("REQ.DIR");
			var msg3 = msgFactory.createMessage("RESP.DIR");
			var msg4 = msgFactory.createMessage();
			var msg5 = new gmsec.Message();

			this.check("Message 1 has unexpected kind", msg1.getKind() == gmsec.Message.Kind_PUBLISH);
			this.check("Message 2 has unexpected kind", msg2.getKind() == gmsec.Message.Kind_REQUEST);
			this.check("Message 3 has unexpected kind", msg3.getKind() == gmsec.Message.Kind_REPLY);
			this.check("Message 4 has unexpected kind", msg4.getKind() == gmsec.Message.Kind_PUBLISH);
			this.check("Message 5 has unexpected kind", msg5.getKind() == gmsec.Message.Kind_PUBLISH);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testAddField()
	{
		gmsec.Log.info("Test addField()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("LOG");
			var field = new gmsec.StringField("SPECIFICATION", "C2MS");

			//o replace existing field
			this.check("Expected field to be replaced", msg.addField(field) == true);

			var blob = new gmsec.BlobArray();
			for (let i = 0; i < 256; ++i) {
				blob.add(i);
			}

			//o test other overloaded methods
			this.check("Expected new field to be added", msg.addField(new gmsec.BinaryField("BINARY", blob)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.BooleanField("BOOLEAN", true)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.CharField("CHAR", 'c')) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.F32Field("FLOAT", 1.2)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.F64Field("DOUBLE", 2.4)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.I8Field("BYTE", 5)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.I16Field("SHORT", 55)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.I32Field("INT", 555)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.I64Field("LONG", 5555)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.StringField("STRING", "The old fox")) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.U8Field("UBYTE", 5)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.U16Field("USHORT", 55)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.U32Field("UINT", 555)) == false);
			this.check("Expected new field to be added", msg.addField(new gmsec.U64Field("ULONG", 5555)) == false);

			// validate field settings
			this.check("SPECIFICATION not found", msg.hasField("SPECIFICATION"));
			this.check("SPECIFICATION has wrong type", msg.getField("SPECIFICATION").getType() == gmsec.Field.Type_STRING);
			this.check("SPECIFICATION has wrong value", msg.getStringValue("SPECIFICATION") == "C2MS");

			this.check("BINARY not found", msg.hasField("BINARY"));
			this.check("BINARY has wrong type", msg.getField("BINARY").getType() == gmsec.Field.Type_BINARY);
			this.check("BINARY has wrong length", msg.getBinaryField("BINARY").getLength() == blob.size());

			this.check("BOOLEAN not found", msg.hasField("BOOLEAN"));
			this.check("BOOLEAN has wrong type", msg.getField("BOOLEAN").getType() == gmsec.Field.Type_BOOL);
			this.check("BOOLEAN has wrong value", msg.getBooleanField("BOOLEAN").getValue() == true);

			this.check("CHAR not found", msg.hasField("CHAR"));
			this.check("CHAR has wrong type", msg.getField("CHAR").getType() == gmsec.Field.Type_CHAR);
			this.check("CHAR has wrong value", msg.getCharField("CHAR").getValue() == 'c');

			this.check("FLOAT not found", msg.hasField("FLOAT"));
			this.check("FLOAT has wrong type", msg.getField("FLOAT").getType() == gmsec.Field.Type_F32);
			this.check("FLOAT has wrong value", parseFloat(msg.getF32Field("FLOAT").getValue()).toPrecision(2) == 1.2);

			this.check("DOUBLE not found", msg.hasField("DOUBLE"));
			this.check("DOUBLE has wrong type", msg.getField("DOUBLE").getType() == gmsec.Field.Type_F64);
			this.check("DOUBLE has wrong value", msg.getF64Field("DOUBLE").getValue() == 2.4);

			this.check("BYTE not found", msg.hasField("BYTE"));
			this.check("BYTE has wrong type", msg.getField("BYTE").getType() == gmsec.Field.Type_I8);
			this.check("BYTE has wrong value", msg.getI8Field("BYTE").getValue() == 5);

			this.check("SHORT not found", msg.hasField("SHORT"));
			this.check("SHORT has wrong type", msg.getField("SHORT").getType() == gmsec.Field.Type_I16);
			this.check("SHORT has wrong value", msg.getI16Field("SHORT").getValue() == 55);

			this.check("INT not found", msg.hasField("INT"));
			this.check("INT has wrong type", msg.getField("INT").getType() == gmsec.Field.Type_I32);
			this.check("INT has wrong value", msg.getI32Field("INT").getValue() == 555);

			this.check("LONG not found", msg.hasField("LONG"));
			this.check("LONG has wrong type", msg.getField("LONG").getType() == gmsec.Field.Type_I64);
			this.check("LONG has wrong value", msg.getI64Field("LONG").getValue() == 5555);

			this.check("STRING not found", msg.hasField("STRING"));
			this.check("STRING has wrong type", msg.getField("STRING").getType() == gmsec.Field.Type_STRING);
			this.check("STRING has wrong value", msg.getStringField("STRING").getValue() == "The old fox");

			this.check("UBYTE not found", msg.hasField("UBYTE"));
			this.check("UBYTE has wrong type", msg.getField("UBYTE").getType() == gmsec.Field.Type_U8);
			this.check("UBYTE has wrong value", msg.getU8Field("UBYTE").getValue() == 5);

			this.check("USHORT not found", msg.hasField("USHORT"));
			this.check("USHORT has wrong type", msg.getField("USHORT").getType() == gmsec.Field.Type_U16);
			this.check("USHORT has wrong value", msg.getU16Field("USHORT").getValue() == 55);

			this.check("UINT not found", msg.hasField("UINT"));
			this.check("UINT has wrong type", msg.getField("UINT").getType() == gmsec.Field.Type_U32);
			this.check("UINT has wrong value", msg.getU32Field("UINT").getValue() == 555);

			this.check("ULONG not found", msg.hasField("ULONG"));
			this.check("ULONG has wrong type", msg.getField("ULONG").getType() == gmsec.Field.Type_U64);
			this.check("ULONG has wrong value", msg.getU64Field("ULONG").getValue() == 5555);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testAddFields()
	{
		gmsec.Log.info("Test addFields()");

		var field1 = new gmsec.StringField("ONE", "uno");
		var field2 = new gmsec.StringField("TWO", "dos");
		var field3 = new gmsec.StringField("THREE", "tres");

		var fields = new gmsec.FieldArray();
		fields.add(field1);
		fields.add(field2);
		fields.add(field3);

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("LOG");

			//o add list of fields; expect all fields to be new
			this.check("Expected only new fields to be added", msg.addFields(fields) == false);

			//o verify
			this.check("Expected to have field ONE", msg.hasField("ONE"));
			this.check("Unexpected field type for ONE", msg.getField("ONE").getType() == gmsec.Field.Type_STRING);
			this.check("Unexpected field value for ONE", msg.getStringValue("ONE") === "uno");

			this.check("Expected to have field TWO", msg.hasField("TWO"));
			this.check("Unexpected field type for TWO", msg.getField("TWO").getType() == gmsec.Field.Type_STRING);
			this.check("Unexpected field value for TWO", msg.getStringValue("TWO") === "dos");

			this.check("Expected to have field THREE", msg.hasField("THREE"));
			this.check("Unexpected field type for THREE", msg.getField("THREE").getType() == gmsec.Field.Type_STRING);
			this.check("Unexpected field value for THREE", msg.getStringValue("THREE") === "tres");

			//o add (once again) list of fields; expect all fields to be replaced
			this.check("Expected only new fields to be added", msg.addFields(fields));
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testClearFields()
	{
		gmsec.Log.info("Test clearFields()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			this.check("Message missing CONTENT-VERSION", msg.hasField("CONTENT-VERSION"));
			this.check("Message missing HEADER-VERSION", msg.hasField("HEADER-VERSION"));
			this.check("Message missing MESSAGE-SUBTYPE", msg.hasField("MESSAGE-SUBTYPE"));
			this.check("Message missing MESSAGE-TYPE", msg.hasField("MESSAGE-TYPE"));
			this.check("Message missing SPECIFICATION", msg.hasField("SPECIFICATION"));

			msg.clearFields();

			this.check("Message has CONTENT-VERSION", msg.hasField("CONTENT-VERSION") == false);
			this.check("Message has HEADER-VERSION", msg.hasField("HEADER-VERSION") == false);
			this.check("Message has MESSAGE-SUBTYPE", msg.hasField("MESSAGE-SUBTYPE") == false);
			this.check("Message has MESSAGE-TYPE", msg.hasField("MESSAGE-TYPE") == false);
			this.check("Message has SPECIFICATION", msg.hasField("SPECIFICATION") == false);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testClearField()
	{
		gmsec.Log.info("Test clearField()");

		try {
			var msgFactory = new gmsec.MessageFactory();

			var msg = msgFactory.createMessage("HB");

			this.check("CONTENT-VERSION not found in message", msg.hasField("CONTENT-VERSION"));

			this.check("CONTENT-VERSION not cleared from message", msg.clearField("CONTENT-VERSION") == true);
			this.check("FOO-BAR does not exist and cannot be cleared", msg.clearField("FOO-BAR") == false);
			this.check("foo-bar does not exist and cannot be cleared", msg.clearField("foo-bar") == false);

			this.check("CONTENT-VERSION should not exist in message", msg.hasField("CONTENT-VERSION") == false);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetStringValue()
	{
		gmsec.Log.info("Test getStringValue()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			msg.setFieldValue("PUB-RATE", "30");

			this.check("MESSAGE-TYPE has unexpected value", msg.getStringValue("MESSAGE-TYPE") === "MSG");
			this.check("PUB-RATE has unexpected value", msg.getStringValue("PUB-RATE") === "30");
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetBooleanValue()
	{
		gmsec.Log.info("Test getBooleanValue()");

		try {
			var msg = new gmsec.Message();

			msg.addField( new gmsec.I16Field("FIELD-1", 0) );
			msg.addField( new gmsec.I16Field("FIELD-2", 1) );
			msg.addField( new gmsec.StringField("FIELD-3", "10000") );
			msg.addField( new gmsec.I32Field("FIELD-4", 10000) );
			msg.addField( new gmsec.BooleanField("FIELD-5", true) );
			msg.addField( new gmsec.BooleanField("FIELD-6", false) );
			msg.addField( new gmsec.F64Field("FIELD-7", 2019.0) );
			msg.addField( new gmsec.StringField("FIELD-8", "true") );
			msg.addField( new gmsec.StringField("FIELD-9", "FALSE") );
			msg.addField( new gmsec.StringField("FIELD-0", "foo bar") );

			//o Nominal tests
			this.check("1 Expected boolean value of false", msg.getBooleanValue("FIELD-1") == false);
			this.check("2 Expected boolean value of true",  msg.getBooleanValue("FIELD-2"));
			this.check("3 Expected boolean value of true",  msg.getBooleanValue("FIELD-3"));
			this.check("4 Expected boolean value of true",  msg.getBooleanValue("FIELD-4"));
			this.check("5 Expected boolean value of true",  msg.getBooleanValue("FIELD-5"));
			this.check("6 Expected boolean value of false", msg.getBooleanValue("FIELD-6") == false);
			this.check("7 Expected boolean value of true",  msg.getBooleanValue("FIELD-7"));
			this.check("8 Expected boolean value of true",  msg.getBooleanValue("FIELD-8"));
			this.check("9 Expected boolean value of false", msg.getBooleanValue("FIELD-9") == false);

			//o Off-nominal tests
			//o Field cannot be converted
			try {
				msg.getBooleanValue("FIELD-0");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field cannot be represented as a boolean"));
			}

			//o Unknown field
			try {
				msg.getBooleanValue("BOGUS");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain field with name: BOGUS"));
			}

			//o Empty-string field name
			try {
				msg.getBooleanValue("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetIntegerValue()
	{
		gmsec.Log.info("Test getIntegerValue()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			msg.setFieldValue("PUB-RATE", "30");

			this.check("PUB-RATE has unexpected value", msg.getIntegerValue("PUB-RATE") == 30);

			// Off-nominal test
			try {
				msg.getIntegerValue("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field cannot be converted to a GMSEC_I64"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetDoubleValue()
	{
		gmsec.Log.info("Test getDoubleValue()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			msg.setFieldValue("PUB-RATE", "30");

			this.check("PUB-RATE has unexpected value", msg.getDoubleValue("PUB-RATE") == 30);

			// Off-nominal test
			try {
				msg.getDoubleValue("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field cannot be converted to a GMSEC_F64"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testHasField()
	{
		gmsec.Log.info("Test hasField()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			msg.setFieldValue("PUB-RATE", "30");

			this.check("Message should have PUB-RATE", msg.hasField("PUB-RATE"));
			this.check("Message should not have PUBLISH-RATE", msg.hasField("PUBLISH-RATE") == false);
			this.check("Message should not have empty string field", msg.hasField("") == false);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetField()
	{
		gmsec.Log.info("Test getField()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			//o Use valid field name
			this.check("Message should have MESSAGE-TYPE", msg.getField("MESSAGE-TYPE") != null);

			// Use bogus field name
			this.check("Message should have MSG-TYPE", msg.getField("MSG-TYPE") == null);

			// Use empty-string field name
			try {
				msg.getField("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetFieldType()
	{
		gmsec.Log.info("Test getFieldType()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");

			this.check("MESSAGE-TYPE has unexpected field type", msg.getFieldType("MESSAGE-TYPE") == gmsec.Field.Type_STRING);
			this.check("CONTENT-VERSION has unexpected field type", msg.getFieldType("CONTENT-VERSION") == gmsec.Field.Type_F32);

			// Use a bogus field name
			try {
				msg.getFieldType("BOGUS");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain field with name: BOGUS"));
			}

			// Use empty-string field name
			try {
				msg.getFieldType("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetBinaryField()
	{
		gmsec.Log.info("Test getBinaryField()");

		var blob = new gmsec.BlobArray();
		for (let i = 0; i < 256; ++i) {
			blob.add(i);
		}

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("MSG.TLMPKT");
			var tmp = new gmsec.BinaryField("DATA", blob);

			msg.addField(tmp);

			//o Use valid field name
			var field = msg.getBinaryField("DATA");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_BINARY);
			this.check("Unexpected field name", field.getName() === "DATA");
			this.check("Unexpected data length", field.getLength() == blob.size());

			var data = field.getValue();
			for (var i = 0; i < data.size(); i++) {
				if (blob.get(i) != data.get(i)) {
					this.check("BinaryField value at position " + i.toString() + " is wrong", false);
				}
			}

			//o Use non-binary field name
			try {
				msg.getBinaryField("CONTENT-VERSION");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain BinaryField with name: CONTENT-VERSION"));
			}

			//o Use empty-string field name
			try {
				msg.getBinaryField("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetBooleanField()
	{
		gmsec.Log.info("Test getBooleanField()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");
			var tmp = new gmsec.BooleanField("RESPONSE", true);

			msg.addField(tmp);

			//o Use valid field name
			var field = msg.getBooleanField("RESPONSE");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_BOOL);
			this.check("Unexpected field name", field.getName() === "RESPONSE");
			this.check("Unexpected data value", field.getValue() == true);

			//o Use non-boolean field name
			try {
				msg.getBooleanField("CONTENT-VERSION");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain BooleanField with name: CONTENT-VERSION"));
			}

			//o Use empty-string field name
			try {
				msg.getBooleanField("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetCharField()
	{
		gmsec.Log.info("Test getCharField()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");
			var tmp = new gmsec.CharField("CHAR-FIELD", 'c');

			msg.addField(tmp);

			//o Use valid field name
			var field = msg.getCharField("CHAR-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_CHAR);
			this.check("Unexpected field name", field.getName() === "CHAR-FIELD");
			this.check("Unexpected data value", field.getValue() == 'c');

			//o Use non-char field name
			try {
				msg.getCharField("CONTENT-VERSION");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain CharField with name: CONTENT-VERSION"));
			}

			//o Use empty-string field name
			try {
				msg.getCharField("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetF32Field()
	{
		gmsec.Log.info("Test getF32Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			//o Use valid field name
			var field = msg.getF32Field("CONTENT-VERSION");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_F32);
			this.check("Unexpected field name", field.getName() === "CONTENT-VERSION");

			//o Use non-F32 field name
			try {
				msg.getF32Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain F32Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getF32Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetF64Field()
	{
		gmsec.Log.info("Test getF64Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");
			var tmp = new gmsec.F64Field("F64-FIELD", 3.14);

			msg.addField(tmp);

			//o Use valid field name
			var field = msg.getF64Field("F64-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_F64);
			this.check("Unexpected field name", field.getName() === "F64-FIELD");
			this.check("Unexpected field value", parseFloat(field.getValue()).toPrecision(3) == 3.14);

			//o Use non-F64 field name
			try {
				msg.getF64Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain F64Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getF64Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetI8Field()
	{
		gmsec.Log.info("Test getI8Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.I8Field("I8-FIELD", 8) );

			//o Use valid field name
			var field = msg.getI8Field("I8-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I8);
			this.check("Unexpected field name", field.getName() === "I8-FIELD");
			this.check("Unexpected field value", field.getValue() == 8);

			//o Use non-I8 field name
			try {
				msg.getI8Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain I8Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getI8Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetI16Field()
	{
		gmsec.Log.info("Test getI16Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.I16Field("I16-FIELD", 16) );

			//o Use valid field name
			var field = msg.getI16Field("I16-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I16);
			this.check("Unexpected field name", field.getName() === "I16-FIELD");
			this.check("Unexpected field value", field.getValue() == 16);

			//o Use non-I16 field name
			try {
				msg.getI16Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain I16Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getI16Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetI32Field()
	{
		gmsec.Log.info("Test getI32Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.I32Field("I32-FIELD", 32) );

			//o Use valid field name
			var field = msg.getI32Field("I32-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I32);
			this.check("Unexpected field name", field.getName() === "I32-FIELD");
			this.check("Unexpected field value", field.getValue() == 32);

			//o Use non-I32 field name
			try {
				msg.getI32Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain I32Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getI32Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetI64Field()
	{
		gmsec.Log.info("Test getI64Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.I64Field("I64-FIELD", 64) );

			//o Use valid field name
			var field = msg.getI64Field("I64-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_I64);
			this.check("Unexpected field name", field.getName() === "I64-FIELD");
			this.check("Unexpected field value", field.getValue() == 64);

			//o Use non-I64 field name
			try {
				msg.getI64Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain I64Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getI64Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetU8Field()
	{
		gmsec.Log.info("Test getU8Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.U8Field("U8-FIELD", 8) );

			//o Use valid field name
			var field = msg.getU8Field("U8-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U8);
			this.check("Unexpected field name", field.getName() === "U8-FIELD");
			this.check("Unexpected field value", field.getValue() == 8);

			//o Use non-U8 field name
			try {
				msg.getU8Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain U8Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getU8Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetU16Field()
	{
		gmsec.Log.info("Test getU16Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.U16Field("U16-FIELD", 16) );

			//o Use valid field name
			var field = msg.getU16Field("U16-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U16);
			this.check("Unexpected field name", field.getName() === "U16-FIELD");
			this.check("Unexpected field value", field.getValue() == 16);

			//o Use non-U16 field name
			try {
				msg.getU16Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain U16Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getU16Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetU32Field()
	{
		gmsec.Log.info("Test getU32Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.U32Field("U32-FIELD", 32) );

			//o Use valid field name
			var field = msg.getU32Field("U32-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U32);
			this.check("Unexpected field name", field.getName() === "U32-FIELD");
			this.check("Unexpected field value", field.getValue() == 32);

			//o Use non-U32 field name
			try {
				msg.getU32Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain U32Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getU32Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetU64Field()
	{
		gmsec.Log.info("Test getU64Field()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			msg.addField( new gmsec.U64Field("U64-FIELD", 64) );

			//o Use valid field name
			var field = msg.getU64Field("U64-FIELD");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_U64);
			this.check("Unexpected field name", field.getName() === "U64-FIELD");
			this.check("Unexpected field value", field.getValue() == 64);

			//o Use non-U64 field name
			try {
				msg.getU64Field("MESSAGE-TYPE");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain U64Field with name: MESSAGE-TYPE"));
			}

			//o Use empty-string field name
			try {
				msg.getU64Field("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetStringField()
	{
		gmsec.Log.info("Test getStringField()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("REQ.DIR");

			//o Use valid field name
			var field = msg.getStringField("MESSAGE-TYPE");
			this.check("Unexpected field type", field.getType() == gmsec.Field.Type_STRING);
			this.check("Unexpected field name", field.getName() === "MESSAGE-TYPE");
			this.check("Unexpected field value", field.getValue() === "REQ" );

			//o Use non-string field name
			try {
				msg.getStringField("CONTENT-VERSION");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Message does not contain StringField with name: CONTENT-VERSION"));
			}

			//o Use empty-string field name
			try {
				msg.getStringField("");
				this.check("An exception was expected", false);
			}
			catch (e) {
				this.check(e.message, e.message.includes("Field name cannot be NULL, nor an empty string"));
			}
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetFieldCount()
	{
		gmsec.Log.info("Test getFieldCount()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg = msgFactory.createMessage("HB");
			this.check("Message has unexpected field count", msg.getFieldCount() == 13);

			msg.setFieldValue("PUB-RATE", "30");
			this.check("Message has unexpected field count", msg.getFieldCount() == 14);

			msg.clearField("PUB-RATE");
			this.check("Message has unexpected field count", msg.getFieldCount() == 13);

			msg.clearFields();
			this.check("Message has unexpected field count", msg.getFieldCount() == 0);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testCopyFields()
	{
		gmsec.Log.info("Test copyFields()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg1 = msgFactory.createMessage("HB");
			var msg2 = new gmsec.Message(msg1);

			msg1.setFieldValue("PUB-RATE", "30");

			this.check("Message 2 should not have PUB-RATE", msg2.hasField("PUB-RATE") == false);

			msg1.copyFields(msg2);

			this.check("Message 2 should have PUB-RATE", msg2.hasField("PUB-RATE"));
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testToXML()
	{
		gmsec.Log.info("Test toXML()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg1 = msgFactory.createMessage("HB");
			var msg2 = new gmsec.Message(msg1);

			this.check("Message 1 has null XML content", msg1.toXML() != null);
			this.check("Message 2 has null XML content", msg2.toXML() != null);
			this.check("Messages 1 and 2 do not have the same XML content", msg1.toXML() === msg2.toXML());

			var msg3 = msgFactory.fromData(msg1.toXML(), gmsec.DataType_XML_DATA);

			this.check("Messages 1 and 3 do not have the same XML content", msg1.toXML() === msg3.toXML());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testToJSON()
	{
		gmsec.Log.info("Test toJSON()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			this.setStandardFields(msgFactory);

			var msg1 = msgFactory.createMessage("HB");
			var msg2 = new gmsec.Message(msg1);

			this.check("Message 1 has null JSON content", msg1.toJSON() != null);
			this.check("Message 2 has null JSON XML content", msg2.toJSON() != null);
			this.check("Messages 1 and 2 do not have the same JSON content", msg1.toJSON() === msg2.toJSON());

			var msg3 = msgFactory.fromData(msg1.toJSON(), gmsec.DataType_JSON_DATA);

			this.check("Messages 1 and 3 do not have the same JSON content", msg1.toJSON() === msg3.toJSON());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetSize()
	{
		gmsec.Log.info("Test getSize()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg1 = msgFactory.createMessage("HB");
			var msg2 = new gmsec.Message(msg1);

			this.check("Message 1 has unexpected size", msg1.getSize() > 0);
			this.check("Message 2 has unexpected size", msg2.getSize() > 0);
			this.check("Message 1 and 2 should have the same size", msg1.getSize() == msg2.getSize());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetFieldIterator()
	{
		gmsec.Log.info("Test getFieldIterator()");

		try {
			var msgFactory = new gmsec.MessageFactory();
			var msg = msgFactory.createMessage("HB");
			var iter = msg.getFieldIterator();

			this.check("Failed to get the field iterator", iter != null);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}
}


test = new T005_Message();
test.setDescription("Test Message");
test.run(process.argv);
