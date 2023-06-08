import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.SchemaIDIterator;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.util.Log;


public class T013_SchemaIDIterator extends TestCase
{
	public static void main(String[] args)
	{
		T013_SchemaIDIterator test = new T013_SchemaIDIterator();
		test.setDescription("Test SchemaIDIterator");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_iterator();
			test_reset();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_iterator()
		throws Exception
	{
		Log.info("Test hasNext() and next()");

		test_schema_id_iterator_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_0, 73);
		test_schema_id_iterator_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_1, 103);
		test_schema_id_iterator_aux(Gmsec.MSG_SPEC_CURRENT, Specification.SchemaLevel.LEVEL_2, 106);

		test_schema_id_iterator_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_0, 73);
		test_schema_id_iterator_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_1, 103);
		test_schema_id_iterator_aux(Gmsec.MSG_SPEC_2019_00, Specification.SchemaLevel.LEVEL_2, 106);
	}


	private void test_reset()
		throws Exception
	{
		Log.info("Test reset()");

		Specification spec = new Specification();

		SchemaIDIterator iter = spec.getSchemaIDIterator();

		while (iter.hasNext())
		{
			iter.next();
		}

		check("Expected no more schema IDs", iter.hasNext() == false);

		iter.reset();

		check("Expected to have schema IDs", iter.hasNext());
	}


	private void test_schema_id_iterator_aux(int specVersion, Specification.SchemaLevel schemaLevel, int expectedSchemaIds)
		throws Exception
	{
		Config config = new Config();

		config.addValue("gmsec-specification-version", "" + specVersion);
		config.addValue("gmsec-schema-level", "" + schemaLevel.getValue());

		Specification spec = new Specification(config);

		SchemaIDIterator iter = spec.getSchemaIDIterator();

		int numSchemaIds = 0;

		while (iter.hasNext())
		{
			String id = iter.next();

			check("Schema ID is null", id != null);

			++numSchemaIds;
		}

		check("Unexpected number of schema IDs", expectedSchemaIds == numSchemaIds);
	}
}
