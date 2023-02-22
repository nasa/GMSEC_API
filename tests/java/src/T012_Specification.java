import gov.nasa.gsfc.gmsec.api5.Config;
import gov.nasa.gsfc.gmsec.api5.Gmsec;
import gov.nasa.gsfc.gmsec.api5.GmsecException;
import gov.nasa.gsfc.gmsec.api5.MessageSpecification;
import gov.nasa.gsfc.gmsec.api5.SchemaIDIterator;
import gov.nasa.gsfc.gmsec.api5.Specification;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.util.Collection;


public class T012_Specification extends TestCase
{
	public static void main(String[] args)
	{
		T012_Specification test = new T012_Specification();
		test.setDescription("Test Specification");
		test.run(args);
	}


    public void testCase()
		throws Exception
	{
		try
		{
			test_constructor_1();
			test_constructor_2();
			test_copy_constructor();
			test_get_schema_id_iterator();
			test_get_message_specifications();
			test_get_header_field_names();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_constructor_1()
		throws Exception
	{
		Log.info("Test constructor (no args)");

		Specification spec = new Specification();
		check("Unexpected specification version", spec.getVersion() == Gmsec.MSG_SPEC_CURRENT);
		check("Unexpected specification schema level", spec.getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
	}


	private void test_constructor_2()
		throws Exception
	{
		Log.info("Test constructor w/ Config");

		//o Nominal tests
		{
			Specification spec = new Specification( new Config() );
			check("Unexpected specification version", spec.getVersion() == Gmsec.MSG_SPEC_CURRENT);
			check("Unexpected specification schema level", spec.getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		}

		{
			Specification spec0 = new Specification( new Config("gmsec-specification-version=201900 gmsec-schema-level=0", Gmsec.DataType.KEY_VALUE_DATA) );
			Specification spec1 = new Specification( new Config("gmsec-specification-version=201900 gmsec-schema-level=1", Gmsec.DataType.KEY_VALUE_DATA) );
			Specification spec2 = new Specification( new Config("gmsec-specification-version=201900 gmsec-schema-level=2", Gmsec.DataType.KEY_VALUE_DATA) );

			check("Unexpected specification version", Gmsec.MSG_SPEC_2019_00 == spec0.getVersion());
			check("Unexpected specification schema level", Specification.SchemaLevel.LEVEL_0 == spec0.getSchemaLevel());

			check("Unexpected specification version", Gmsec.MSG_SPEC_2019_00 == spec1.getVersion());
			check("Unexpected specification schema level", Specification.SchemaLevel.LEVEL_1 == spec1.getSchemaLevel());

			check("Unexpected specification version", Gmsec.MSG_SPEC_2019_00 == spec2.getVersion());
			check("Unexpected specification schema level", Specification.SchemaLevel.LEVEL_2 == spec2.getSchemaLevel());
		}

		{
			Specification spec = new Specification( new Config("name=value", Gmsec.DataType.KEY_VALUE_DATA) );
			check("Unexpected specification version", spec.getVersion() == Gmsec.MSG_SPEC_CURRENT);
			check("Unexpected specification schema level", spec.getSchemaLevel() == Specification.SchemaLevel.LEVEL_0);
		}

		//o Off-nominal tests
		try {
			Specification spec = new Specification( new Config("gmsec-specification-version=201300", Gmsec.DataType.KEY_VALUE_DATA) );
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Could not list files in template directory"));
		}

		try {
			Specification spec = new Specification( new Config("gmsec-specification-version=ABC", Gmsec.DataType.KEY_VALUE_DATA) );
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("\"GMSEC-SPECIFICATION-VERSION\" contains invalid value"));
		}

		try {
			Specification spec = new Specification( new Config("gmsec-schema-level=3", Gmsec.DataType.KEY_VALUE_DATA) );
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("DIRECTORY is missing definition for LEVEL-3 HEADER"));
		}

		try {
			Specification spec = new Specification( new Config("gmsec-schema-level=10", Gmsec.DataType.KEY_VALUE_DATA) );
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer"));
		}

		try {
			Specification spec = new Specification( new Config("gmsec-schema-level=ABC", Gmsec.DataType.KEY_VALUE_DATA) );
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("\"GMSEC-SCHEMA-LEVEL\" contains invalid value"));
		}

		try {
			Specification spec = new Specification( new Config("gmsec-schema-path=here", Gmsec.DataType.KEY_VALUE_DATA) );
			check("An exception was expected", false);
		}
		catch (GmsecException e) {
			check(e.getMessage(), e.getMessage().contains("Could not list files in template directory"));
		}
	}


	private void test_copy_constructor()
		throws Exception
	{
		Log.info("Test copy-constructor");

		Specification spec1 = new Specification();
		Specification spec2 = new Specification(spec1);

		check("Unexpected specification version", spec1.getVersion() == spec2.getVersion());
		check("Unexpected specification schema level", spec1.getSchemaLevel() == spec2.getSchemaLevel());
		check("Unexpected message specifications", spec1.getMessageSpecifications().size() == spec2.getMessageSpecifications().size());
	}


	private void test_get_schema_id_iterator()
		throws Exception
	{
		Log.info("Test getSchemaIDIterator()");

		Specification spec = new Specification(); 

		SchemaIDIterator iter = spec.getSchemaIDIterator();

		check("Expected availability of schema IDs", iter.hasNext());
	}


	private void test_get_message_specifications()
		throws Exception
	{
		Log.info("Test getMessageSpecifications()");

		Specification spec = new Specification();

		Collection<MessageSpecification> msgSpecs = spec.getMessageSpecifications();

		check("Expected to have message specifications", msgSpecs.size() > 0);
	}


	private void test_get_header_field_names()
		throws Exception
	{
		Log.info("Test getHeaderFieldNames()");

		Specification spec = new Specification();

		Collection<String> headers = spec.getHeaderFieldNames();
		check("Expected to have message specifications", headers.size() == 26);

		Collection<String> headers2 = spec.getHeaderFieldNames("");
		check("Expected to have message specifications", headers2.size() == 26);

		Collection<String> headers3 = spec.getHeaderFieldNames(null);
		check("Expected to have message specifications", headers3.size() == 26);

		Collection<String> defaultHeaders = spec.getHeaderFieldNames("DEFAULT");
		check("Expected to have message specifications", headers.size() == 26);

		Collection<String> C2MSHeaders = spec.getHeaderFieldNames("C2MS");
		check("Expected to have message specifications", headers.size() == 26);
	}
}
