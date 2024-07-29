import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.MessageFieldIterator;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.I32Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T006_MessageFieldIterator extends TestCase
{
	public static void main(String[] args)
	{
		T006_MessageFieldIterator test = new T006_MessageFieldIterator();
		test.setDescription("MessageFieldIterator");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_all_fields();
			test_header_fields();
			test_non_header_fields();
			test_reset();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_all_fields()
		throws Exception
	{
		Log.info("Test ALL_FIELDS");

		Message msg = TestMessage.instance().getMessage();

		MessageFieldIterator iter = msg.getFieldIterator( MessageFieldIterator.Selector.ALL_FIELDS );

		int fieldsFound = 0;

		while (iter.hasNext())
		{
			Field field = iter.next();

			++fieldsFound;

			switch (fieldsFound)
			{
			case 1:
				check("Unexpected field type 1", field.getType() == Field.Type.STRING);
				check("Unexpected field name 1", field.getName().equals("FIELD-1"));
				check("Unexpected field value 1", field.getStringValue().equals("1"));
				break;

			case 2:
				check("Unexpected field type 2", field.getType() == Field.Type.STRING);
				check("Unexpected field name 2", field.getName().equals("FIELD-2"));
				check("Unexpected field value 2", field.getStringValue().equals("2"));
				break;

			case 3:
				check("Unexpected field type 3", field.getType() == Field.Type.I32);
				check("Unexpected field name 3", field.getName().equals("FIELD-3"));
				check("Unexpected field value 3", field.getI32Value() == 3);
				break;
			}
		}

		check("Unexpected field count", fieldsFound == 3);

		// Off-nominal test
		try
		{
			// iterator already at end
			iter.next();
			check("Expected a NoSuchElementException", false);
		}
		catch (java.util.NoSuchElementException e)
		{
			check(e.getMessage(), true);
		}
	}


	private void test_header_fields()
		throws Exception
	{
		Log.info("Test HEADER_FIELDS");

		Message msg = TestMessage.instance().getMessage();

		MessageFieldIterator iter = msg.getFieldIterator( MessageFieldIterator.Selector.HEADER_FIELDS );

		int fieldsFound = 0;

		while (iter.hasNext())
		{
			Field field = iter.next();

			++fieldsFound;

			switch (fieldsFound)
			{
			case 1:
				check("Unexpected field type 1", field.getType() == Field.Type.STRING);
				check("Unexpected field name 1", field.getName().equals("FIELD-1"));
				check("Unexpected field value 1", field.getStringValue().equals("1"));
				break;

			case 2:
				check("Unexpected field type 2", field.getType() == Field.Type.STRING);
				check("Unexpected field name 2", field.getName().equals("FIELD-2"));
				check("Unexpected field value 2", field.getStringValue().equals("2"));
				break;

			case 3:
				check("Unexpected field", false);
				break;
			}
		}

		check("Unexpected field count", fieldsFound == 2);

		// Off-nominal test
		try
		{
			// iterator already at end
			iter.next();
			check("Expected a NoSuchElementException", false);
		}
		catch (java.util.NoSuchElementException e)
		{
			check(e.getMessage(), true);
		}
	}


	private void test_non_header_fields()
		throws Exception
	{
		Log.info("Test NON_HEADER_FIELDS");

		Message msg = TestMessage.instance().getMessage();

		MessageFieldIterator iter = msg.getFieldIterator( MessageFieldIterator.Selector.NON_HEADER_FIELDS );

		int fieldsFound = 0;

		while (iter.hasNext())
		{
			Field field = iter.next();

			++fieldsFound;

			switch (fieldsFound)
			{
			case 1:
				check("Unexpected field type 3", field.getType() == Field.Type.I32);
				check("Unexpected field name 3", field.getName().equals("FIELD-3"));
				check("Unexpected field value 3", field.getI32Value() == 3);
				break;

			default:
				check("Unexpected field", false);
				break;
			}
		}

		check("Unexpected field count", fieldsFound == 1);

		// Off-nominal test
		try
		{
			// iterator already at end
			iter.next();
			check("Expected a NoSuchElementException", false);
		}
		catch (java.util.NoSuchElementException e)
		{
			check(e.getMessage(), true);
		}
	}


	private void test_reset()
		throws Exception
	{
		Log.info("Test reset()");

		Message msg = TestMessage.instance().getMessage();

		MessageFieldIterator iter = msg.getFieldIterator();

		int fieldsFound = 0;

		while (iter.hasNext())
		{
			iter.next();
			++fieldsFound;
		}

		check("Unexpected field count", fieldsFound == 3);

		check("Should no more field(s) to iterate over", iter.hasNext() == false);

		iter.reset();

		check("Should have field(s) to iterator over", iter.hasNext());

		fieldsFound = 0;

		while (iter.hasNext())
		{
			iter.next();
			++fieldsFound;
		}

		check("Unexpected field count", fieldsFound == 3);
	}
}


class TestMessage
{
	private static TestMessage s_instance = null;
	private Message            m_msg      = null;

	public static TestMessage instance()
		throws GmsecException
	{
		if (s_instance == null) {
			s_instance = new TestMessage();
		}

		return s_instance;
	}

	public Message getMessage()
	{
		return m_msg;
	}

	private TestMessage()
		throws GmsecException
	{
		m_msg = new Message();

		StringField field1 = new StringField("FIELD-1", "1", true);
		StringField field2 = new StringField("FIELD-2", "2", true);
		I32Field    field3 = new I32Field("FIELD-3", 3);

		m_msg.addField(field1);
		m_msg.addField(field2);
		m_msg.addField(field3);
	}
}
