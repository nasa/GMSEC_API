import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Connection;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.HeartbeatGenerator;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.U16;

import gov.nasa.gsfc.gmsec.api5.field.Field;
import gov.nasa.gsfc.gmsec.api5.field.U16Field;

import gov.nasa.gsfc.gmsec.api5.util.Log;
import gov.nasa.gsfc.gmsec.api5.util.TimeUtil;

import java.util.List;
import java.util.ArrayList;


public class T011_HeartbeatGenerator extends TestCase
{
	public static void main(String[] args)
	{
		T011_HeartbeatGenerator test = new T011_HeartbeatGenerator();
		test.setDescription("Test HeartbeatGenerator");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_create();   // also invokes destroy()
			test_start();
			test_stop();
			test_change_publish_rate();
			test_set_field();
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

		//o Nominal test
		{
			HeartbeatGenerator hbgen1 = HeartbeatGenerator.create(getConfig(), 5, null);
			HeartbeatGenerator hbgen2 = HeartbeatGenerator.create(getConfig(), 5, emptyFieldList);
			HeartbeatGenerator hbgen3 = HeartbeatGenerator.create(getConfig(), 5, standardFields);

			check("HeartbeatGenerator should not be running", hbgen1.isRunning() == false);
			check("HeartbeatGenerator should not be running", hbgen2.isRunning() == false);
			check("HeartbeatGenerator should not be running", hbgen3.isRunning() == false);

			HeartbeatGenerator.destroy(hbgen1);
			HeartbeatGenerator.destroy(hbgen2);
			HeartbeatGenerator.destroy(hbgen3);
		}

		//o Off-nominal test(s)
		try
		{
			// Bogus middleware
			Config config = new Config(getConfig());
			config.addValue("mw-id", "bogus-mw");
			HeartbeatGenerator.create(config, 5, null);
			check("Expected an exception", false);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), e.getMessage().contains("Unable to load"));
		}
	}


	private void test_start()
		throws Exception
	{
		Log.info("Test start()");

		Config config = new Config(getConfig());

		HeartbeatGenerator hbgen = HeartbeatGenerator.create(config, 1, getStandardFields());

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", getUniqueComponent());

		hbgen.start();

		check("HeartbeatGenerator should be running", hbgen.isRunning());

		verify_heartbeat_message(config, 1);

		hbgen.stop();

		/* Allow time for the HB-gen thread to stop */
		TimeUtil.millisleep(2000);

		HeartbeatGenerator.destroy(hbgen);

		//o Off-nominal tests
		config.addValue("gmsec-msg-content-validate", "true");

		HeartbeatGenerator hbgen2 = HeartbeatGenerator.create(config, 1, getStandardFields());

		// Add bogus field using a Field
		try {
			hbgen2.setField(new U16Field("BOGUS-FIELD", new U16(2)));
			hbgen2.start();
			check("An expection was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message Validation Failed"));
		}

		// Add bogus field using a long value
		try {
			hbgen2.setField("BOGUS-FIELD", (long) 2);
			hbgen2.start();
			check("An expection was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message Validation Failed"));
		}

		// Add bogus field using a double value
		try {
			hbgen2.setField("BOGUS-FIELD", 2.0);
			hbgen2.start();
			check("An expection was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message Validation Failed"));
		}

		// Add bogus field using a string value
		try {
			hbgen2.setField("BOGUS-FIELD", "2");
			hbgen2.start();
			check("An expection was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Message Validation Failed"));
		}

		HeartbeatGenerator.destroy(hbgen2);
	}


	private void test_stop()
		throws Exception
	{
		Log.info("Test stop()");

		Config config = getConfig();

		HeartbeatGenerator hbgen = HeartbeatGenerator.create(config, 1, getStandardFields());

		hbgen.start();

		check("HeartbeatGenerator should be running", hbgen.isRunning());

		hbgen.stop();

		check("HeartbeatGenerator should not be running", hbgen.isRunning() == false);

		/* Allow time for the HB-gen thread to stop */
		TimeUtil.millisleep(2000);

		Connection conn = null;

		try
		{
			conn = Connection.create(config);
			conn.connect();
			conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + getUniqueComponent());

			for (int i = 0; i < 3; ++i)
			{
				Message hbmsg = conn.receive(5000);

				if (hbmsg != null)
				{
					check("Unexpectedly received a Heartbeat Message", false);
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

		HeartbeatGenerator.destroy(hbgen);
	}


	private void test_change_publish_rate()
		throws Exception
	{
		Log.info("Test changePublishRate()");

		Config config = getConfig();

		HeartbeatGenerator hbgen = HeartbeatGenerator.create(config, 1, getStandardFields());

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", getUniqueComponent());

		hbgen.start();

		hbgen.changePublishRate(2);

		verify_heartbeat_message(config, 2);

		Connection conn = null;

		try
		{
			conn = Connection.create(config);
			conn.connect();
			conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + getUniqueComponent());

			TimeUtil.millisleep(1000);

			hbgen.changePublishRate(0);

			TimeUtil.millisleep(1000);

			boolean lastMessage = false;

			for (int i = 0; i < 3 && !lastMessage; ++i)
			{
				Message hbmsg = conn.receive(5000);

				if (hbmsg == null)
				{
					check("Failed to receive Heartbeat Message", false);
				}
				else if (hbmsg.getI32Value("PUB-RATE") != 0)
				{
					Message.destroy(hbmsg);
				}
				else
				{
					lastMessage = true;
					Message.destroy(hbmsg);
				}
			}

			check("Failed to receive a message with PUB-RATE of 0", lastMessage);
		}
		catch (GmsecException e)
		{
			check(e.getMessage(), false);
		}
		finally
		{
			if (conn != null) Connection.destroy(conn);
		}

		hbgen.stop();

		/* Allow time for the HB-gen thread to stop */
		TimeUtil.millisleep(2000);

		HeartbeatGenerator.destroy(hbgen);
	}


	private void test_set_field()
		throws Exception
	{
		Config config = new Config(getConfig());     // make a copy

		// Nominal tests
		HeartbeatGenerator hbgen = HeartbeatGenerator.create(config, 1, getStandardFields());

		/* To ensure this heartbeat message is unique, we set the COMPONENT field */
		hbgen.setField("COMPONENT", getUniqueComponent());

		hbgen.start();

		TimeUtil.millisleep(2000);

		Log.info("Test setField() with a Field");
		hbgen.setField(new U16Field("PUB-RATE", new U16(2)));
		verify_heartbeat_message(config, 2);

		Log.info("Test setField() with a Field Name and long value");
		hbgen.setField("PUB-RATE", (long) 3);
		verify_heartbeat_message(config, 3);

		Log.info("Test setField() with a Field Name and double value");
		hbgen.setField("PUB-RATE", 2.0);
		verify_heartbeat_message(config, 2);

		Log.info("Test setField() with a Field Name and string value");
		hbgen.setField("PUB-RATE", "3");
		verify_heartbeat_message(config, 3);

		hbgen.stop();

		/* Allow time for the HB-gen thread to stop */
		TimeUtil.millisleep(2000);

		HeartbeatGenerator.destroy(hbgen);
	}


	private void verify_heartbeat_message(Config config, int expectedPubRate)
		throws Exception
	{
		Connection conn = null;

		try
		{
			conn = Connection.create(config);
			conn.connect();
			conn.subscribe("C2MS.*.*.*.*.*.MSG.HB." + getUniqueComponent() + ".+");

			double t1 = 0, t2;

			for (int i = 0; i < 7; ++i)
			{
				Message hbmsg = conn.receive(5000);

				// ignore the first few incoming messages (if any)
				if (i < 3)
				{
					continue;
				}

				if (hbmsg != null)
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

					check("Unexpected MESSAGE-TYPE", hbmsg.getStringValue("MESSAGE-TYPE").equals("MSG"));
					check("Unexpected MESSAGE-SUBTYPE", hbmsg.getStringValue("MESSAGE-SUBTYPE").equals("HB"));
					check("Unexpected PUB-RATE", hbmsg.getI32Value("PUB-RATE") == expectedPubRate);
				}
				else
				{
					check("Failed to received Heartbeat Message", false);
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
