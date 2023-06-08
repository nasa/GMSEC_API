#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T013_SchemaIdIterator extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testIterator();
		this.testReset();
	}

	testIterator()
	{
		gmsec.Log.info("Test hasNext() and next()");

		this.testSchemaIdIteratorAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_0, 73);
		this.testSchemaIdIteratorAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_1, 103);
		this.testSchemaIdIteratorAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_2, 106);

		this.testSchemaIdIteratorAux(gmsec.GMSEC_MSG_SPEC_2019_00, gmsec.Specification.SchemaLevel_LEVEL_0, 73);
		this.testSchemaIdIteratorAux(gmsec.GMSEC_MSG_SPEC_2019_00, gmsec.Specification.SchemaLevel_LEVEL_1, 103);
		this.testSchemaIdIteratorAux(gmsec.GMSEC_MSG_SPEC_2019_00, gmsec.Specification.SchemaLevel_LEVEL_2, 106);
	}

	testReset()
	{
		gmsec.Log.info("Test reset()");

		var spec = new gmsec.Specification();
		var iter = spec.getSchemaIDIterator();

		while (iter.hasNext()) {
			iter.next();
		}

		this.check("Expected no more schema IDs", iter.hasNext() == false);

		iter.reset();

		this.check("Expected have schema IDs", iter.hasNext());
	}

	testSchemaIdIteratorAux(specVersion, schemaLevel, expectedSchemaIds)
	{
		var config = new gmsec.Config();
		config.addValue("gmsec-specification-version", specVersion.toString());
		config.addValue("gmsec-schema-level", schemaLevel.toString());

		var spec = new gmsec.Specification(config);
		var iter = spec.getSchemaIDIterator();

		var numSchemaIds = 0;

		while (iter.hasNext()) {
			var id = iter.next();

			this.check("Schema ID is null", id != null);

			numSchemaIds += 1;
		}

		this.check("Unexpected number of schema IDs", numSchemaIds == expectedSchemaIds);
	}
}

test = new T013_SchemaIdIterator();
test.setDescription("Test SchemaIdIterator");
test.run(process.argv);
