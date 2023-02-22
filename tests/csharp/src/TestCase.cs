namespace GMSEC.API5.Testing.Common
{
	using GMSEC.API5;
	using System;
	using System.Diagnostics;
	using System.IO;
    using System.Linq;
    using System.Text;
    using System.Collections.Generic;

	public class TestCase
	{
		private int nOk;
		private int nBad;
		private string directory;
		private string prefix;
		private string signal;
		private string reason;
		private string description;

		private Config config;

		private FieldList standardFields;

		private void InitStandardFields()
		{
			standardFields = new FieldList()
			{
				new StringField("MISSION-ID", "MY-MISSION", true),
				new StringField("CONSTELLATION-ID", "MY-CONSTELLATION", true),
				new StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", true),
				new StringField("SAT-ID-LOGICAL", "MY-SAT-ID", true),
				new StringField("FACILITY", "MY-FACILITY", true),
				new StringField("DOMAIN1", "MY-DOMAIN-1", true),
				new StringField("DOMAIN2", "MY-DOMAIN-2", true),
				new StringField("COMPONENT", "MY-COMPONENT", true)
			};
		}

		public Config GetConfig()
		{
			return config;
		}

		public FieldList GetStandardFields()
		{
			return standardFields;
		}

		public void SetStandardFields(MessageFactory msgFactory)
		{
			msgFactory.SetStandardFields(standardFields);
		}

		public void VerifyStandardFields(Message msg)
		{
			foreach (Field field in standardFields)
			{
				Require("Message is missing standardField", msg.HasField(field.GetName()));
				Check("Message has unexpected standard field type", msg.GetField(field.GetName()).GetType() == field.GetType());
				Check("Message has unexpected standard field value", msg.GetField(field.GetName()).GetStringValue() == field.GetStringValue());
			}
		}

		public Boolean Check(string what, Boolean flag)
		{
			return _check(what, flag);
		}

		public Boolean Check(string what, Status status)
		{
			return _check(what + ": " + status.Get(), !status.HasError());
		}

		public void Require(string what, Boolean flag)
		{
			if (!_check(what, flag))
				throw new TestException(what);
		}

		public void Require(string what, Status status)
		{
			if (!_check(what, !status.HasError()))
				throw new TestException(what + ":" + status.Get());
		}

		public string GetSubject()
		{
			return prefix;
		}

		public string GetSubject(string tag)
		{
			return prefix + "." + tag.ToUpper();
		}

		public string GetUniqueComponent()
		{
			return prefix.Replace(".", "-");
		}

		private Boolean _check(string what, Boolean flag)
		{
			if (flag) {
				++nOk;
			}
			else {
				++nBad;
                Log.Error(what);
			}
			return flag;
		}

		private string GetDescription()
		{
			return description.Length > 0 ? description : "Unknown C# Unit Test";
		}

		protected void SetDescription(string desc)
		{
			description = desc;
			Start();
		}

		public void Run(string[] args)
		{
			Initialize(args);

			Log.Info("Testing " + GetType().Name + "...");

			try
			{
				Run();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
			catch (TestException e)
			{
				//Console.WriteLine("Exception:" + e.ToString());
				e.ToString();
			}
			finally
			{
				Finish();
			}
		}

		private void Initialize(string[] args)
		{
			// Ascertain if we're given a signal file
			foreach (string s in args)
			{
				if (s[0] == '-')
				{
					if (s.StartsWith("-signal="))
					{
						signal = s.Substring(8);
					}
				}
			}

			// Ascertain working directory
			directory = ".";

			string app = Environment.GetCommandLineArgs()[0];
			int pos = app.LastIndexOf('/');
			if (pos != -1)
			{
				directory = app.Substring(0, pos);
			}
			else
			{
				pos = app.LastIndexOf('\\');
				if (pos != -1)
				{
					directory = app.Substring(0, pos);
				}
			}

			// Ascertain prefix
			string procId     = "" + Process.GetCurrentProcess().Id;
			string machName   = System.Environment.MachineName;
			string testNumber = GetType().Name.Substring(0, GetType().Name.IndexOf('_'));

			StringBuilder sb = new StringBuilder(200);
			sb.Append("NIGHTRUN.CS");
			sb.Append(".").Append(procId.ToUpper()).Append("_").Append(machName.ToUpper());
			sb.Append(".").Append(testNumber);

			prefix = sb.ToString().ToUpper();

			// Ascertain log level and file
			config = new Config(args);
			config.AddValue("LOGLEVEL", config.GetValue("LOGLEVEL", "INFO"));
			config.AddValue("LOGFILE",  config.GetValue("LOGFILE", "STDERR"));

			// Initialize standard fields
			InitStandardFields();
		}

		public virtual void Run()
		{
			Check("this is not a complete test case", false);
		}

		public void Excuse(string s)
		{
			reason = s;
		}

		public string GetDataFile(string file)
		{
			string addonDir = GetConfig().GetValue("addons");

			if (addonDir != null)
			{
				return addonDir + "/" + file;
			}

			return directory + "/../src/addons/" + file;
		}

		public void Comment(string comment)
        {
			Log.Info("[comment] " + comment);
		}

		public bool UsingLinux()
		{
			int p = (int) Environment.OSVersion.Platform;
			return (p == 4) || (p == 6) || (p == 128);
		}

		private void Start()
		{
	 		//
			// Same as Finish(), but only put out initial signal file with
			// a valid description and that's all.  Meant to be called from
			// SetDescription() so presumably it will be called at the start
			// of every unit test.
			//

			StringBuilder sb = new StringBuilder(1000);

			sb.Append("DESCRIPTION=").Append(GetDescription());

			if (signal != null)
			{
				StreamWriter writer = null;
				try {
					writer = new StreamWriter(signal);
					writer.WriteLine(sb);
					writer.Close();
				}
				catch (IOException) {
					Console.WriteLine(System.Environment.StackTrace);
				}
			}
        }

		private void Finish()
		{
			StringBuilder sb = new StringBuilder(1000);
			string result = "FAIL";
			if (nBad == 0)
			{
				if (nOk > 0)
					result = "PASS";
			}
			if (reason != null)
				result = "EXCUSE";

			sb.Append("RESULT=").Append(result)
				.Append("\nDESCRIPTION=").Append(GetDescription())
				.Append("\nN_OK=").Append(nOk)
				.Append("\nN_BAD=").Append(nBad)
				.Append("\nPREFIX=").Append(prefix)
				;

			if (reason != null)
				sb.Append("\nEXCUSE=").Append(reason);

            string summary = sb.ToString();
            Log.Info("Summary:\n" + summary);

			if (signal != null)
			{
				StreamWriter writer = null;
				try {
					writer = new StreamWriter(signal);
					writer.WriteLine(summary);
					writer.Close();
				}
				catch (IOException) {
					Console.WriteLine(System.Environment.StackTrace);
				}
			}

			// Prevent further calls to (Default) LogHandler.OnMessage().
			// Certain unit tests fail due to late garbage collection.
			// Some GMSEC API classes output log messages when being destructed.
			GetConfig().AddValue("loglevel", "none");
        }
			
		public static DateTime BuildDateTime(String gmsecTimeString)
		{
			if (gmsecTimeString == null)
			{
				throw new ArgumentException("GMSEC time string cannot be null");
			}

			int year    = Convert.ToInt32(gmsecTimeString.Substring(0, 4));
			int days    = Convert.ToInt32(gmsecTimeString.Substring(5, 3));
			int hours   = Convert.ToInt32(gmsecTimeString.Substring(9, 2));
			int minutes = Convert.ToInt32(gmsecTimeString.Substring(12, 2));
			int seconds = Convert.ToInt32(gmsecTimeString.Substring(15, 2));
			int millis  = Convert.ToInt32(gmsecTimeString.Substring(18, 3));

			return new DateTime(year, 1, 1, hours, minutes, seconds).AddDays(days - 1).AddMilliseconds(millis);
		}
	}

	class TestException : System.Exception
	{
		public TestException(string s) : base(s) {}
	}
}
