namespace T015
{
	using GMSEC.API5;
	using GMSEC.API5.Testing.Common;
	using System;
	using System.Text;


	class T015_FieldSpecification : TestCase
	{
		static void Main(string[] args)
		{
			T015_FieldSpecification test = new T015_FieldSpecification();
			test.SetDescription("Test FieldSpecification");
			test.Run(args);
		}


		public override void Run()
		{
			Test_FieldSpecification();
		}


		private void Test_FieldSpecification()
		{
			Log.Info("Test FieldSpecification");

			Test_FieldSpecificationAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_0);
			Test_FieldSpecificationAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_1);
			Test_FieldSpecificationAux(Gmsec.GMSEC_MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_2);

			Test_FieldSpecificationAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_0);
			Test_FieldSpecificationAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_1);
			Test_FieldSpecificationAux(Gmsec.GMSEC_MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_2);
		}


		private void Test_FieldSpecificationAux(int specVersion, Specification.SchemaLevel schemaLevel)
		{
			Config config = new Config();

			config.AddValue("gmsec-specification-version", "" + specVersion);
			config.AddValue("gmsec-schema-level", "" + (int) schemaLevel);

			Specification spec = new Specification(config);

			MessageSpecificationList msgSpecs = spec.GetMessageSpecifications();

			foreach (MessageSpecification msgSpec in msgSpecs)
			{
				FieldSpecificationList fieldSpecs = msgSpec.GetFieldSpecifications();

				Check("Expected to have at least 1 (one) field specification", fieldSpecs.Count > 0);

				foreach (FieldSpecification fieldSpec in fieldSpecs)
				{
					Check("FieldSpecification name is null", fieldSpec.GetName() != null);
					Check("FieldSpecification type is null", fieldSpec.GetType() != null);
					Check("FieldSpecification mode is null", fieldSpec.GetMode() != null);
					Check("FieldSpecification classification is null", fieldSpec.GetClassification() != null);
					Check("FieldSpecification value is null", fieldSpec.GetValue() != null);
					Check("FieldSpecification description is null", fieldSpec.GetDescription() != null);
				}
			}
		}
	}
}
