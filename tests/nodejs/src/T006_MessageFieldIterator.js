#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T006_MessageFieldIterator extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testAllFields();
		this.testHeaderFields();
		this.testNonHeaderFields();
		this.testReset();
	}

	testAllFields()
	{
		gmsec.Log.info("Test ALL_FIELDS");

		var msg = TestMessage.getInstance().getMessage();
		var iter = msg.getFieldIterator(gmsec.MessageFieldIterator.Selector_ALL_FIELDS);

		var fieldsFound = 0;

		while (iter.hasNext()) {
			var field = iter.next();

			fieldsFound += 1;

			if (fieldsFound == 1) {
				this.check("Unexpected field type 1", field.getType() == gmsec.Field.Type_STRING);
				this.check("Unexpected field name 1", field.getName() === "FIELD-1");
				this.check("Unexpected field value 1", field.getStringValue() === "1");
			}
			else if (fieldsFound == 2) {
				this.check("Unexpected field type 2", field.getType() == gmsec.Field.Type_STRING);
				this.check("Unexpected field name 2", field.getName() === "FIELD-2");
				this.check("Unexpected field value 2", field.getStringValue() === "2");
			}
			else {
				this.check("Unexpected field type 3", field.getType() == gmsec.Field.Type_I32);
				this.check("Unexpected field name 3", field.getName() === "FIELD-3");
				this.check("Unexpected field value 3", field.getIntegerValue() === 3);
			}
		}

		this.check("Unexpected field count", fieldsFound == 3);
	}

	testHeaderFields()
	{
		gmsec.Log.info("Test HEADER_FIELDS");

		var msg = TestMessage.getInstance().getMessage();
		var iter = msg.getFieldIterator(gmsec.MessageFieldIterator.Selector_HEADER_FIELDS);

		var fieldsFound = 0;

		while (iter.hasNext()) {
			var field = iter.next();

			fieldsFound += 1;

			if (fieldsFound == 1) {
				this.check("Unexpected field type 1", field.getType() == gmsec.Field.Type_STRING);
				this.check("Unexpected field name 1", field.getName() === "FIELD-1");
				this.check("Unexpected field value 1", field.getStringValue() === "1");
			}
			else if (fieldsFound == 2) {
				this.check("Unexpected field type 2", field.getType() == gmsec.Field.Type_STRING);
				this.check("Unexpected field name 2", field.getName() === "FIELD-2");
				this.check("Unexpected field value 2", field.getStringValue() === "2");
			}
			else {
				this.check("Unexpected field", false);
			}
		}

		this.check("Unexpected field count", fieldsFound == 2);
	}

	testNonHeaderFields()
	{
		gmsec.Log.info("Test NON_HEADER_FIELDS");

		var msg = TestMessage.getInstance().getMessage();
		var iter = msg.getFieldIterator(gmsec.MessageFieldIterator.Selector_NON_HEADER_FIELDS);

		var fieldsFound = 0;

		while (iter.hasNext()) {
			var field = iter.next();

			fieldsFound += 1;

			if (fieldsFound == 1) {
				this.check("Unexpected field type 3", field.getType() == gmsec.Field.Type_I32);
				this.check("Unexpected field name 3", field.getName() === "FIELD-3");
				this.check("Unexpected field value 3", field.getIntegerValue() === 3);
			}
			else {
				this.check("Unexpected field", false);
			}
		}

		this.check("Unexpected field count", fieldsFound == 1);
	}

	testReset()
	{
		gmsec.Log.info("Test reset()");

		var msg = TestMessage.getInstance().getMessage();
		var iter = msg.getFieldIterator();

		var fieldsFound = 0;

		while (iter.hasNext()) {
			iter.next();
			fieldsFound += 1;
		}

		this.check("Unexpected field count", fieldsFound == 3);
		this.check("Should be no more fields to iterate over", iter.hasNext() == false);

		iter.reset();

		this.check("Should be no more fields to iterate over", iter.hasNext());

		fieldsFound = 0;

		while (iter.hasNext()) {
			iter.next();
			fieldsFound += 1;
		}

		this.check("Unexpected field count", fieldsFound == 3);
		this.check("Should be no more fields to iterate over", iter.hasNext() == false);
	}
}

class TestMessage
{
	constructor() {
		this.instance = null;
		this.msg = null;
	}

	static getInstance() {
		if (!this.instance) {
			this.instance = new TestMessage();
		}
		return this.instance;
	}

	getMessage() {
		this.msg = new gmsec.Message();

		var field1 = new gmsec.StringField("FIELD-1", "1", true);  // header field
		var field2 = new gmsec.StringField("FIELD-2", "2", true);  // header field
		var field3 = new gmsec.I32Field("FIELD-3", 3);             // non-header field

		this.msg.addField(field1);
		this.msg.addField(field2);
		this.msg.addField(field3);

		return this.msg;
	}
}


test = new T006_MessageFieldIterator();
test.setDescription("Test MessageFieldIterator");
test.run(process.argv);
