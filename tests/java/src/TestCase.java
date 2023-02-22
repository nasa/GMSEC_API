import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Connection;
import gov.nasa.gsfc.gmsec.api5.Message;
import gov.nasa.gsfc.gmsec.api5.MessageFactory;
import gov.nasa.gsfc.gmsec.api5.Status;

import gov.nasa.gsfc.gmsec.api5.field.*;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.io.FileWriter;
import java.lang.management.ManagementFactory;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;


public class TestCase
{
	private int nOk;
	private int nBad;
	private String prefix;
	private String signal;
	private String reason;
	private String exitReason;
	private String description = new String();

	private Config config;

	private List<Field> standardFields;

    private void initStandardFields()
    {
		standardFields = new ArrayList<Field>();

		standardFields.add(new StringField("MISSION-ID", "MY-MISSION", true));
        standardFields.add(new StringField("CONSTELLATION-ID", "MY-CONSTELLATION", true));
        standardFields.add(new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true));
        standardFields.add(new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true));
        standardFields.add(new StringField("FACILITY", "MY-FACILITY", true));
        standardFields.add(new StringField("DOMAIN1", "MY-DOMAIN-1", true));
        standardFields.add(new StringField("DOMAIN2", "MY-DOMAIN-2", true));
        standardFields.add(new StringField("COMPONENT", "MY-COMPONENT", true));
    }

	Config getConfig()
	{
		return config;
	}

	List<Field> getStandardFields()
	{
		return standardFields;
	}

    void setStandardFields(MessageFactory msgFactory)
	{
		msgFactory.setStandardFields(standardFields);
	}

	void verifyStandardFields(Message msg)
		throws Exception
	{
		for (Field field : standardFields)
		{
			require("Message is missing standard field", msg.hasField(field.getName()));
			check("Message has unexpected standard field type", msg.getField(field.getName()).getType() == field.getType());
			check("Message has unexpected standard field value", msg.getField(field.getName()).getStringValue().equals(field.getStringValue()));
		}
	}

	boolean check(String what, boolean flag)
	{
		return _check(what, flag);
	}

	boolean check(String what, Status status)
	{
		return _check(what + ": " + status.get(), !status.hasError());
	}

	void require(String what, boolean flag) throws TestException
	{
		if (!_check(what, flag))
			throw new TestException(what);
	}

	void require(String what, Status status) throws TestException
	{
		if (!_check(what, !status.hasError()))
			throw new TestException(what + ":" + status.get());
	}

	void millisleep(int ms)
	{
		try { Thread.sleep(ms); } catch (InterruptedException ie) {}
	}

	String getSubject()
	{
		return prefix;
	}

	String getSubject(String tag)
	{
		return prefix + "." + tag.toUpperCase();
	}

	String getUniqueComponent()
	{
		return prefix.replace(".", "-");
	}

	String getDataFile(String file)
	{
		String path = null;

		try
		{
			String addonDir = getConfig().getValue("addons");
			if (addonDir != null)
			{
				path = addonDir + "/" + file;
			}
			else
			{
				//path = "src/addons/" + file;
				path = "../../GMSEC_API-5.x/tests/java/src/addons/" + file;
			}
		}
		catch (IllegalArgumentException e)
		{
			Log.error(e.toString());
		}

		return path;
	}

	private boolean _check(String what, boolean flag)
	{
		if (flag)
		{
			++nOk;
		}
		else
		{
			++nBad;
			Log.warning("[error] " + what);
		}
		return flag;
	}

	void forceExitAtCleanup(String reason)
	{
		exitReason = reason;
	}

	void run(String[] args)
	{
		// Set up shutdown hook so that finish() is called
		// regardless of whether the program terminates under
		// its own volition or due to external forces.
		Runtime.getRuntime().addShutdownHook(new Thread()
		{
			@Override
			public void run()
			{
				Connection.shutdownAllMiddlewares();
				finish();
			}
		});

		initialize(args);

		Log.info("Testing " + getClass() + "...");

		try
		{
			testCase();
		}
		catch (UnsatisfiedLinkError e)
		{
			check("exception:" + e.toString(), false);
			++nBad;
		}
		catch (Exception e)
		{
			check("exception:" + e.toString(), false);
		}
		finally
		{
			if (exitReason != null)
			{
				Log.info("Forcing exit due to: " + exitReason);
				System.exit(0);
			}
		}
	}

	private void initialize(String[] args)
	{
		for (String s : args)
		{
			if (s.charAt(0) == '-' && s.startsWith("-signal="))
			{
				signal = s.substring(8);
			}
		}

		StringBuilder sb = new StringBuilder(200);
		sb.append("NIGHTRUN.JAVA");

		String uid = ManagementFactory.getRuntimeMXBean().getName();
		// replace all invalid subject characters with '-'
		uid = uid.replaceAll("[^-A-Za-z0-9]", "_");
		sb.append('.').append(uid);

		String testNumber = getClass().getName().substring(0, getClass().getName().indexOf('_'));
		sb.append(".").append(testNumber);
		prefix = sb.toString().toUpperCase();

		config = new Config(args);
		config.addValue("LOGLEVEL", config.getValue("LOGLEVEL", "INFO"));
		config.addValue("LOGFILE",  config.getValue("LOGFILE", "STDERR"));

		initStandardFields();
	}

	void testCase() throws Exception
	{
		check("this is not a complete test case", false);
	}

	void excuse(String s)
	{
		reason = s;
	}

	void comment(String comment)
	{
		Log.info("[comment] " + comment);
	}

	void setDescription(String desc)
	{
		description = desc;
		start();
	}

	String getDescription()
	{
		return description.length() > 0 ? description : "Unknown Java Unit Test";
	}

	LocalDateTime buildDateTime(String gmsecTimeString) throws TestException
	{
		if (gmsecTimeString == null)
		{
			throw new TestException("GMSEC time string cannot be null");
		}

		int year	= Integer.parseInt(gmsecTimeString.substring(0, 4));
		int days	= Integer.parseInt(gmsecTimeString.substring(5, 8));
		int hours	= Integer.parseInt(gmsecTimeString.substring(9, 11));
		int minutes	= Integer.parseInt(gmsecTimeString.substring(12, 14));
		int seconds	= Integer.parseInt(gmsecTimeString.substring(15, 17));
		int millis	= Integer.parseInt(gmsecTimeString.substring(18, 21));

		//return LocalDateTime().plusYears(years).plusDays(days).plusHours(hours).plusMinutes(minutes).plusSeconds(seconds);
		return LocalDateTime.of(year, 1, 1, hours, minutes, seconds).plusDays(days - 1).plusNanos(millis*10);
	}

	private void start()
	{
 		//
		// Same as finish(), but only put out initial signal file with
		// a valid description and that's all.  Meant to be called from
		// setDescription() so presumably it will be called at the start
		// of every unit test.
		//
		StringBuilder sb = new StringBuilder(1000);

		sb.append("DESCRIPTION=").append(getDescription());

		if (signal != null)
		{
			FileWriter writer = null;
			try
			{
				writer = new FileWriter(signal);
				writer.write(sb.toString());
				writer.close();
			}
			catch (java.io.IOException e)
			{
				e.printStackTrace();
			}
		}
	}

	private void finish()
	{
		StringBuilder sb = new StringBuilder(1000);
		String result = "FAIL";
		if (nBad == 0)
		{
			if (nOk > 0)
			{
				result = "PASS";
			}
		}
		if (reason != null)
		{
			result = "EXCUSE";
		}

		sb.append("RESULT=").append(result)
			.append("\nDESCRIPTION=").append(getDescription())
			.append("\nN_OK=").append(nOk)
			.append("\nN_BAD=").append(nBad)
			.append("\nPREFIX=").append(prefix)
			;

		if (reason != null)
		{
			sb.append("\nEXCUSE=").append(reason);
		}

		String summary = sb.toString();
		Log.info("summary:\n" + summary);

		if (signal != null)
		{
			FileWriter writer = null;
			try
			{
				writer = new FileWriter(signal);
				writer.write(summary);
				writer.close();
			}
			catch (java.io.IOException e)
			{
				e.printStackTrace();
			}
		}
	}
}


class TestException extends Exception
{
	TestException(String s)
	{
		super(s);
	}
}
