import gov.nasa.gsfc.gmsec.api5.Status;
import gov.nasa.gsfc.gmsec.api5.StatusClassification;
import gov.nasa.gsfc.gmsec.api5.StatusCode;
import gov.nasa.gsfc.gmsec.api5.GmsecException;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T001_Status extends TestCase
{
	public static void main(String[] args)
	{
		T001_Status test = new T001_Status();
		test.setDescription("Test Status");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_constructor_1();
			test_constructor_2();
			test_exception_constructor();
			test_copy_constructor();
			test_get();
			test_set();
			test_setClass();
			test_setCode();
			test_setReason();
			test_setCustomCode();
			test_reset();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_constructor_1()
		throws Exception
	{
		Log.info("Test constructor (no args)");

		String reason1 = "";

		Status s1 = new Status();

		check("Unexpected error", false == s1.hasError());
		check("Unexpected classification", s1.getClassification() == StatusClassification.NO_ERROR);
		check("Unexpected code", s1.getCode() == StatusCode.NO_ERROR_CODE);
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(reason1));
	}


	private void test_constructor_2()
		throws Exception
	{
		Log.info("Test constructor (3-4 args)");

		String reason = "Some worthy status";

		Status s1 = new Status(3, 4, reason);

		check("Unexpected error", true == s1.hasError());
		check("Unexpected classification", 3 == s1.getClassification());
		check("Unexpected code", 4 == s1.getCode());
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(reason));

		Status s2 = new Status(6, 7, reason, 8);

		check("Unexpected error", true == s2.hasError());
		check("Unexpected classification", 6 == s2.getClassification());
		check("Unexpected code", 7 == s2.getCode());
		check("Unexpected custom code", 8 == s2.getCustomCode());
		check("Unexpected reason", s2.getReason().equals(reason));
	}


	private void test_exception_constructor()
		throws Exception
	{
		Log.info("Test constructor (GmsecException arg)");

		String reason = "Some worthy exception";

		GmsecException e1 = new GmsecException(1, 2, 3, reason);

		Status s1 = new Status(e1);

		check("Unexpected error", true == s1.hasError());
		check("Unexpected classification", 1 == s1.getClassification());
		check("Unexpected code", 2 == s1.getCode());
		check("Unexpected custom code", 3 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(reason));
	}


	private void test_copy_constructor()
		throws Exception
	{
		Log.info("Test copy-constructor");

		String reason = "Some worthy exception";

		Status s1 = new Status(1, 2, reason, 3);
		Status s2 = new Status(s1);

		check("Unexpected error", true == s2.hasError());
		check("Unexpected classification", s1.getClassification() == s2.getClassification());
		check("Unexpected code", s1.getCode() == s2.getCode());
		check("Unexpected custom code", s1.getCustomCode() == s2.getCustomCode());
		check("Unexpected reason", s2.getReason().equals(reason));
	}


	private void test_get()
		throws Exception
	{
		Log.info("Test get()");

		String reason = "Some worthy exception";

		Status s1 = new Status(1, 2, reason, 3);

		StringBuilder get = new StringBuilder();
		get.append("[1,2,3] : ").append(reason);

		check("Unexpected get() string", s1.get().equals(get.toString()));
	}


	private void test_set()
		throws Exception
	{
		Log.info("Test set()");

		String reason = "Some worthy exception";

		Status s1 = new Status();
		s1.set(1, 2, reason);
		check("Unexpected error", true == s1.hasError());
		check("Unexpected classification", 1 == s1.getClassification());
		check("Unexpected code", 2 == s1.getCode());
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(reason));

		Status s2 = new Status(1, 2, "foobar", 6);
		s2.set(1, 2, reason, 3);
		check("Unexpected error", true == s2.hasError());
		check("Unexpected classification", 1 == s2.getClassification());
		check("Unexpected code", 2 == s2.getCode());
		check("Unexpected custom code", 3 == s2.getCustomCode());
		check("Unexpected reason", s2.getReason().equals(reason));
	}


	private void test_setClass()
		throws Exception
	{
		Log.info("Test setClass()");

		Status s1 = new Status();
		s1.setClassification(0);
		check("Unexpected error", false == s1.hasError());
		check("Unexpected classification", 0 == s1.getClassification());
		check("Unexpected code", 0 == s1.getCode());
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(""));

		Status s2 = new Status();
		s2.setClassification(1);
		check("Unexpected error", true == s2.hasError());
		check("Unexpected classification", 1 == s2.getClassification());
		check("Unexpected code", 0 == s2.getCode());
		check("Unexpected custom code", 0 == s2.getCustomCode());
		check("Unexpected reason", s2.getReason().equals(""));
	}


	private void test_setCode()
		throws Exception
	{
		Log.info("Test setCode()");

		Status s1 = new Status();
		s1.setCode(1);
		check("Unexpected error", false == s1.hasError());
		check("Unexpected classification", 0 == s1.getClassification());
		check("Unexpected code", 1 == s1.getCode());
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(""));
	}


	private void test_setReason()
		throws Exception
	{
		Log.info("Test setReason()");

		String reason = "Some worthy exception";

		Status s1 = new Status();
		s1.setReason(reason);
		check("Unexpected error", false == s1.hasError());
		check("Unexpected classification", 0 == s1.getClassification());
		check("Unexpected code", 0 == s1.getCode());
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(reason));
	}


	private void test_setCustomCode()
		throws Exception
	{
		Log.info("Test setCustomCode()");

		Status s1 = new Status();
		s1.setCustomCode(1);
		check("Unexpected error", false == s1.hasError());
		check("Unexpected classification", 0 == s1.getClassification());
		check("Unexpected code", 0 == s1.getCode());
		check("Unexpected custom code", 1 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(""));
	}


	private void test_reset()
		throws Exception
	{
		Log.info("Test reset()");

		Status s1 = new Status(1, 2, "foobar", 3);
		s1.reset();
		check("Unexpected error", false == s1.hasError());
		check("Unexpected classification", 0 == s1.getClassification());
		check("Unexpected code", 0 == s1.getCode());
		check("Unexpected custom code", 0 == s1.getCustomCode());
		check("Unexpected reason", s1.getReason().equals(""));
	}
}
