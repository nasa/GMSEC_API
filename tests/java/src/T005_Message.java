import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.MessageFieldIterator;
import gov.nasa.gsfc.gmsec.api5.MessageValidator;
import gov.nasa.gsfc.gmsec.api5.Specification;
import gov.nasa.gsfc.gmsec.api5.Status;
import gov.nasa.gsfc.gmsec.api5.StatusClassification;
import gov.nasa.gsfc.gmsec.api5.StatusCode;
import gov.nasa.gsfc.gmsec.api5.U16;
import gov.nasa.gsfc.gmsec.api5.U32;
import gov.nasa.gsfc.gmsec.api5.U64;
import gov.nasa.gsfc.gmsec.api5.U8;

import gov.nasa.gsfc.gmsec.api5.field.*;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.ArrayList;
import java.util.List;


public class T005_Message extends TestCase
{
	public static void main(String[] args)
	{
		T005_Message test = new T005_Message();
		test.setDescription("Test Message");
		test.run(args);
	}


	public void testCase()
		throws Exception
	{
		try
		{
			test_constructor();
			test_copy_constructor();
			test_destroy();
			test_get_schema_id();
			test_get_version();
			test_get_schema_level();
			test_is_compliant();
			test_register_message_validator();
			test_set_field_value();
			test_set_config();
			test_set_subject();
			test_set_subject_element();
			test_get_subject();
			test_set_kind();
			test_get_kind();
			test_add_field();
			test_add_fields();
			test_clear_fields();
			test_clear_field();
			test_get_string_value();
			test_get_boolean_value();
			test_get_i16_value();
			test_get_i32_value();
			test_get_i64_value();
			test_get_u16_value();
			test_get_u32_value();
			test_get_u64_value();
			test_get_f64_value();
			test_has_field();
			test_get_field();
			test_get_field_type();
			test_get_binary_field();
			test_get_boolean_field();
			test_get_char_field();
			test_get_f32_field();
			test_get_f64_field();
			test_get_i8_field();
			test_get_i16_field();
			test_get_i32_field();
			test_get_i64_field();
			test_get_u8_field();
			test_get_u16_field();
			test_get_u32_field();
			test_get_u64_field();
			test_get_string_field();
			test_get_field_count();
			test_copy_fields();
			test_to_xml();
			test_to_json();
			test_get_size();
			test_get_field_iterator();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_constructor()
		throws Exception
	{
		Log.info("Test constructor");

		Message msg = new Message();

		check("Unexpected subject", msg.getSubject().equals(""));
		check("Unexpected kind", msg.getKind() == Message.Kind.PUBLISH);
		check("Unexpected schema ID", msg.getSchemaID().equals(""));
		check("Unexpected version", msg.getVersion() == Gmsec.MSG_SPEC_CURRENT);
		check("Unexpected schema level", msg.getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		check("Unexpected compliancy", msg.isCompliant().hasError() == true);
		check("Unexpected field count", msg.getFieldCount() == 0);
		check("Unexpected XML", msg.toXML() != null);
		check("Unexpected JSON", msg.toXML() != null);
	}


	private void test_copy_constructor()
		throws Exception
	{
		Log.info("Test copy-constructor");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");

		// Nominal test
		Message msg2 = new Message(msg1);

		check("Unexpected subject", msg1.getSubject().equals(msg2.getSubject()));
		check("Unexpected kind", msg1.getKind() == msg2.getKind());
		check("Unexpected schema ID", msg1.getSchemaID().equals(msg2.getSchemaID()));
		check("Unexpected version", msg1.getVersion() == msg2.getVersion());
		check("Unexpected schema level", msg1.getSchemaLevel() == msg2.getSchemaLevel());
		check("Expected compliant message", msg1.isCompliant().hasError() == msg2.isCompliant().hasError());
		check("Unexpected field count", msg1.getFieldCount() == msg2.getFieldCount());
		check("Unexpected XML", msg1.toXML().equals(msg2.toXML()));
		check("Unexpected JSON", msg1.toJSON().equals(msg2.toJSON()));

		MessageFactory.destroy(msgFactory);

		// Off-Nominal test
		try
		{
			new Message((Message) null);
			check("Expected exception", false);
		}
		catch (Exception e)
		{
			check(e.getMessage(), e.getMessage().contains("Message is null"));
		}
	}


	private void test_destroy()
		throws Exception
	{
		Log.info("Test destroy()");

		Message msg = new Message();

		Message.destroy(msg);
		check("First time will be okay", true);

		Message.destroy(msg);
		check("Second time will be okay", true);
	}


	private void test_get_schema_id()
		throws Exception
	{
		Log.info("Test getSchemaID()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = msgFactory.createMessage("MVAL");
		Message msg3 = msgFactory.createMessage("TLMFRAME");

		check("Message 1 has unexpected schema ID", msg1.getSchemaID().equals("MSG.HB"));
		check("Message 2 has unexpected schema ID", msg2.getSchemaID().equals("MSG.MVAL"));
		check("Message 3 has unexpected schema ID", msg3.getSchemaID().equals("MSG.TLMFRAME"));

		MessageFactory.destroy(msgFactory);
	}


	private void test_get_version()
		throws Exception
	{
		Log.info("Test getVersion()");

		MessageFactory msgFactory1 = MessageFactory.create();
		MessageFactory msgFactory2 = MessageFactory.create(new Config("gmsec-specification-version=201900", Gmsec.DataType.KEY_VALUE_DATA));

		Message msg1 = msgFactory1.createMessage("HB");
		Message msg2 = msgFactory2.createMessage("HB");

		check("Message 1 has unexpected version", msg1.getVersion() == Gmsec.MSG_SPEC_CURRENT);
		check("Message 2 has unexpected version", msg2.getVersion() == Gmsec.MSG_SPEC_2019_00);

		MessageFactory.destroy(msgFactory1);
		MessageFactory.destroy(msgFactory2);
	}


	private void test_get_schema_level()
		throws Exception
	{
		Log.info("Test getSchemaLevel()");

		MessageFactory msgFactory1 = MessageFactory.create();
		MessageFactory msgFactory2 = MessageFactory.create(new Config("gmsec-schema-level=2", Gmsec.DataType.KEY_VALUE_DATA));

		Message msg1 = msgFactory1.createMessage("HB");
		Message msg2 = msgFactory2.createMessage("HB");

		check("Message 1 has unexpected version", msg1.getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		check("Message 2 has unexpected version", msg2.getSchemaLevel() == Specification.SchemaLevel.LEVEL_2);

		MessageFactory.destroy(msgFactory1);
		MessageFactory.destroy(msgFactory2);
	}


	private void test_is_compliant()
		throws Exception
	{
		Log.info("Test isCompliant()");

		Config config = new Config(getConfig());   // make a copy
		config.addValue("gmsec-msg-content-validate", "true");

		MessageFactory msgFactory = MessageFactory.create(config);

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("HB");

		// Nominal test
		check("Expected compliant message", msg.isCompliant().hasError() == false);

		// Off-Nominal test
		msg.clearField("DOMAIN1");
		Status status = msg.isCompliant();
		check("Expected non-compliant message", status.hasError() == true);

		MessageFactory.destroy(msgFactory);
	}


	private void test_register_message_validator()
		throws Exception
	{
		Log.info("Test registerMessageValidator()");

		T005_MessageValidator validator = new T005_MessageValidator();

		Config config = new Config(getConfig());   // make a copy
		config.addValue("gmsec-msg-content-validate", "true");

		MessageFactory msgFactory = MessageFactory.create(config);

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("HB");

		msg.registerMessageValidator(validator);

		// Expecting no error
		Status status = msg.isCompliant();
		check("Expected compliant message", status.hasError() == false);

		// Expecting an error
		status = msg.isCompliant();
		check("Expected non-compliant message", status.hasError() == true);
		check("Expected non-compliant message", status.getReason().equals("Field is missing"));

		// Expecting an error
		Message msg2 = new Message(msg);
		status = msg2.isCompliant();
		check("Expected non-compliant message", status.hasError() == true);
		check("Expected non-compliant message", status.getReason().equals("Field is missing"));

		MessageFactory.destroy(msgFactory);
	}


	private void test_set_field_value()
		throws Exception
	{
		Log.info("Test setFieldValue()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("HB");

		//o Test with string value
		msg.setFieldValue("PUB-RATE", "5");
		check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == Field.Type.U16);
		check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 5);

		//o Test with int value
		msg.setFieldValue("PUB-RATE", 10);
		check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == Field.Type.U16);
		check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 10);

		//o Test with long value
		msg.setFieldValue("PUB-RATE", (long) 20);
		check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == Field.Type.U16);
		check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 20);

		//o Test with double value
		msg.setFieldValue("PUB-RATE", 40.123);
		check("PUB-RATE has unexpected field type", msg.getField("PUB-RATE").getType() == Field.Type.U16);
		check("PUB-RATE has unexpected field value", msg.getI32Value("PUB-RATE") == 40);

		//o Verify result returned is 'true' when replacing a field
		check("Expected return value of true", msg.setFieldValue("PUB-RATE", 10) == true);

		//o Verify result returned is 'false' when adding a new field
		check("Expected return value of false", msg.setFieldValue("PUBLISH-RATE", 10) == false);


		MessageFactory.destroy(msgFactory);
	}


