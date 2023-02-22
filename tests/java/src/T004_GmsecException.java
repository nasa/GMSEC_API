import gov.nasa.gsfc.gmsec.api5.Status;
import gov.nasa.gsfc.gmsec.api5.StatusClassification;
import gov.nasa.gsfc.gmsec.api5.StatusCode;
import gov.nasa.gsfc.gmsec.api5.GmsecException;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T004_GmsecException extends TestCase
{
	public static void main(String[] args)
	{
		T004_GmsecException test = new T004_GmsecException();
		test.setDescription("Test GmsecException");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_constructor_0();
			test_constructor_1();
			test_constructor_2();
			test_constructor_3();
			test_copy_constructor();

			//Note: Accessors are tested in each test above.
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_constructor_0()
	{
		Log.info("Testing GmsecException constructor (string arg)...");

		String emsg1 = "Cannot parse number";
		String emsg2 = "Error: Cannot process input";
		String emsg3 = "[] : Pants on fire!";

		// NOTE: The usage of the following constructor is for internal API
		//       usage only. However since it is declared public, we need to
		//       test it. Ideally the constructor should be declared private,
		//       but then this would affect GMSEC Utils (long story ensues...).

		GmsecException e1 = new GmsecException(emsg1);
		check("Unexpected error string 1", e1.getMessage().equals(emsg1));

		GmsecException e2 = new GmsecException(emsg2);
		check("Unexpected error string 2", e2.getMessage().equals(emsg2));

		GmsecException e3 = new GmsecException(emsg3);
		check("Unexpected error string 3", e3.getMessage().equals(emsg3));
	}

	private void test_constructor_1()
		throws Exception
	{
		Log.info("Test constructor (3 args)");

		String emsg1 = "Something bad happened";

		GmsecException e1 = new GmsecException(1, 2, emsg1);

		check("Unexpected error classification", e1.getErrorClassification() == 1);
		check("Unexpected error code", e1.getErrorCode() == 2);
		check("Unexpected custom code", e1.getCustomCode() == 0);
		check("Unexpected error message", e1.getErrorMessage().equals(emsg1));
		check("Unexpected exception message", e1.getMessage().equals("[1,2,0]: " + emsg1));
		check("Unexpected exception message", e1.toString().equals("[1,2,0]: " + emsg1));
	}


	private void test_constructor_2()
		throws Exception
	{
		Log.info("Test constructor (4 args)");

		String emsg1 = "Something bad happened";

		GmsecException e1 = new GmsecException(1, 2, 3, emsg1);

		check("Unexpected error classification", e1.getErrorClassification() == 1);
		check("Unexpected error code", e1.getErrorCode() == 2);
		check("Unexpected custom code", e1.getCustomCode() == 3);
		check("Unexpected error message", e1.getErrorMessage().equals(emsg1));
		check("Unexpected exception message", e1.getMessage().equals("[1,2,3]: " + emsg1));
		check("Unexpected exception message", e1.toString().equals("[1,2,3]: " + emsg1));
	}


	private void test_constructor_3()
		throws Exception
	{
		Log.info("Test constructor (Status arg)");

		String reason = "Some worthy status";

		Status status = new Status(1,2, reason, 3);

		GmsecException e1 = new GmsecException(status);

		check("Unexpected error classification", e1.getErrorClassification() == 1);
		check("Unexpected error code", e1.getErrorCode() == 2);
		check("Unexpected custom code", e1.getCustomCode() == 3);
		check("Unexpected error message", e1.getErrorMessage().equals(reason));
		check("Unexpected exception message", e1.getMessage().equals("[1,2,3]: " + reason));
		check("Unexpected exception message", e1.toString().equals("[1,2,3]: " + reason));
	}


	private void test_copy_constructor()
		throws Exception
	{
		Log.info("Test copy-constructor");

		String emsg = "Some worthy exception";

		GmsecException e1 = new GmsecException(1, 2, 3, emsg);
		GmsecException e2 = new GmsecException(e1);

		check("Unexpected error classification", e1.getErrorClassification() == e2.getErrorClassification());
		check("Unexpected error code", e1.getErrorCode() == e2.getErrorCode());
		check("Unexpected custom code", e1.getCustomCode() == e2.getCustomCode());
		check("Unexpected error message", e1.getErrorMessage().equals(e2.getErrorMessage()));
		check("Unexpected exception message", e1.getMessage().equals(e2.getMessage()));
		check("Unexpected exception message", e1.toString().equals(e2.toString()));
	}
}
