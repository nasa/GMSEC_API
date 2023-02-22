namespace T011
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T011_HeartbeatGenerator : TestCase
	{
		static void Main(string[] args)
		{
			T011_HeartbeatGenerator test = new T011_HeartbeatGenerator();
			test.SetDescription("Test HeartbeatGenerator");
			test.Run(args);
		}


		public override void Run()
		{
			try
			{
				Test_Constructor_1();
				Test_Constructor_2();
				Test_Start();
				Test_Stop();
				Test_ChangePublishRate();
				Test_SetField();
			}
			catch (GmsecException e)
			{
				Require(e.ToString(), false);
			}
		}


		private void Test_Constructor_1()
		{
			Log.Info("Test constructor 1");

			//o Nominal test
			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(GetConfig(), 5))
			{
				Check("HeartbeatGenerator should not be running", hbgen.IsRunning() == false);
			}
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor 2");

			FieldList emptyFieldList = new FieldList();
			FieldList standardFields = GetStandardFields();

			//o Nominal test
			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(GetConfig(), 5, emptyFieldList))
			{
				Check("HeartbeatGenerator should not be running", hbgen.IsRunning() == false);
			}

			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(GetConfig(), 5, standardFields))
			{
				Check("HeartbeatGenerator should not be running", hbgen.IsRunning() == false);
			}
		}


		private void Test_Start()
		{
			Log.Info("Test Start()");

			Config config = GetConfig();

			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(config, 1, GetStandardFields()))
			{
				/* To ensure this heartbeat message is unique, we set the COMPONENT field */
				hbgen.SetField("COMPONENT", GetUniqueComponent());

				hbgen.Start();

				Check("HeartbeatGenerator should be running", hbgen.IsRunning());

				VerifyHeartbeatMessage(config, 1);

				hbgen.Stop();

				/* Allow time for the HB-gen thread to stop */
				TimeUtil.Millisleep(2000);
			}
		}


		private void Test_Stop()
		{
			Log.Info("Test Stop()");

			Config config = GetConfig();

			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(config, 1, GetStandardFields()))
			{
				hbgen.Start();

				Check("HeartbeatGenerator should be running", hbgen.IsRunning());

				hbgen.Stop();

				Check("HeartbeatGenerator should not be running", hbgen.IsRunning() == false);

				/* Allow time for the HB-gen thread to stop */
				TimeUtil.Millisleep(2000);

				using (Connection conn = new Connection(config))
				{
					conn.Connect();
					conn.Subscribe("C2MS.*.*.*.*.*.MSG.HB." + GetUniqueComponent());

					for (int i = 0; i < 2; ++i)
					{
						Message hbmsg = conn.Receive(5000);

						if (hbmsg != null)
						{
							Check("Unexpectedly received a Heartbeat Message", false);

							Message.Destroy(hbmsg);
						}
					}

					conn.Disconnect();
				}
			}
		}


		private void Test_ChangePublishRate()
		{
			Log.Info("Test ChangePublishRate()");

			Config config = GetConfig();

			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(config, 1, GetStandardFields()))
			{
				/* To ensure this heartbeat message is unique, we set the COMPONENT field */
				hbgen.SetField("COMPONENT", GetUniqueComponent());

				hbgen.Start();

				TimeUtil.Millisleep(2000);    // allow time for generator to start

				hbgen.ChangePublishRate(2);

				VerifyHeartbeatMessage(config, 2);

				using (Connection conn = new Connection(config))
				{
					conn.Connect();
					conn.Subscribe("C2MS.*.*.*.*.*.MSG.HB." + GetUniqueComponent());

					TimeUtil.Millisleep(1000);

					hbgen.ChangePublishRate(0);

					TimeUtil.Millisleep(1000);

					bool lastMessage = false;

					for (int i = 0; i < 3 && !lastMessage; ++i)
					{
						Message hbmsg = conn.Receive(5000);

						if (hbmsg == null)
						{
							Check("Failed to receive Heartbeat Message", false);
						}
						else if (hbmsg.GetU32Value("PUB-RATE") != 0)
						{
							Message.Destroy(hbmsg);
						}
						else
						{
							lastMessage = true;
							Message.Destroy(hbmsg);
						}
					}

					Check("Failed to receive message with PUB-RATE of 0", lastMessage);

					conn.Disconnect();
				}

				hbgen.Stop();

				/* Allow time for the HB-gen thread to stop */
				TimeUtil.Millisleep(2000);
			}
		}


		private void Test_SetField()
		{
			Config config = new Config(GetConfig());     // make a copy

			FieldList standardFields = GetStandardFields();

			//o Nominal tests
			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(config, 1, standardFields))
			{
				/* To ensure this heartbeat message is unique, we set the COMPONENT field */
				hbgen.SetField("COMPONENT", GetUniqueComponent());

				hbgen.Start();

				TimeUtil.Millisleep(2000);

				Log.Info("Test SetField() with a Field");
				hbgen.SetField(new U16Field("PUB-RATE", (ushort) 2));
				VerifyHeartbeatMessage(config, 2);

				Log.Info("Test SetField() with a Field Name and long value");
				hbgen.SetField("PUB-RATE", (long) 3);
				VerifyHeartbeatMessage(config, 3);

				Log.Info("Test SetField() with a Field Name and double value");
				hbgen.SetField("PUB-RATE", 2.0);
				VerifyHeartbeatMessage(config, 2);

				Log.Info("Test SetField() with a Field Name and string value");
				hbgen.SetField("PUB-RATE", "3");
				VerifyHeartbeatMessage(config, 3);

				hbgen.Stop();

				/* Allow time for the HB-gen thread to stop */
				TimeUtil.Millisleep(2000);
			}

			//o Off-nominal tests
			config.AddValue("gmsec-msg-content-validate", "true");

			using (HeartbeatGenerator hbgen = new HeartbeatGenerator(config, 1, standardFields))
			{
				// Add bogus field using a Field
				try {
					hbgen.SetField(new U16Field("BOGUS-FIELD", (ushort) 2));
					Check("An expection was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Message Validation Failed"));
				}

				// Add bogus field using a long value
				try {
					hbgen.SetField("BOGUS-FIELD", (long) 2);
					Check("An expection was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Message Validation Failed"));
				}

				// Add bogus field using a double value
				try {
					hbgen.SetField("BOGUS-FIELD", 2.0);
					Check("An expection was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Message Validation Failed"));
				}

				// Add bogus field using a string value
				try {
					hbgen.SetField("BOGUS-FIELD", "2");
					Check("An expection was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Message Validation Failed"));
				}
			}
		}


		private void VerifyHeartbeatMessage(Config config, ushort expectedPubRate)
		{
			using (Connection conn = new Connection(config))
			{
				conn.Connect();

				conn.Subscribe("C2MS.*.*.*.*.*.MSG.HB." + GetUniqueComponent() + ".+");

				double t1 = 0, t2;

				for (int i = 0; i < 7; ++i)
				{
					Message hbmsg = conn.Receive(5000);

					// ignore the first few incoming messages (if any)
					if (i < 3)
					{
						if (hbmsg != null) Message.Destroy(hbmsg);
						continue;
					}

					if (hbmsg != null)
					{
						if (t1 == 0)
						{
							t1 = TimeUtil.GetCurrentTime_s();
						}
						else
						{
							t2 = TimeUtil.GetCurrentTime_s();

							double delta = t2 - t1;
							if (delta < expectedPubRate)
							{
								delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
							}

							Log.Info("Expected rate is: " + expectedPubRate + ", delta is: " + delta);

							Check("Unexpected publish rate", expectedPubRate == (ushort) delta);

							t1 = t2;
						}

						Check("Unexpected MESSAGE-TYPE", hbmsg.GetStringValue("MESSAGE-TYPE") == "MSG");
						Check("Unexpected MESSAGE-SUBTYPE", hbmsg.GetStringValue("MESSAGE-SUBTYPE") == "HB");
						Check("Unexpected PUB-RATE", hbmsg.GetU32Value("PUB-RATE") == (uint) expectedPubRate);

						Message.Destroy(hbmsg);
					}
					else
					{
						Check("Failed to received Heartbeat Message", false);
					}
				}

				conn.Disconnect();
			}
		}
	}
}