	private void test_set_config()
		throws Exception
	{
		Log.info("Test setConfig()");

		Config config = new Config("tracking=false", Gmsec.DataType.KEY_VALUE_DATA);

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");

		msg.setConfig(config);

		check("Message has unexpected Config content", config.toXML().equals(msg.getConfig().toXML()));

		MessageFactory.destroy(msgFactory);
	}


	private void test_set_subject()
		throws Exception
	{
		Log.info("Test setSubject()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");

		msg.setSubject("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY");

		check("Message has unexpected subject", msg.getSubject().equals("GMSEC.MY-MISSION.MY-SAT-ID.MSG.TLMPROC.FOOEY"));

		MessageFactory.destroy(msgFactory);

		//o Off-nominal tests
		try {
			//o null subject
			Message tmp = new Message();
			tmp.setSubject(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Subject is null or contains an empty string"));
		}

		try {
			//o zero-length subject
			Message tmp = new Message();
			tmp.setSubject("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Subject is null or contains an empty string"));
		}

		try {
			//o invalid subject content
			Message tmp = new Message();
			tmp.setSubject("Subject.Cannot.Contain.Lowercase.Letters");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Invalid message subject"));
		}

		try {
			//o invalid subject content
			Message tmp = new Message();
			tmp.setSubject("HELLO%WORLD");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Invalid message subject"));
		}
	}


