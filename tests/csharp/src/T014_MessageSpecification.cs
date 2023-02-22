namespace T014
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T014_MessageSpecification : TestCase
	{
		static void Main(string[] args)
		{
			T014_MessageSpecification test = new T014_MessageSpecification();
			test.SetDescription("Test MessageSpecification");
			test.Run(args);
		}


		public override void Run()
		{
			Test_MessageSpecification();
		}


		private void Test_MessageSpecification()
		{
			Log.Info("Test MessageSpecification");

			Test_MessageSpecificationAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_0);
			Test_MessageSpecificationAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_1);
			Test_MessageSpecificationAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_2);

			Test_MessageSpecificationAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_0);
			Test_MessageSpecificationAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_1);
			Test_MessageSpecificationAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_2);
		}


		private void Test_MessageSpecificationAux(int specVersion, Specification.SchemaLevel schemaLevel)
		{
			Config config = new Config();

			config.AddValue("gmsec-specification-version", "" + specVersion);
			config.AddValue("gmsec-schema-level", "" + (int) schemaLevel);

			Specification spec = new Specification(config);

			MessageSpecificationList msgSpecs = spec.GetMessageSpecifications();

			Check("Expected at least 1 (one) message specification", msgSpecs.Count > 0);

			foreach (MessageSpecification msgSpec in msgSpecs)
			using (msgSpec)
			{
				Check("Schema ID is NULL", msgSpec.GetSchemaID() != null);
				Check("Subject Template is NULL", msgSpec.GetSubjectTemplate() != null);

				FieldSpecificationList fieldSpecs = msgSpec.GetFieldSpecifications();

				Check("Expected at least 1 (one) field specification", fieldSpecs.Count > 0);
			}
		}
	}
}
