namespace T006
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T006_MessageFieldIterator : TestCase
	{
		static void Main(string[] args)
		{
			T006_MessageFieldIterator test = new T006_MessageFieldIterator();
			test.SetDescription("MessageFieldIterator");
			test.Run(args);
		}


		public override void Run()
		{
			Test_AllFields();
			Test_HeaderFields();
			Test_NonHeaderFields();
			Test_Reset();
		}


		private void Test_AllFields()
		{
			Log.Info("Test ALL_FIELDS");

			Message msg = TestMessage.Instance().GetMessage();

			MessageFieldIterator iter = msg.GetFieldIterator( MessageFieldIterator.Selector.ALL_FIELDS );

			int fieldsFound = 0;

			while (iter.HasNext())
			{
				Field field = iter.Next();

				++fieldsFound;

				switch (fieldsFound)
				{
				case 1:
					Check("Unexpected field type 1", field.GetFieldType() == Field.Type.STRING);
					Check("Unexpected field name 1", field.GetName() == "FIELD-1");
					Check("Unexpected field value 1", field.GetStringValue() == "1");
					break;

				case 2:
					Check("Unexpected field type 2", field.GetFieldType() == Field.Type.STRING);
					Check("Unexpected field name 2", field.GetName() == "FIELD-2");
					Check("Unexpected field value 2", field.GetStringValue() == "2");
					break;

				case 3:
					Check("Unexpected field type 3", field.GetFieldType() == Field.Type.I32);
					Check("Unexpected field name 3", field.GetName() == "FIELD-3");
					Check("Unexpected field value 3", field.GetI32Value() == 3);
					break;
				}
			}

			Check("Unexpected field count", fieldsFound == 3);
		}


		private void Test_HeaderFields()
		{
			Log.Info("Test HEADER_FIELDS");

			Message msg = TestMessage.Instance().GetMessage();

			MessageFieldIterator iter = msg.GetFieldIterator( MessageFieldIterator.Selector.HEADER_FIELDS );

			int fieldsFound = 0;

			while (iter.HasNext())
			{
				Field field = iter.Next();

				++fieldsFound;

				switch (fieldsFound)
				{
				case 1:
					Check("Unexpected field type 1", field.GetFieldType() == Field.Type.STRING);
					Check("Unexpected field name 1", field.GetName() == "FIELD-1");
					Check("Unexpected field value 1", field.GetStringValue() == "1");
					break;

				case 2:
					Check("Unexpected field type 2", field.GetFieldType() == Field.Type.STRING);
					Check("Unexpected field name 2", field.GetName() == "FIELD-2");
					Check("Unexpected field value 2", field.GetStringValue() == "2");
					break;

				default:
					Check("Unexpected field", false);
					break;
				}
			}

			Check("Unexpected field count", fieldsFound == 2);
		}


		private void Test_NonHeaderFields()
		{
			Log.Info("Test NON_HEADER_FIELDS");

			Message msg = TestMessage.Instance().GetMessage();

			MessageFieldIterator iter = msg.GetFieldIterator( MessageFieldIterator.Selector.NON_HEADER_FIELDS );

			int fieldsFound = 0;

			while (iter.HasNext())
			{
				Field field = iter.Next();

				++fieldsFound;

				switch (fieldsFound)
				{
				case 1:
					Check("Unexpected field type 3", field.GetFieldType() == Field.Type.I32);
					Check("Unexpected field name 3", field.GetName() == "FIELD-3");
					Check("Unexpected field value 3", field.GetI32Value() == 3);
					break;

				default:
					Check("Unexpected field", false);
					break;
				}
			}

			Check("Unexpected field count", fieldsFound == 1);
		}


		private void Test_Reset()
		{
			Log.Info("Test Reset()");

			Message msg = TestMessage.Instance().GetMessage();

			MessageFieldIterator iter = msg.GetFieldIterator();

			int fieldsFound = 0;

			while (iter.HasNext())
			{
				iter.Next();
				++fieldsFound;
			}

			Check("Unexpected field count", fieldsFound == 3);

			Check("Should no more field(s) to iterate over", iter.HasNext() == false);

			iter.Reset();

			Check("Should have field(s) to iterator over", iter.HasNext());

			fieldsFound = 0;

			while (iter.HasNext())
			{
				iter.Next();
				++fieldsFound;
			}

			Check("Unexpected field count", fieldsFound == 3);
		}
	}


	class TestMessage
	{
		private static TestMessage s_instance = null;
		private Message            m_msg      = null;

		public static TestMessage Instance()
		{
			if (s_instance == null) {
				s_instance = new TestMessage();
			}

			return s_instance;
		}

		public Message GetMessage()
		{
			return m_msg;
		}

		private TestMessage()
		{
			m_msg = new Message();

			StringField field1 = new StringField("FIELD-1", "1", true);
			StringField field2 = new StringField("FIELD-2", "2", true);
			I32Field    field3 = new I32Field("FIELD-3", (int) 3, false);

			m_msg.AddField(field1);
			m_msg.AddField(field2);
			m_msg.AddField(field3);
		}
	}
}
