import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.FieldSpecification;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.MessageSpecification;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.Collection;


public class T015_FieldSpecification extends TestCase
{
	public static void main(String[] args)
	{
		T015_FieldSpecification test = new T015_FieldSpecification();
		test.setDescription("Test FieldSpecification");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_field_specification();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_field_specification()
		throws Exception
	{
		Log.info("Test FieldSpecification");

		test_field_specification_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_0);
		test_field_specification_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_1);
		test_field_specification_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_2);

		test_field_specification_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_0);
		test_field_specification_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_1);
		test_field_specification_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_2);
	}


	private void test_field_specification_aux(int specVersion, Specification.SchemaLevel schemaLevel)
		throws Exception
	{
		Config config = new Config();

		config.addValue("gmsec-specification-version", "" + specVersion);
		config.addValue("gmsec-schema-level", "" + schemaLevel.getValue());

		Specification spec = new Specification(config);

		Collection<MessageSpecification> msgSpecs = spec.getMessageSpecifications();

		for (MessageSpecification msgSpec : msgSpecs)
		{
			Collection<FieldSpecification> fieldSpecs = msgSpec.getFieldSpecifications();

			check("Expected to have at least 1 (one) field specification", fieldSpecs.size() > 0);

			for (FieldSpecification fieldSpec : fieldSpecs)
			{
				check("FieldSpecification name is null", fieldSpec.getName() != null);
				check("FieldSpecification type is null", fieldSpec.getType() != null);
				check("FieldSpecification mode is null", fieldSpec.getMode() != null);
				check("FieldSpecification classification is null", fieldSpec.getClassification() != null);
				check("FieldSpecification value is null", fieldSpec.getValue() != null);
				check("FieldSpecification description is null", fieldSpec.getDescription() != null);
			}
		}
	}
}
