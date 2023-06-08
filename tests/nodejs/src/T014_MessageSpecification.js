#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T014_MessageSpecification extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testMessageSpecification();
	}

	testMessageSpecification()
	{
		gmsec.Log.info("Test MessageSpecification");

		this.testMessageSpecificationAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_0);
		this.testMessageSpecificationAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_1);
		this.testMessageSpecificationAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_2);

		this.testMessageSpecificationAux(gmsec.GMSEC_MSG_SPEC_2019_00, gmsec.Specification.SchemaLevel_LEVEL_0);
		this.testMessageSpecificationAux(gmsec.GMSEC_MSG_SPEC_2019_00, gmsec.Specification.SchemaLevel_LEVEL_1);
		this.testMessageSpecificationAux(gmsec.GMSEC_MSG_SPEC_2019_00, gmsec.Specification.SchemaLevel_LEVEL_2);
	}

	testMessageSpecificationAux(specVersion, schemaLevel)
	{
		var config = new gmsec.Config();
        config.addValue("gmsec-specification-version", specVersion.toString());
        config.addValue("gmsec-schema-level", schemaLevel.toString());

        var spec     = new gmsec.Specification(config);
        var msgSpecs = spec.getMsgSpecs();

		this.check("Expected at least 1 (one) message specification", msgSpecs.size() > 0);

		for (var i = 0; i < msgSpecs.size(); i++) {
			var msgSpec = msgSpecs.get(i);
			this.check("Schema ID is NULL", msgSpec.getSchemaID() != null);
			this.check("Subject Template is NULL", msgSpec.getSubjectTemplate() != null);

			var fieldSpecs = msgSpec.getFieldSpecifications();
            this.check("Expected at least 1 (one) field specification", fieldSpecs.size() > 0);
		}
	}
}

test = new T014_MessageSpecification();
test.setDescription("Test MessageSpecification");
test.run(process.argv);
