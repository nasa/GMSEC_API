namespace T012
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
    using System.Collections.Generic;
    using System.Text;


	class T012_Specification : TestCase
	{
		static void Main(string[] args)
		{
			T012_Specification test = new T012_Specification();
			test.SetDescription("Test Specification");
			test.Run(args);
		}


		public override void Run()
		{
			Test_Constructor_1();
			Test_Constructor_2();
			Test_CopyConstructor();
			Test_GetSchemaIDIterator();
			Test_GetMessageSpecifications();
			Test_GetHeaderFieldNames();
		}


		private void Test_Constructor_1()
		{
			Log.Info("Test constructor (no args)");

			Specification spec = new Specification();
			Check("Unexpected specification version", spec.GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
			Check("Unexpected specification schema level", spec.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		}


		private void Test_Constructor_2()
		{
			Log.Info("Test constructor w/ Config");

			//o Nominal tests
			{
				Specification spec = new Specification( new Config() );
				Check("Unexpected specification version", spec.GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
				Check("Unexpected specification schema level", spec.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
			}

			{
				Specification spec0 = new Specification( new Config("gmsec-specification-version=201900 gmsec-schema-level=0", DataType.KEY_VALUE_DATA) );
				Specification spec1 = new Specification( new Config("gmsec-specification-version=201900 gmsec-schema-level=1", DataType.KEY_VALUE_DATA) );
				Specification spec2 = new Specification( new Config("gmsec-specification-version=201900 gmsec-schema-level=2", DataType.KEY_VALUE_DATA) );

				Check("Unexpected specification version", Gmsec.GMSEC_MSG_SPEC_2019_00 == spec0.GetVersion());
				Check("Unexpected specification schema level", Specification.SchemaLevel.LEVEL_0 == spec0.GetSchemaLevel());

				Check("Unexpected specification version", Gmsec.GMSEC_MSG_SPEC_2019_00 == spec1.GetVersion());
				Check("Unexpected specification schema level", Specification.SchemaLevel.LEVEL_1 == spec1.GetSchemaLevel());

				Check("Unexpected specification version", Gmsec.GMSEC_MSG_SPEC_2019_00 == spec2.GetVersion());
				Check("Unexpected specification schema level", Specification.SchemaLevel.LEVEL_2 == spec2.GetSchemaLevel());
			}

			{
				Specification spec = new Specification( new Config("name=value", DataType.KEY_VALUE_DATA) );
				Check("Unexpected specification version", spec.GetVersion() == Gmsec.GMSEC_MSG_SPEC_CURRENT);
				Check("Unexpected specification schema level", spec.GetSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
			}

			//o Off-nominal tests
			try {
				new Specification( new Config("gmsec-specification-version=201300", DataType.KEY_VALUE_DATA) );
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Could not list files in template directory"));
			}

			try {
				new Specification( new Config("gmsec-specification-version=ABC", DataType.KEY_VALUE_DATA) );
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("\"GMSEC-SPECIFICATION-VERSION\" contains invalid value"));
			}

			try {
				new Specification( new Config("gmsec-schema-level=3", DataType.KEY_VALUE_DATA) );
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("DIRECTORY is missing definition for LEVEL-3 HEADER"));
			}

			try {
				new Specification( new Config("gmsec-schema-level=10", DataType.KEY_VALUE_DATA) );
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer"));
			}

			try {
				new Specification( new Config("gmsec-schema-level=ABC", DataType.KEY_VALUE_DATA) );
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("\"GMSEC-SCHEMA-LEVEL\" contains invalid value"));
			}

			try {
				new Specification( new Config("gmsec-schema-path=here", DataType.KEY_VALUE_DATA) );
				Check("An exception was expected", false);
			}
			catch (GmsecException e) {
				Check(e.ToString(), e.ToString().Contains("Could not list files in template directory"));
			}
		}


		private void Test_CopyConstructor()
		{
			Log.Info("Test copy-constructor");

			Specification spec1 = new Specification();
			Specification spec2 = new Specification(spec1);

			Check("Unexpected specification version", spec1.GetVersion() == spec2.GetVersion());
			Check("Unexpected specification schema level", spec1.GetSchemaLevel() == spec2.GetSchemaLevel());
			Check("Unexpected message specifications", spec1.GetMessageSpecifications().Count == spec2.GetMessageSpecifications().Count);
		}


		private void Test_GetSchemaIDIterator()
		{
			Log.Info("Test GetSchemaIDIterator()");

			Specification spec = new Specification(); 

			SchemaIDIterator iter = spec.GetSchemaIDIterator();

			Check("Expected availability of schema IDs", iter.HasNext());
		}


		private void Test_GetMessageSpecifications()
		{
			Log.Info("Test GetMessageSpecifications()");

			Specification spec = new Specification();

			MessageSpecificationList msgSpecs = spec.GetMessageSpecifications();

			Check("Expected to have message specifications", msgSpecs.Count > 0);
		}


		private void Test_GetHeaderFieldNames()
		{
			Log.Info("Test GetHeaderFieldNames()");

			Specification spec = new Specification();

			HeaderList headers = spec.GetHeaderFieldNames();
			Check("Expected to have headers", headers.Count == 26);

			HeaderList headers2 = spec.GetHeaderFieldNames("");
			Check("Expected to have headers", headers2.Count == 26);

			HeaderList defaultHeaders = spec.GetHeaderFieldNames("DEFAULT");
			Check("Expected to have headers", defaultHeaders.Count == 26);

			HeaderList C2MSHeaders = spec.GetHeaderFieldNames("C2MS");
			Check("Expected to have headers", C2MSHeaders.Count == 26);
		}
	}
}
