namespace T004
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T004_GmsecException : TestCase
	{
		static void Main(string[] args)
		{
			T004_GmsecException test = new T004_GmsecException();
			test.SetDescription("Test GmsecException");
			test.Run(args);
		}


		public override void Run()
		{
			Test_Constructor_1();
			Test_Constructor_2();
			Test_Constructor_3();
			Test_CopyConstructor();

			//Note: Accessors are tested in each test above.
		}


		private void Test_Constructor_1()
		{
			Log.Info("Test constructor (string arg)");

			string emsg = "Something bad happened";

			GmsecException e1 = new GmsecException("[1,2,3] : " + emsg);

			Check("Unexpected error classification", e1.GetErrorClass() == 1);
			Check("Unexpected error code", e1.GetErrorCode() == 2);
			Check("Unexpected custom code", e1.GetCustomCode() == 3);
			Check("Unexpected error message", e1.GetErrorMessage() == emsg);
			Check("Unexpected exception message", e1.ToString() == "GMSEC.API5.GmsecException: [1,2,3] : " + emsg);
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor (4 args)");

			string emsg = "Something bad happened";

			GmsecException e1 = new GmsecException(1, 2, 3, emsg);

			Check("Unexpected error classification", e1.GetErrorClass() == 1);
			Check("Unexpected error code", e1.GetErrorCode() == 2);
			Check("Unexpected custom code", e1.GetCustomCode() == 3);
			Check("Unexpected error message", e1.GetErrorMessage() == emsg);
			Check("Unexpected exception message", e1.ToString() == "GMSEC.API5.GmsecException: [1,2,3] : " + emsg);
		}


		private void Test_Constructor_3()
		{
			Log.Info("Test constructor (Status arg)");

			string reason = "Some worthy status";

			Status status = new Status(1,2, reason, 3);

			GmsecException e1 = new GmsecException(status);

			Check("Unexpected error classification", e1.GetErrorClass() == 1);
			Check("Unexpected error code", e1.GetErrorCode() == 2);
			Check("Unexpected custom code", e1.GetCustomCode() == 3);
			Check("Unexpected error message", e1.GetErrorMessage() == reason);
			Check("Unexpected exception message", e1.ToString() == "GMSEC.API5.GmsecException: [1,2,3] : " + reason);
		}


		private void Test_CopyConstructor()
		{
			Log.Info("Test copy-constructor");

			String emsg = "Some worthy exception";

			GmsecException e1 = new GmsecException(1, 2, 3, emsg);
			GmsecException e2 = new GmsecException(e1);

			Check("Unexpected error classification", e1.GetErrorClass() == e2.GetErrorClass());
			Check("Unexpected error code", e1.GetErrorCode() == e2.GetErrorCode());
			Check("Unexpected custom code", e1.GetCustomCode() == e2.GetCustomCode());
			Check("Unexpected error message", e1.GetErrorMessage() == e2.GetErrorMessage());
			Check("Unexpected exception message", e1.ToString() == e2.ToString());
		}
	}
}
