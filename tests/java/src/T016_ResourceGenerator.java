import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Connection;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.ResourceGenerator;
import gov.nasa.gsfc.gmsec.api5.U16;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.StringField;
import gov.nasa.gsfc.gmsec.api5.field.U16Field;

import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.List;
import java.util.ArrayList;


public class T016_ResourceGenerator extends TestCase
{
	public static void main(String[] args)
	{
		T016_ResourceGenerator test = new T016_ResourceGenerator();
		test.setDescription("Test ResourceGenerator");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_create();
			test_start();
			test_stop();
			test_set_field();
			test_create_resource_message();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_create()
		throws Exception
	{
		Log.info("Test create()");

		List<Field> emptyFieldList = new ArrayList<Field>();
		List<Field> standardFields = getStandardFields();

		//o Nominal test (empty, populated, and null list of fields)
		{
			ResourceGenerator rsrcgen1 = ResourceGenerator.create(getConfig(), 5, 1, 10, null);
			ResourceGenerator rsrcgen2 = ResourceGenerator.create(getConfig(), 5, 1, 10, emptyFieldList);
			ResourceGenerator rsrcgen3 = ResourceGenerator.create(getConfig(), 5, 1, 10, standardFields);

			check("ResourceGenerator should not be running", rsrcgen1.isRunning() == false);
			check("ResourceGenerator should not be running", rsrcgen2.isRunning() == false);
			check("ResourceGenerator should not be running", rsrcgen3.isRunning() == false);

			ResourceGenerator.destroy(rsrcgen1);
			ResourceGenerator.destroy(rsrcgen2);
			ResourceGenerator.destroy(rsrcgen3);
		}
	}


	private void test_start()
		throws Exception
	{
		Log.info("Test start()");

		Config config  = getConfig();
		int    pubRate = 1;

		ResourceGenerator rsrcgen = ResourceGenerator.create(config, pubRate, 1, 10, getStandardFields());

		/* To ensure this resource message is unique, we set the COMPONENT field */
		rsrcgen.setField( new StringField("COMPONENT", getUniqueComponent()) );

		rsrcgen.start();

		check("ResourceGenerator should be running", rsrcgen.isRunning());

		verify_resource_message(config, pubRate);

		rsrcgen.stop();

		/* Allow for RSRC-gen thread to stop */
		TimeUtil.millisleep(2000);

		ResourceGenerator.destroy(rsrcgen);
	}


	private void test_stop()
		throws Exception
	{
		Log.info("Test stop()");

		Config config  = getConfig();
		int    pubRate = 1;

		ResourceGenerator rsrcgen = ResourceGenerator.create(config, pubRate, 1, 10, getStandardFields());

		/* To ensure this resource message is unique, we set the COMPONENT field */
		rsrcgen.setField( new StringField("COMPONENT", getUniqueComponent()) );

		rsrcgen.start();

		check("ResourceGenerator should be running", rsrcgen.isRunning());

		rsrcgen.stop();

		check("ResourceGenerator should not be running", rsrcgen.isRunning() == false);

		/* Allow for RSRC-gen thread to stop */
		TimeUtil.millisleep(2000);

		Connection conn = null;

		try
		{
			conn = Connection.create(config);
			conn.connect();
			conn.subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + getUniqueComponent());

			for (int i = 0; i < 3; ++i)
			{
				Message rsrcMsg = conn.receive(5000);

				if (rsrcMsg != null)
				{
					check("Unexpectedly received a Resource Message", false);
				}
			}
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		ResourceGenerator.destroy(rsrcgen);
	}


	private void test_set_field()
		throws Exception
	{
		Config config = new Config(getConfig());     // make a copy

		int pubRate = 1;

		//o Nominal tests
		ResourceGenerator rsrcgen = ResourceGenerator.create(config, pubRate, 1, 10, getStandardFields());

		/* To ensure this resource message is unique, we set the COMPONENT field */
		rsrcgen.setField( new StringField("COMPONENT", getUniqueComponent()) );

		rsrcgen.start();

		TimeUtil.millisleep(2000);

		Log.info("Test setField() with a Field");
		int newPubRate = 2;
		rsrcgen.setField(new U16Field("PUB-RATE", new U16(newPubRate)));
		verify_resource_message(config, newPubRate);

		rsrcgen.stop();

		/* Allow for RSRC-gen thread to stop */
		TimeUtil.millisleep(2000);

		ResourceGenerator.destroy(rsrcgen);

		//o Off-nominal tests
		config.addValue("gmsec-msg-content-validate", "true");

		ResourceGenerator rsrcgen2 = ResourceGenerator.create(config, pubRate, 1, 10, getStandardFields());

		// Add bogus field using a Field
		try {
			rsrcgen2.setField(new U16Field("BOGUS-FIELD", new U16(2)));

			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message Validation Failed"));
		}

		ResourceGenerator.destroy(rsrcgen2);
	}


	private void test_create_resource_message()
		throws Exception
	{
		Log.info("Test createResourceMessage()...");

		MessageFactory factory         = null;
		int            sampleInterval  = 1;
		int            averageInterval = 10;

		try
		{
			factory = MessageFactory.create( getConfig() );

			factory.setStandardFields( getStandardFields() );

			Message rsrcMsg = ResourceGenerator.createResourceMessage( factory, sampleInterval, averageInterval );

			check("Unexpected MESSAGE-TYPE", rsrcMsg.getStringValue("MESSAGE-TYPE").equals("MSG"));
			check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.getStringValue("MESSAGE-SUBTYPE").equals("RSRC"));
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (factory != null)
			{
				MessageFactory.destroy( factory );
			}
		}
	}


	private void verify_resource_message(Config config, int expectedPubRate)
		throws Exception
	{
		Connection conn = null;

		try
		{
			conn = Connection.create(config);
			conn.connect();
			conn.subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + getUniqueComponent() + ".+");

			double t1 = 0, t2;

			for (int i = 0; i < 7; ++i)
			{
				Message rsrcMsg = conn.receive(5000);

				// ignore the first few incoming messages (if any)
				if (i < 3)
				{
					continue;
				}

				if (rsrcMsg != null)
				{
					if (t1 == 0)
					{
						t1 = TimeUtil.getCurrentTime_s();
					}
					else
					{
						t2 = TimeUtil.getCurrentTime_s();

						double delta = t2 - t1;
						if (delta < expectedPubRate)
						{
							delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
						}

						Log.info("Expected rate is: " + expectedPubRate + ", delta is: " + delta);

						check("Unexpected publish rate", expectedPubRate == (int) delta);

						t1 = t2;
					}

					check("Unexpected MESSAGE-TYPE", rsrcMsg.getStringValue("MESSAGE-TYPE").equals("MSG"));
					check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.getStringValue("MESSAGE-SUBTYPE").equals("RSRC"));
					check("Unexpected PUB-RATE", rsrcMsg.getI32Value("PUB-RATE") == expectedPubRate);
				}
				else
				{
					check("Failed to received Resource Message", false);
				}
			}

			conn.disconnect();
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}
	}
}
