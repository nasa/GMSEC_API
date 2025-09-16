namespace T016
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T016_ResourceGenerator : TestCase
	{
		static void Main(string[] args)
		{
			T016_ResourceGenerator test = new T016_ResourceGenerator();
			test.SetDescription("Test ResourceGenerator");
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
				Test_SetField();
				Test_CreateResourceMessage();
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
			using (ResourceGenerator rsrcgen = new ResourceGenerator(GetConfig(), 5, 1, 10))
			{
				Check("ResourceGenerator should not be running", rsrcgen.IsRunning() == false);
			}

			//o Off-nominal test(s)
			try
			{
				// Bogus middleware
				Config config = new Config(GetConfig());
				config.AddValue("mw-id", "bogus-mw");
				new ResourceGenerator(config, 5, 1, 10);
				Check("An exception was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Unable to load"));
			}
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor 2");

			FieldList emptyFieldList = new FieldList();
			FieldList standardFields = GetStandardFields();

			//o Nominal test (empty, populated, and null list of fields)
			using (ResourceGenerator rsrcgen = new ResourceGenerator(GetConfig(), 5, 1, 10, emptyFieldList))
			{
				Check("ResourceGenerator should not be running", rsrcgen.IsRunning() == false);
			}

			using (ResourceGenerator rsrcgen = new ResourceGenerator(GetConfig(), 5, 1, 10, standardFields))
			{
				Check("ResourceGenerator should not be running", rsrcgen.IsRunning() == false);
			}

			//o Off-nominal test(s)
			try
			{
				// Bogus middleware
				Config config = new Config(GetConfig());
				config.AddValue("mw-id", "bogus-mw");
				new ResourceGenerator(config, 5, 1, 10, standardFields);
				Check("An exception was expected", false);
			}
			catch (GmsecException e)
			{
				Check(e.ToString(), e.ToString().Contains("Unable to load"));
			}
		}


		private void Test_Start()
		{
			Log.Info("Test Start()");

			Config config  = new Config(GetConfig());
			ushort pubRate = 1;

			using (ResourceGenerator rsrcgen = new ResourceGenerator(config, pubRate, 1, 10, GetStandardFields()))
			{
				/* To ensure this resource message is unique, we set the COMPONENT field */
				rsrcgen.SetField( new StringField("COMPONENT", GetUniqueComponent()) );

				rsrcgen.Start();

				Check("ResourceGenerator should be running", rsrcgen.IsRunning());

				VerifyResourceMessage(config, pubRate);

				rsrcgen.Stop();

				/* Allow time for the RSRC-gen thread to stop */
				TimeUtil.Millisleep(2000);
			}

			// Off-nominal test(s)
			config.AddValue("gmsec-msg-content-validate", "true");

			using (ResourceGenerator rsrcgen = new ResourceGenerator(config, pubRate, 1, 10, GetStandardFields()))
			{
				try {
					// Add bogus field
					rsrcgen.SetField( new StringField("BOGUS-FIELD", "2") );
					rsrcgen.Start();
					Check("An exception was expected", false);
				}
				catch (GmsecException e) {
					Check(e.ToString(), e.ToString().Contains("Message Validation Failed"));
				}
			}
		}


		private void Test_Stop()
		{
			Log.Info("Test Stop()");

			Config config  = GetConfig();
			ushort pubRate = 1;

			using (ResourceGenerator rsrcgen = new ResourceGenerator(config, pubRate, 1, 10, GetStandardFields()))
			{
				/* To ensure this resource message is unique, we set the COMPONENT field */
				rsrcgen.SetField( new StringField("COMPONENT", GetUniqueComponent()) );

				rsrcgen.Start();

				Check("ResourceGenerator should be running", rsrcgen.IsRunning());

				rsrcgen.Stop();

				Check("ResourceGenerator should not be running", rsrcgen.IsRunning() == false);

				/* Allow time for the RSRC-gen thread to stop */
				TimeUtil.Millisleep(2000);

				using (Connection conn = new Connection(config))
				{
					conn.Connect();
					conn.Subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + GetUniqueComponent());

					for (int i = 0; i < 3; ++i)
					{
						Message rsrcMsg = conn.Receive(5000);

						if (rsrcMsg != null)
						{
							Check("Unexpectedly received a Resource Message", false);

							Message.Destroy(rsrcMsg);
						}
					}
				}
			}
		}


		private void Test_SetField()
		{
			Config config = new Config(GetConfig());     // make a copy

			FieldList standardFields = GetStandardFields();
			ushort    pubRate = 1;

			//o Nominal tests
			using (ResourceGenerator rsrcgen = new ResourceGenerator(config, pubRate, 1, 10, standardFields))
			{
				/* To ensure this resource message is unique, we set the COMPONENT field */
				rsrcgen.SetField( new StringField("COMPONENT", GetUniqueComponent()) );

				rsrcgen.Start();

				TimeUtil.Millisleep(2000);

				Log.Info("Test SetField() with a Field");
				ushort newPubRate = 2;
				rsrcgen.SetField(new U16Field("PUB-RATE", newPubRate));
				VerifyResourceMessage(config, newPubRate);

				rsrcgen.Stop();

				/* Allow time for the RSRC-gen thread to stop */
				TimeUtil.Millisleep(2000);
			}
		}


		private void Test_CreateResourceMessage()
		{
			Log.Info("Test CreateResourceMessage()");

			using (MessageFactory factory = new MessageFactory( GetConfig() ))
			{
				ushort sampleInterval  = 1;
				ushort averageInterval = 10;

				factory.SetStandardFields( GetStandardFields() );

				Message rsrcMsg = ResourceGenerator.CreateResourceMessage( factory, sampleInterval, averageInterval );

				Check("Unexpected MESSAGE-TYPE", rsrcMsg.GetStringValue("MESSAGE-TYPE") == "MSG");
				Check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.GetStringValue("MESSAGE-SUBTYPE") == "RSRC");
			}
		}


		private void VerifyResourceMessage(Config config, ushort expectedPubRate)
		{
			using (Connection conn = new Connection(config))
			{
				conn.Connect();

				conn.Subscribe("C2MS.*.*.*.*.*.MSG.RSRC." + GetUniqueComponent() + ".+");

				double t1 = 0, t2;

				for (int i = 0; i < 7; ++i)
				{
					// Start time t1 is at the time start to receive data
					t1 = TimeUtil.GetCurrentTime_s();
					Message rsrcMsg = conn.Receive(5000);

					// The end time t2 has to be measured immediately right after it completes receiving data.
					t2 = TimeUtil.GetCurrentTime_s();

					// ignore the first few incoming messages (if any)
					if (i < 3)
					{
						if (rsrcMsg != null) Message.Destroy(rsrcMsg);
						continue;
					}

					if (rsrcMsg != null)
					{
						//if (t1 == 0)
						//{
						//	t1 = TimeUtil.GetCurrentTime_s();
						//}
						//else
						//{
						//	t2 = TimeUtil.GetCurrentTime_s();

							double delta = t2 - t1;
							ushort roundUpPubRateToSecond = (ushort)delta;
							if (delta < expectedPubRate)
							{
								//delta = ((t2 - t1) * 10.0 + 0.5) / 10.0;
								roundUpPubRateToSecond = (ushort)(delta + 0.5);    // round up to second
							}

							Log.Info("Expected rate is: " + expectedPubRate + ", delta is: " + delta);

							//Check("Unexpected publish rate", expectedPubRate == (ushort) delta);
							Check("Unexpected publish rate", expectedPubRate == roundUpPubRateToSecond);

						//	t1 = t2;
						//}

						Check("Unexpected MESSAGE-TYPE", rsrcMsg.GetStringValue("MESSAGE-TYPE") == "MSG");
						Check("Unexpected MESSAGE-SUBTYPE", rsrcMsg.GetStringValue("MESSAGE-SUBTYPE") == "RSRC");
						Check("Unexpected PUB-RATE", rsrcMsg.GetU32Value("PUB-RATE") == (uint) expectedPubRate);

						Message.Destroy(rsrcMsg);
					}
					else
					{
						Check("Failed to received Resource Message", false);
					}
				}

				conn.Disconnect();
			}
		}
	}
}
