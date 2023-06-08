#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T012_Specification extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testConstructor_1();
		this.testConstructor_2();
		this.testCopyConstructor();
		this.testGetSchemaIDIterator();
		this.testGetMsgSpecs();
		this.testGetHeaderFields();
	}

	testConstructor_1()
	{
		gmsec.Log.info("Test constructor w/ no args")

		try {
			var spec = new gmsec.Specification();
			this.check("Unexpected specification version", spec.getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
			this.check("Unexpected specification schema level", spec.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_0);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testConstructor_2()
	{
		gmsec.Log.info("Test constructor w/ Config")

		// Nominal tests
		try {
			var spec = new gmsec.Specification(new gmsec.Config());
			this.check("Unexpected specification version", spec.getVersion() == gmsec.GMSEC_MSG_SPEC_CURRENT);
			this.check("Unexpected specification schema level", spec.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_0);

			var spec0 = new gmsec.Specification(new gmsec.Config("gmsec-specification-version=201900 gmsec-schema-level=0", gmsec.DataType_KEY_VALUE_DATA));
			var spec1 = new gmsec.Specification(new gmsec.Config("gmsec-specification-version=201900 gmsec-schema-level=1", gmsec.DataType_KEY_VALUE_DATA));
			var spec2 = new gmsec.Specification(new gmsec.Config("gmsec-specification-version=201900 gmsec-schema-level=2", gmsec.DataType_KEY_VALUE_DATA));

			this.check("Unexpected specification version", spec0.getVersion() == gmsec.GMSEC_MSG_SPEC_2019_00);
			this.check("Unexpected specification schema level", spec0.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_0);

			this.check("Unexpected specification version", spec1.getVersion() == gmsec.GMSEC_MSG_SPEC_2019_00);
			this.check("Unexpected specification schema level", spec1.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_1);

			this.check("Unexpected specification version", spec2.getVersion() == gmsec.GMSEC_MSG_SPEC_2019_00);
			this.check("Unexpected specification schema level", spec2.getSchemaLevel() == gmsec.Specification.SchemaLevel_LEVEL_2);
		}
		catch (e) {
			this.require(e.message, false);
		}

		// Off-nominal tests
		// TODO: Not able to test because SWIG-generated code does not propagate
		// GMSEC API exceptions on overloaded functions.
		/*
		try {
			var spec = new gmsec.Specification(new gmsec.Config("gmsec-specification-version=201300", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Could not list files in template directory"));
		}

		try {
			var spec = new gmsec.Specification(new gmsec.Config("gmsec-specification-version=ABC", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("\"GMSEC-SPECIFICATION-VERSION\" contains invalid value"));
		}

		try {
			var spec = new gmsec.Specification(new gmsec.Config("gmsec-schema-level=3", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("DIRECTORY is missing definition for LEVEL-3 HEADER"));
		}

		try {
			var spec = new gmsec.Specification(new gmsec.Config("gmsec-schema-level=10", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("\"GMSEC-SCHEMA-LEVEL\" contains out-of-range integer"));
		}

		try {
			var spec = new gmsec.Specification(new gmsec.Config("gmsec-schema-level=ABC", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("\"GMSEC-SCHEMA-LEVEL\" contains invalid value"));
		}

		try {
			var spec = new gmsec.Specification(new gmsec.Config("gmsec-schema-path=here", gmsec.DataType_KEY_VALUE_DATA));
			this.check("Expected an exception", false);
		}
		catch (e) {
			this.check(e.message, e.message.includes("Could not list files in template directory"));
		}
		*/
	}

	testCopyConstructor()
	{
		gmsec.Log.info("Test copy constructor")

		try {
			var spec1 = new gmsec.Specification(new gmsec.Config("gmsec-specification-version=201900 gmsec-schema-level=2", gmsec.DataType_KEY_VALUE_DATA));
			var spec2 = new gmsec.Specification(spec1);

			this.check("Unexpected specification version", spec1.getVersion() == spec2.getVersion());
			this.check("Unexpected specification schema level", spec1.getSchemaLevel() == spec2.getSchemaLevel());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetSchemaIDIterator()
	{
		gmsec.Log.info("Test getSchemaIDIterator()")

		try {
			var spec = new gmsec.Specification();
			var iter = spec.getSchemaIDIterator();
			this.check("Expected iterator to have schema IDs", iter.hasNext());
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetMsgSpecs()
	{
		gmsec.Log.info("Test getMsgSpecs")

		try {
			var spec = new gmsec.Specification();
			var msgSpecs = spec.getMsgSpecs();
			this.check("Expected to have message specifications", msgSpecs.size() > 0);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}

	testGetHeaderFields()
	{
		gmsec.Log.info("Test getHeaderFields()")

		try {
			var spec = new gmsec.Specification();
			var headers = null;

			headers = spec.getHeaderFields();
			this.check("Expected to have message specs", headers.size() == 26);
			/*
			for (var i = 0; i < headers.size(); ++i) {
				gmsec.Log.info(headers.get(i));
			}
			*/

			headers = spec.getHeaderFields("");
			this.check("Expected to have message specs", headers.size() == 26);

			headers = spec.getHeaderFields("DEFAULT");
			this.check("Expected to have message specs", headers.size() == 26);

			headers = spec.getHeaderFields("C2MS");
			this.check("Expected to have message specs", headers.size() == 26);
		}
		catch (e) {
			this.require(e.message, false);
		}
	}
}

test = new T012_Specification();
test.setDescription("Test Specification");
test.run(process.argv);