	private void test_set_subject_element()
		throws Exception
	{
		Log.info("Test setSubjectElement()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");

		msg.setSubjectElement("ME4", "FOOEY");

		check("Message has unexpected subject", msg.getSubject().equals("C2MS.MY-DOMAIN-1.MY-DOMAIN-2.MY-MISSION.MY-CONSTELLATION.MY-SAT-ID.MSG.TLMPROC.MY-COMPONENT.FILL.FILL.FOOEY"));
		try {
			msg.setSubjectElement("ME9000", "FOOEY");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not have a subject element named ME9000"));
		}

		MessageFactory.destroy(msgFactory);

		//o Off-nominal tests
		try {
			Message tmp = new Message();
			tmp.setSubjectElement(null, "FOOEY");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Name is null or contains an empty string"));
		}

		try {
			Message tmp = new Message();
			tmp.setSubjectElement("", "FOOEY");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Name is null or contains an empty string"));
		}

		try {
			Message tmp = new Message();
			tmp.setSubjectElement("ME4", "FOOEY");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException | GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not have a message template"));
		}
	}


	private void test_get_subject()
		throws Exception
	{
		Log.info("Test getSubject()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("TLMPROC");
		msg.addField("STREAM-MODE", "RT");  // add ME2 (stream mode)

		String subject = msg.getSubject();
		subject += ".2";   // add ME2 (virtual channel ID)
		subject += ".1";   // add ME3 (AP ID)

		msg.setSubject(subject);

		check("Message has unexpected subject", msg.getSubject().equals(subject));

		MessageFactory.destroy(msgFactory);
	}


	public void test_set_kind()
		throws Exception
	{
		Log.info("Test setKind()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("HB");
		check("Expected PUBLISH-kind", msg.getKind() == Message.Kind.PUBLISH);

		// Set to REQUEST-kind, and verify
		msg.setKind(Message.Kind.REQUEST);
		check("Expected REQUEST-kind", msg.getKind() == Message.Kind.REQUEST);

		// Set to REPLY-kind, and verify
		msg.setKind(Message.Kind.REPLY);
		check("Expected REPLY-kind", msg.getKind() == Message.Kind.REPLY);

		// Set to PUBLISH-kind, and verify
		msg.setKind(Message.Kind.PUBLISH);
		check("Expected PUBLISH-kind", msg.getKind() == Message.Kind.PUBLISH);

		MessageFactory.destroy(msgFactory);
	}


	public void test_get_kind()
		throws Exception
	{
		Log.info("Test getKind()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = msgFactory.createMessage("REQ.DIR");
		Message msg3 = msgFactory.createMessage("RESP.DIR");
		Message msg4 = msgFactory.createMessage();
		Message msg5 = new Message();

		check("Message 1 has unexpected kind", msg1.getKind() == Message.Kind.PUBLISH);
		check("Message 2 has unexpected kind", msg2.getKind() == Message.Kind.REQUEST);
		check("Message 3 has unexpected kind", msg3.getKind() == Message.Kind.REPLY);
		check("Message 4 has unexpected kind", msg4.getKind() == Message.Kind.PUBLISH);
		check("Message 5 has unexpected kind", msg5.getKind() == Message.Kind.PUBLISH);

		MessageFactory.destroy(msgFactory);
	}


	private void test_add_field()
		throws Exception
	{
		Log.info("Test addField()");

		StringField field = new StringField("SPECIFICATION", "C2MS");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("LOG");

		MessageFactory.destroy(msgFactory);

		//o replace existing field
		check("Expected field to be replaced", msg.addField(field) == true);

		//o test other overloaded methods
		check("Expected new field to be added", msg.addField("BINARY", new byte[6]) == false);
		check("Expected new field to be added", msg.addField("BOOLEAN", true) == false);
		check("Expected new field to be added", msg.addField("CHAR", 'c') == false);
		check("Expected new field to be added", msg.addField("FLOAT", 1.2f) == false);
		check("Expected new field to be added", msg.addField("DOUBLE", 2.4) == false);
		check("Expected new field to be added", msg.addField("BYTE", (byte) 5) == false);
		check("Expected new field to be added", msg.addField("SHORT", (short) 55) == false);
		check("Expected new field to be added", msg.addField("INT", 555) == false);
		check("Expected new field to be added", msg.addField("LONG", (long) 5555) == false);
		check("Expected new field to be added", msg.addField("STRING", "The old fox") == false);
		check("Expected new field to be added", msg.addField("UBYTE", new U8(5)) == false);
		check("Expected new field to be added", msg.addField("USHORT", new U16(55)) == false);
		check("Expected new field to be added", msg.addField("UINT", new U32((long) 555)) == false);
		check("Expected new field to be added", msg.addField("ULONG", new U64("5555")) == false);

		//o validate field settings
		check("SPECIFICATION not found", msg.hasField("SPECIFICATION"));
		check("SPECIFICATION has wrong type", msg.getField("SPECIFICATION").getType() == Field.Type.STRING);
		check("SPECIFICATION has wrong value", msg.getStringValue("SPECIFICATION").equals("C2MS"));

		check("BINARY not found", msg.hasField("BINARY"));
		check("BINARY has wrong type", msg.getField("BINARY").getType() == Field.Type.BINARY);
		check("BINARY has wrong length", msg.getBinaryField("BINARY").getValue().length == 6);

		check("BOOLEAN not found", msg.hasField("BOOLEAN"));
		check("BOOLEAN has wrong type", msg.getField("BOOLEAN").getType() == Field.Type.BOOL);
		check("BOOLEAN has wrong value", msg.getBooleanField("BOOLEAN").getValue() == true);

		check("CHAR not found", msg.hasField("CHAR"));
		check("CHAR has wrong type", msg.getField("CHAR").getType() == Field.Type.CHAR);
		check("CHAR has wrong value", msg.getCharField("CHAR").getValue() == 'c');

		check("FLOAT not found", msg.hasField("FLOAT"));
		check("FLOAT has wrong type", msg.getField("FLOAT").getType() == Field.Type.F32);
		check("FLOAT has wrong value", msg.getF32Field("FLOAT").getValue() == 1.2f);

		check("DOUBLE not found", msg.hasField("DOUBLE"));
		check("DOUBLE has wrong type", msg.getField("DOUBLE").getType() == Field.Type.F64);
		check("DOUBLE has wrong value", msg.getF64Field("DOUBLE").getValue() == 2.4);

		check("BYTE not found", msg.hasField("BYTE"));
		check("BYTE has wrong type", msg.getField("BYTE").getType() == Field.Type.I8);
		check("BYTE has wrong value", msg.getI8Field("BYTE").getValue() == (byte) 5);

		check("SHORT not found", msg.hasField("SHORT"));
		check("SHORT has wrong type", msg.getField("SHORT").getType() == Field.Type.I16);
		check("SHORT has wrong value", msg.getI16Field("SHORT").getValue() == (short) 55);

		check("INT not found", msg.hasField("INT"));
		check("INT has wrong type", msg.getField("INT").getType() == Field.Type.I32);
		check("INT has wrong value", msg.getI32Field("INT").getValue() == 555);

		check("LONG not found", msg.hasField("LONG"));
		check("LONG has wrong type", msg.getField("LONG").getType() == Field.Type.I64);
		check("LONG has wrong value", msg.getI64Field("LONG").getValue() == (long) 5555);

		check("STRING not found", msg.hasField("STRING"));
		check("STRING has wrong type", msg.getField("STRING").getType() == Field.Type.STRING);
		check("STRING has wrong value", msg.getStringField("STRING").getValue().equals("The old fox"));

		check("UBYTE not found", msg.hasField("UBYTE"));
		check("UBYTE has wrong type", msg.getField("UBYTE").getType() == Field.Type.U8);
		check("UBYTE has wrong value", msg.getU8Field("UBYTE").getValue().get() == 5);

		check("USHORT not found", msg.hasField("USHORT"));
		check("USHORT has wrong type", msg.getField("USHORT").getType() == Field.Type.U16);
		check("USHORT has wrong value", msg.getU16Field("USHORT").getValue().get() == 55);

		check("UINT not found", msg.hasField("UINT"));
		check("UINT has wrong type", msg.getField("UINT").getType() == Field.Type.U32);
		check("UINT has wrong value", msg.getU32Field("UINT").getValue().get() == (long) 555);

		check("ULONG not found", msg.hasField("ULONG"));
		check("ULONG has wrong type", msg.getField("ULONG").getType() == Field.Type.U64);
		check("ULONG has wrong value", msg.getU64Field("ULONG").getValue().toString().equals("5555"));
	}


	private void test_add_fields()
		throws Exception
	{
		Log.info("Test addFields()");

		StringField field1 = new StringField("ONE", "uno");
		StringField field2 = new StringField("TWO", "dos");
		StringField field3 = new StringField("THREE", "tres");

		List<Field> fields = new ArrayList<Field>();
		fields.add(field1);
		fields.add(field2);
		fields.add(field3);

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("LOG");

		MessageFactory.destroy(msgFactory);

		check("Expected only new fields to be added", false == msg.addFields(fields));

		check("Expected to have field ONE", true == msg.hasField("ONE"));
		check("Unexpected field type for ONE", Field.Type.STRING == msg.getField("ONE").getType());
		check("Unexpected field value for ONE", msg.getStringValue("ONE").equals("uno"));

		check("Expected to have field TWO", true == msg.hasField("TWO"));
		check("Unexpected field type for TWO", Field.Type.STRING == msg.getField("TWO").getType());
		check("Unexpected field value for TWO", msg.getStringValue("TWO").equals("dos"));

		check("Expected to have field THREE", true == msg.hasField("THREE"));
		check("Unexpected field type for THREE", Field.Type.STRING == msg.getField("THREE").getType());
		check("Unexpected field value for THREE", msg.getStringValue("THREE").equals("tres"));
	}


	private void test_clear_fields()
		throws Exception
	{
		Log.info("Test clearFields()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		check("Message missing CONTENT-VERSION", true == msg.hasField("CONTENT-VERSION"));
		check("Message missing HEADER-VERSION", true == msg.hasField("HEADER-VERSION"));
		check("Message missing MESSAGE-SUBTYPE", true == msg.hasField("MESSAGE-SUBTYPE"));
		check("Message missing MESSAGE-TYPE", true == msg.hasField("MESSAGE-TYPE"));
		check("Message missing SPECIFICATION", true == msg.hasField("SPECIFICATION"));

		msg.clearFields();

		check("Message has CONTENT-VERSION", false == msg.hasField("CONTENT-VERSION"));
		check("Message has HEADER-VERSION", false == msg.hasField("HEADER-VERSION"));
		check("Message has MESSAGE-SUBTYPE", false == msg.hasField("MESSAGE-SUBTYPE"));
		check("Message has MESSAGE-TYPE", false == msg.hasField("MESSAGE-TYPE"));
		check("Message has SPECIFICATION", false == msg.hasField("SPECIFICATION"));
	}


	private void test_clear_field()
		throws Exception
	{
		Log.info("Test clearField()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		check("CONTENT-VERSION not found in message", msg.hasField("CONTENT-VERSION"));

		check("CONTENT-VERSION not cleared from message", msg.clearField("CONTENT-VERSION") == true);
		check("FOO-BAR does not exist and cannot be cleared", msg.clearField("FOO-BAR") == false);
		check("foo-bar does not exist and cannot be cleared", msg.clearField("foo-bar") == false);

		check("CONTENT-VERSION should not exist in message", msg.hasField("CONTENT-VERSION") == false);
	}


	private void test_get_string_value()
		throws Exception
	{
		Log.info("Test getStringValue()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("MESSAGE-TYPE has unexpected value", msg.getStringValue("MESSAGE-TYPE").equals("MSG"));
		check("PUB-RATE has unexpected value", msg.getStringValue("PUB-RATE").equals("30"));
	}


	private void test_get_boolean_value()
		throws Exception
	{
		Log.info("Test getBooleanValue()");

		Message msg = new Message();

		msg.addField("FIELD-1", 0);
		msg.addField("FIELD-2", 1);
		msg.addField("FIELD-3", "10000");
		msg.addField("FIELD-4", 10000);
		msg.addField("FIELD-5", true);
		msg.addField("FIELD-6", false);
		msg.addField("FIELD-7", 2019.0);
		msg.addField("FIELD-8", "true");
		msg.addField("FIELD-9", "FALSE");
		msg.addField("FIELD-0", "foo bar");

		// Nominal tests
		check("1 Expected boolean value of false", msg.getBooleanValue("FIELD-1") == false);
		check("2 Expected boolean value of true",  msg.getBooleanValue("FIELD-2"));
		check("3 Expected boolean value of true",  msg.getBooleanValue("FIELD-3"));
		check("4 Expected boolean value of true",  msg.getBooleanValue("FIELD-4"));
		check("5 Expected boolean value of true",  msg.getBooleanValue("FIELD-5"));
		check("6 Expected boolean value of false", msg.getBooleanValue("FIELD-6") == false);
		check("7 Expected boolean value of true",  msg.getBooleanValue("FIELD-7"));
		check("8 Expected boolean value of true",  msg.getBooleanValue("FIELD-8"));
		check("9 Expected boolean value of false", msg.getBooleanValue("FIELD-9") == false);

		// Off-nominal tests
		// Field cannot be converted
		try {
			msg.getBooleanValue("FIELD-0");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be represented as a boolean"));
		}

		// Unknown field
		try {
			msg.getBooleanValue("BOGUS");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain field with name: BOGUS"));
		}

		// NULL field name
		try {
			msg.getBooleanValue(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		// empty-string field name
		try {
			msg.getBooleanValue("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_i16_value()
		throws Exception
	{
		Log.info("Test getI16Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getI16Value("PUB-RATE") == (short) 30);

		try {
			msg.getI16Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_I16"));
		}
	}


	private void test_get_i32_value()
		throws Exception
	{
		Log.info("Test getI32Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getI32Value("PUB-RATE") == 30);

		try {
			msg.getI32Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_I32"));
		}
	}


	private void test_get_i64_value()
		throws Exception
	{
		Log.info("Test getI64Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getI64Value("PUB-RATE") == (long) 30);

		try {
			msg.getI64Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_I64"));
		}
	}


	private void test_get_u16_value()
		throws Exception
	{
		Log.info("Test getU16Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getU16Value("PUB-RATE").get() == (int) 30);

		try {
			msg.getU16Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_U16"));
		}
	}


	private void test_get_u32_value()
		throws Exception
	{
		Log.info("Test getU32Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getU32Value("PUB-RATE").get() == (long) 30);

		try {
			msg.getU32Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_U32"));
		}
	}


	private void test_get_u64_value()
		throws Exception
	{
		Log.info("Test getU64Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getU64Value("PUB-RATE").toString().equals("30"));

		try {
			msg.getU64Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_U64"));
		}
	}


	private void test_get_f64_value()
		throws Exception
	{
		Log.info("Test getF64Value()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("PUB-RATE has unexpected value", msg.getF64Value("PUB-RATE") == (double) 30);

		try {
			msg.getF64Value("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Field cannot be converted to a GMSEC_F64"));
		}
	}


	private void test_has_field()
		throws Exception
	{
		Log.info("Test hasField()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		msg.setFieldValue("PUB-RATE", "30");

		check("Message should have PUB-RATE", msg.hasField("PUB-RATE"));
		check("Message should not have PUBLISH-RATE", msg.hasField("PUBLISH-RATE") == false);
		check("Message should not have null field", msg.hasField(null) == false);
		check("Message should not have empty string field", msg.hasField("") == false);
	}


	private void test_get_field()
		throws Exception
	{
		Log.info("Test getField()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		//o Use valid field name
		check("Message should have MESSAGE-TYPE", msg.getField("MESSAGE-TYPE") != null);

		//o Use bogus field name
		check("Message should have MSG-TYPE", msg.getField("MSG-TYPE") == null);

		//o Use null field name
		try {
			msg.getField(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getField("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_field_type()
		throws Exception
	{
		Log.info("Test getFieldType()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		check("MESSAGE-TYPE has unexpected field type", msg.getFieldType("MESSAGE-TYPE") == Field.Type.STRING);
		check("CONTENT-VERSION has unexpected field type", msg.getFieldType("CONTENT-VERSION") == Field.Type.F32);

		//o Use bogus field name
		try {
			msg.getFieldType("MSG-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain field with name: MSG-TYPE"));
		}

		//o Use null field name
		try {
			msg.getFieldType(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getFieldType("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_binary_field()
		throws Exception
	{
		Log.info("Test getBinaryField()");

		byte[] data = new byte[256];
		for (int i = 0; i < data.length; ++i) {
			data[i] = (byte)(i & 0xFF);
		}

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("MSG.TLMPKT");

		MessageFactory.destroy(msgFactory);

		msg.addField("DATA", data);

		//o Use valid field name
		BinaryField field = msg.getBinaryField("DATA");
		check("Unexpected field type", field.getType() == Field.Type.BINARY);
		check("Unexpected field name", field.getName().equals("DATA"));
		check("Unexpected data length", field.getValue().length == data.length);

		//o Use non-binary field name
		try {
			msg.getBinaryField("CONTENT-VERSION");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain BinaryField with name: CONTENT-VERSION"));
		}

		//o Use null field name
		try {
			msg.getBinaryField(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getBinaryField("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_boolean_field()
		throws Exception
	{
		Log.info("Test getBooleanField()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("RESPONSE", true);

		//o Use valid field name
		BooleanField field = msg.getBooleanField("RESPONSE");
		check("Unexpected field type", field.getType() == Field.Type.BOOL);
		check("Unexpected field name", field.getName().equals("RESPONSE"));
		check("Unexpected data value", field.getValue() == true);

		//o Use non-boolean field name
		try {
			msg.getBooleanField("CONTENT-VERSION");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain BooleanField with name: CONTENT-VERSION"));
		}

		//o Use null field name
		try {
			msg.getBooleanField(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getBooleanField("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_char_field()
		throws Exception
	{
		Log.info("Test getCharField()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("CHAR-FIELD", 'c');

		//o Use valid field name
		CharField field = msg.getCharField("CHAR-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.CHAR);
		check("Unexpected field name", field.getName().equals("CHAR-FIELD"));
		check("Unexpected data value", field.getValue() == 'c');

		//o Use non-char field name
		try {
			msg.getCharField("CONTENT-VERSION");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain CharField with name: CONTENT-VERSION"));
		}

		//o Use null field name
		try {
			msg.getCharField(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getCharField("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_f32_field()
		throws Exception
	{
		Log.info("Test getF32Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		//o Use valid field name
		F32Field field = msg.getF32Field("CONTENT-VERSION");
		check("Unexpected field type", field.getType() == Field.Type.F32);
		check("Unexpected field name", field.getName().equals("CONTENT-VERSION"));

		//o Use non-F32 field name
		try {
			msg.getF32Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain F32Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getF32Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getF32Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_f64_field()
		throws Exception
	{
		Log.info("Test getF64Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("F64-FIELD", 3.14);

		//o Use valid field name
		F64Field field = msg.getF64Field("F64-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.F64);
		check("Unexpected field name", field.getName().equals("F64-FIELD"));
		check("Unexpected field value", field.getValue() == 3.14);

		//o Use non-F64 field name
		try {
			msg.getF64Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain F64Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getF64Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getF64Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_i8_field()
		throws Exception
	{
		Log.info("Test getI8Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("I8-FIELD", (byte) 8);

		//o Use valid field name
		I8Field field = msg.getI8Field("I8-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.I8);
		check("Unexpected field name", field.getName().equals("I8-FIELD"));
		check("Unexpected field value", field.getValue() == (byte) 8);

		//o Use non-I8 field name
		try {
			msg.getI8Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain I8Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getI8Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getI8Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_i16_field()
		throws Exception
	{
		Log.info("Test getI16Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("I16-FIELD", (short) 16);

		//o Use valid field name
		I16Field field = msg.getI16Field("I16-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.I16);
		check("Unexpected field name", field.getName().equals("I16-FIELD"));
		check("Unexpected field value", field.getValue() == (short) 16);

		//o Use non-I16 field name
		try {
			msg.getI16Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain I16Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getI16Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getI16Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_i32_field()
		throws Exception
	{
		Log.info("Test getI32Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("I32-FIELD", 32);

		//o Use valid field name
		I32Field field = msg.getI32Field("I32-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.I32);
		check("Unexpected field name", field.getName().equals("I32-FIELD"));
		check("Unexpected field value", field.getValue() == 32);

		//o Use non-I32 field name
		try {
			msg.getI32Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain I32Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getI32Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getI32Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_i64_field()
		throws Exception
	{
		Log.info("Test getI64Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("I64-FIELD", (long) 64);

		//o Use valid field name
		I64Field field = msg.getI64Field("I64-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.I64);
		check("Unexpected field name", field.getName().equals("I64-FIELD"));
		check("Unexpected field value", field.getValue() == (long) 64);

		//o Use non-I64 field name
		try {
			msg.getI64Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain I64Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getI64Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getI64Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_u8_field()
		throws Exception
	{
		Log.info("Test getU8Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("U8-FIELD", new U8(8));

		//o Use valid field name
		U8Field field = msg.getU8Field("U8-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.U8);
		check("Unexpected field name", field.getName().equals("U8-FIELD"));
		check("Unexpected field value", field.getValue().get() == 8);

		//o Use non-U8 field name
		try {
			msg.getU8Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain U8Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getU8Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getU8Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_u16_field()
		throws Exception
	{
		Log.info("Test getU16Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("U16-FIELD", new U16(16));

		//o Use valid field name
		U16Field field = msg.getU16Field("U16-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.U16);
		check("Unexpected field name", field.getName().equals("U16-FIELD"));
		check("Unexpected field value", field.getValue().get() == 16);

		//o Use non-U16 field name
		try {
			msg.getU16Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain U16Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getU16Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getU16Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_u32_field()
		throws Exception
	{
		Log.info("Test getU32Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("U32-FIELD", new U32((long) 32));

		//o Use valid field name
		U32Field field = msg.getU32Field("U32-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.U32);
		check("Unexpected field name", field.getName().equals("U32-FIELD"));
		check("Unexpected field value", field.getValue().get() == (long) 32);

		//o Use non-U32 field name
		try {
			msg.getU32Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain U32Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getU32Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getU32Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_u64_field()
		throws Exception
	{
		Log.info("Test getU64Field()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		msg.addField("U64-FIELD", new U64("64"));

		//o Use valid field name
		U64Field field = msg.getU64Field("U64-FIELD");
		check("Unexpected field type", field.getType() == Field.Type.U64);
		check("Unexpected field name", field.getName().equals("U64-FIELD"));
		check("Unexpected field value", field.getValue().toString().equals("64"));

		//o Use non-U64 field name
		try {
			msg.getU64Field("MESSAGE-TYPE");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain U64Field with name: MESSAGE-TYPE"));
		}

		//o Use null field name
		try {
			msg.getU64Field(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getU64Field("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_string_field()
		throws Exception
	{
		Log.info("Test getStringField()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("REQ.DIR");

		MessageFactory.destroy(msgFactory);

		//o Use valid field name
		StringField field = msg.getStringField("MESSAGE-TYPE");
		check("Unexpected field type", field.getType() == Field.Type.STRING);
		check("Unexpected field name", field.getName().equals("MESSAGE-TYPE"));
		check("Unexpected field value", field.getValue().equals("REQ"));

		//o Use non-string field name
		try {
			msg.getStringField("CONTENT-VERSION");
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message does not contain StringField with name: CONTENT-VERSION"));
		}

		//o Use null field name
		try {
			msg.getStringField(null);
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}

		//o Use empty-string field name
		try {
			msg.getStringField("");
			check("An exception was expected", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Field name is null or contains an empty string"));
		}
	}


	private void test_get_field_count()
		throws Exception
	{
		Log.info("Test getFieldCount()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		check("Message has unexpected field count", msg.getFieldCount() == 13);

		msg.setFieldValue("PUB-RATE", "30");

		check("Message has unexpected field count", msg.getFieldCount() == 14);

		msg.clearField("PUB-RATE");

		check("Message has unexpected field count", msg.getFieldCount() == 13);

		msg.clearFields();

		check("Message has unexpected field count", msg.getFieldCount() == 0);
	}


	private void test_copy_fields()
		throws Exception
	{
		Log.info("Test copyFields()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = new Message(msg1);

		MessageFactory.destroy(msgFactory);

		msg1.setFieldValue("PUB-RATE", "30");

		check("Message 2 should not have PUB-RATE", msg2.hasField("PUB-RATE") == false);

		msg1.copyFields(msg2);

		check("Message 2 should have PUB-RATE", msg2.hasField("PUB-RATE"));
	}


	private void test_to_xml()
		throws Exception
	{
		Log.info("Test toXML()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = new Message(msg1);

		check("Message 1 has NULL XML content", msg1.toXML() != null);
		check("Message 2 has NULL XML content", msg2.toXML() != null);
		check("Messages 1 and 2 do not have same XML content", msg1.toXML().equals(msg2.toXML()));

		Message msg3 = msgFactory.fromData(msg1.toXML(), Gmsec.DataType.XML_DATA);
		check("Messages 1 and 3 do not have same XML content", msg1.toXML().equals(msg3.toXML()));

		MessageFactory.destroy(msgFactory);
	}


	private void test_to_json()
		throws Exception
	{
		Log.info("Test toJSON()");

		MessageFactory msgFactory = MessageFactory.create();

		setStandardFields(msgFactory);

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = new Message(msg1);

		check("Message 1 has NULL JSON content", msg1.toJSON() != null);
		check("Message 2 has NULL JSON content", msg2.toJSON() != null);
		check("Messages 1 and 2 do not have same JSON content", msg1.toJSON().equals(msg2.toJSON()));

		Message msg3 = msgFactory.fromData(msg1.toJSON(), Gmsec.DataType.JSON_DATA);
		check("Messages 1 and 3 do not have same JSON content", msg1.toJSON().equals(msg3.toJSON()));

		MessageFactory.destroy(msgFactory);
	}


	private void test_get_size()
		throws Exception
	{
		Log.info("Test getSize()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg1 = msgFactory.createMessage("HB");
		Message msg2 = new Message(msg1);

		MessageFactory.destroy(msgFactory);

		check("Message 1 has unexpected size", msg1.getSize() > 0);
		check("Message 2 has unexpected size", msg2.getSize() > 0);
		check("Messages should have the same size", msg1.getSize() == msg2.getSize());
	}


	private void test_get_field_iterator()
		throws Exception
	{
		Log.info("Test getFieldIterator()");

		MessageFactory msgFactory = MessageFactory.create();

		Message msg = msgFactory.createMessage("HB");

		MessageFactory.destroy(msgFactory);

		MessageFieldIterator iter = msg.getFieldIterator();
		check("Got the field iterator", true);
	}
}


class T005_MessageValidator extends MessageValidator
{
	private int iter = 0;

	public T005_MessageValidator() {}

	public Status validateMessage(Message msg)
	{
		Status status = new Status();

		if (iter > 0)
		{
			status = new Status(StatusClassification.MSG_ERROR, StatusCode.MESSAGE_FAILED_VALIDATION, "Field is missing");
		}

		++iter;

		return status;
	}

	public void reset()
	{
		iter = 0;
	}
}
