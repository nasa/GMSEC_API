import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.FieldSpecification;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.MessageSpecification;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.Collection;


public class T014_MessageSpecification extends TestCase
{
	public static void main(String[] args)
	{
		T014_MessageSpecification test = new T014_MessageSpecification();
		test.setDescription("Test MessageSpecification");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_message_specification();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_message_specification()
		throws Exception
	{
		Log.info("Test MessageSpecification");

		test_message_specification_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_0);
		test_message_specification_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_1);
		test_message_specification_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_2);

		test_message_specification_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_0);
		test_message_specification_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_1);
		test_message_specification_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_2);
	}


	private void test_message_specification_aux(int specVersion, Specification.SchemaLevel schemaLevel)
		throws Exception
	{
		Config config = new Config();

		config.addValue("gmsec-specification-version", "" + specVersion);
		config.addValue("gmsec-schema-level", "" + schemaLevel.getValue());

		Specification spec = new Specification(config);

		Collection<MessageSpecification> msgSpecs = spec.getMessageSpecifications();

		check("Expected at least 1 (one) message specification", msgSpecs.size() > 0);

		for (MessageSpecification msgSpec : msgSpecs)
		{
			check("Schema ID is NULL", msgSpec.getSchemaID() != null);
			check("Subject Template is NULL", msgSpec.getSubjectTemplate() != null);

			Collection<FieldSpecification> fieldSpecs = msgSpec.getFieldSpecifications();

			check("Expected at least 1 (one) field specification", fieldSpecs.size() > 0);
		}
	}
}
