namespace T013
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T013_SchemaIDIterator : TestCase
	{
		static void Main(string[] args)
		{
			T013_SchemaIDIterator test = new T013_SchemaIDIterator();
			test.SetDescription("Test SchemaIDIterator");
			test.Run(args);
		}


		public override void Run()
		{
			Test_Iterator();
			Test_Reset();
		}


		private void Test_Iterator()
		{
			Log.Info("Test HasNext() and Next()");

			Test_SchemaIdIteratorAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_0, 73);
			Test_SchemaIdIteratorAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_1, 103);
			Test_SchemaIdIteratorAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_2, 106);

			Test_SchemaIdIteratorAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_0, 73);
			Test_SchemaIdIteratorAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_1, 103);
			Test_SchemaIdIteratorAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_2, 106);
		}


		private void Test_Reset()
		{
			Log.Info("Test Reset()");

			Specification spec = new Specification();

			SchemaIDIterator iter = spec.GetSchemaIDIterator();

			while (iter.HasNext())
			{
				iter.Next();
			}

			Check("Expected no more schema IDs", iter.HasNext() == false);

			iter.Reset();

			Check("Expected to have schema IDs", iter.HasNext());
		}


		private void Test_SchemaIdIteratorAux(int specVersion, Specification.SchemaLevel schemaLevel, int expectedSchemaIds)
		{
			Config config = new Config();

			config.AddValue("gmsec-specification-version", "" + specVersion);
			config.AddValue("gmsec-schema-level", "" + (int) schemaLevel);

			Specification spec = new Specification(config);

			SchemaIDIterator iter = spec.GetSchemaIDIterator();

			int numSchemaIds = 0;

			while (iter.HasNext())
			{
				string id = iter.Next();

				Check("Schema ID is null", id != null);

				++numSchemaIds;
			}

			Check("Unexpected number of schema IDs", expectedSchemaIds == numSchemaIds);
		}
	}
}
