namespace T001
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T001_Status : TestCase
	{
		static void Main(string[] args)
		{
			T001_Status test = new T001_Status();
			test.SetDescription("Test Status");
			test.Run(args);
		}


    	public override void Run()
		{
			try
			{
				Test_Constructor_1();
				Test_Constructor_2();
				Test_ExceptionConstructor();
				Test_CopyConstructor();
				Test_Get();
				Test_Set();
				Test_SetClass();
				Test_SetCode();
				Test_SetReason();
				Test_SetCustomCode();
				Test_Reset();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}


		private void Test_Constructor_1()
		{
			Log.Info("Test constructor (no args)");

			string reason1 = "";

			Status s1 = new Status();

			Check("Unexpected error", false == s1.HasError());
			Check("Unexpected classification", s1.GetClass() == (int) StatusClass.NO_ERROR_CLASS);
			Check("Unexpected code", s1.GetCode() == (int) StatusCode.NO_ERROR_CODE);
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == reason1);
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor (2-4 args)");

			string reason = "Some worthy status";

			Status s1 = new Status(1, 2);
			Check("Unexpected error", true == s1.HasError());
			Check("Unexpected classification", 1 == s1.GetClass());
			Check("Unexpected code", 2 == s1.GetCode());
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == "");

			Status s2 = new Status(3, 4, reason);
			Check("Unexpected error", true == s2.HasError());
			Check("Unexpected classification", 3 == s2.GetClass());
			Check("Unexpected code", 4 == s2.GetCode());
			Check("Unexpected custom code", 0 == s2.GetCustomCode());
			Check("Unexpected reason", s2.GetReason() == reason);

			Status s3 = new Status(6, 7, reason, 8);
			Check("Unexpected error", true == s3.HasError());
			Check("Unexpected classification", 6 == s3.GetClass());
			Check("Unexpected code", 7 == s3.GetCode());
			Check("Unexpected custom code", 8 == s3.GetCustomCode());
			Check("Unexpected reason", s3.GetReason() == reason);
		}


		private void Test_ExceptionConstructor()
		{
			Log.Info("Test constructor (GmsecException arg)");

			string reason = "Some worthy exception";

			GmsecException e1 = new GmsecException(1, 2, 3, reason);

			Status s1 = new Status(e1);

			Check("Unexpected error", true == s1.HasError());
			Check("Unexpected classification", 1 == s1.GetClass());
			Check("Unexpected code", 2 == s1.GetCode());
			Check("Unexpected custom code", 3 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == reason);
		}


		private void Test_CopyConstructor()
		{
			Log.Info("Test copy-constructor");

			string reason = "Some worthy exception";

			Status s1 = new Status(1, 2, reason, 3);
			Status s2 = new Status(s1);

			Check("Unexpected error", true == s2.HasError());
			Check("Unexpected classification", s1.GetClass() == s2.GetClass());
			Check("Unexpected code", s1.GetCode() == s2.GetCode());
			Check("Unexpected custom code", s1.GetCustomCode() == s2.GetCustomCode());
			Check("Unexpected reason", s2.GetReason() == reason);
		}


		private void Test_Get()
		{
			Log.Info("Test Get()");

			string reason = "Some worthy exception";

			Status s1 = new Status(1, 2, reason, 3);

			StringBuilder get = new StringBuilder(200);
			get.Append("[1,2,3] : ").Append(reason);

			Check("Unexpected Get() string", s1.Get() == get.ToString());
		}


		private void Test_Set()
		{
			Log.Info("Test Set()");

			string reason = "Some worthy exception";

			Status s1 = new Status();
			s1.Set(1, 2, reason);
			Check("Unexpected error", true == s1.HasError());
			Check("Unexpected classification", 1 == s1.GetClass());
			Check("Unexpected code", 2 == s1.GetCode());
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == reason);

			Status s2 = new Status(1, 2, "foobar", 6);
			s2.Set(1, 2, reason, 3);
			Check("Unexpected error", true == s2.HasError());
			Check("Unexpected classification", 1 == s2.GetClass());
			Check("Unexpected code", 2 == s2.GetCode());
			Check("Unexpected custom code", 3 == s2.GetCustomCode());
			Check("Unexpected reason", s2.GetReason() == reason);
		}


		private void Test_SetClass()
		{
			Log.Info("Test SetClass()");

			Status s1 = new Status();
			s1.SetClass(0);
			Check("Unexpected error", false == s1.HasError());
			Check("Unexpected classification", 0 == s1.GetClass());
			Check("Unexpected code", 0 == s1.GetCode());
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == "");

			Status s2 = new Status();
			s2.SetClass(1);
			Check("Unexpected error", true == s2.HasError());
			Check("Unexpected classification", 1 == s2.GetClass());
			Check("Unexpected code", 0 == s2.GetCode());
			Check("Unexpected custom code", 0 == s2.GetCustomCode());
			Check("Unexpected reason", s2.GetReason() == "");
		}


		private void Test_SetCode()
		{
			Log.Info("Test SetCode()");

			Status s1 = new Status();
			s1.SetCode(1);
			Check("Unexpected error", false == s1.HasError());
			Check("Unexpected classification", 0 == s1.GetClass());
			Check("Unexpected code", 1 == s1.GetCode());
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == "");
		}


		private void Test_SetReason()
		{
			Log.Info("Test SetReason()");

			string reason = "Some worthy exception";

			Status s1 = new Status();
			s1.SetReason(reason);
			Check("Unexpected error", false == s1.HasError());
			Check("Unexpected classification", 0 == s1.GetClass());
			Check("Unexpected code", 0 == s1.GetCode());
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == reason);
		}


		private void Test_SetCustomCode()
		{
			Log.Info("Test SetCustomCode()");

			Status s1 = new Status();
			s1.SetCustomCode(1);
			Check("Unexpected error", false == s1.HasError());
			Check("Unexpected classification", 0 == s1.GetClass());
			Check("Unexpected code", 0 == s1.GetCode());
			Check("Unexpected custom code", 1 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == "");
		}


		private void Test_Reset()
		{
			Log.Info("Test Reset()");

			Status s1 = new Status(1, 2, "foobar", 3);
			s1.Reset();
			Check("Unexpected error", false == s1.HasError());
			Check("Unexpected classification", 0 == s1.GetClass());
			Check("Unexpected code", 0 == s1.GetCode());
			Check("Unexpected custom code", 0 == s1.GetCustomCode());
			Check("Unexpected reason", s1.GetReason() == "");
		}
	}
}
