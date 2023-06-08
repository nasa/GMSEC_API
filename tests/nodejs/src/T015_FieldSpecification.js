#!/usr/bin/env node

const TestCase = require('./TestCase.js');
const gmsec = require('../../../bin/gmsec_nodejs.node');


class T015_FieldSpecification extends TestCase
{
	constructor()
	{
		super();
	}

	testCase()
	{
		this.testFieldSpecification();
	}

	testFieldSpecification()
	{
		gmsec.Log.info("Test FieldSpecification");

		this.testFieldSpecificationAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_0);
		this.testFieldSpecificationAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_1);
		this.testFieldSpecificationAux(gmsec.GMSEC_MSG_SPEC_CURRENT, gmsec.Specification.SchemaLevel_LEVEL_2);
	}

	testFieldSpecificationAux(specVersion, schemaLevel)
	{
		var config = new gmsec.Config();
		config.addValue("gmsec-specification-version", specVersion.toString());
		config.addValue("gmsec-schema-level", schemaLevel.toString());

		var spec     = new gmsec.Specification(config);
		var msgSpecs = spec.getMsgSpecs();

		for (var i = 0; i < msgSpecs.size(); i++)
		{
			var msgSpec    = msgSpecs.get(i);
			var fieldSpecs = msgSpec.getFieldSpecifications();

			if (fieldSpecs != null) {
				this.check("Expected to have at least 1 (one) field specification", fieldSpecs.size() > 0);

				for (var j = 0; j < fieldSpecs.size(); j++)
				{
					const fieldSpec = fieldSpecs.get(j);
					const name      = fieldSpec.getName();
					const type      = fieldSpec.getType();
					const mode      = fieldSpec.getMode();
					const value     = fieldSpec.getValue();
					const classy    = fieldSpec.getClassification();
					const desc      = fieldSpec.getDescription();

					this.check("FieldSpecification name is null", name != null);
					this.check("FieldSpecification type is null", type != null);
					this.check("FieldSpecification mode is null", mode != null);
					this.check("FieldSpecification value is null", value != null);
					this.check("FieldSpecification classification is null", classy != null);
					this.check("FieldSpecification description is null", desc != null);
				}
			}
			else {
				this.check("FieldSpecification entry is null!", false);
			}
		}
	}
}

test = new T015_FieldSpecification();
test.setDescription("Test FieldSpecification");
test.run(process.argv);
