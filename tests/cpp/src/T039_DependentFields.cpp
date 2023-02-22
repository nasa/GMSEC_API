#include "TestCase.h"

#include <cctype>

using namespace gmsec::api5;
using namespace gmsec::api5::util;


static void RunTest(Test& test, unsigned int version, unsigned int schemaLevel)
{
	GMSEC_INFO << "RunTest: Spec Version: " << version << ", Schema Level: " << schemaLevel;

	Config config(test.getConfig());   // make a copy
	
	config.addValue("GMSEC-MSG-CONTENT-VALIDATE", "true");

	std::stringstream converter;
	converter << version;
	config.addValue("GMSEC-SPECIFICATION-VERSION", converter.str().c_str());

	converter.clear();
	converter.str("");
	converter << schemaLevel;
	config.addValue("GMSEC-SCHEMA-LEVEL", converter.str().c_str());

	converter.clear();
	converter.str("");
	converter << test.getDirectory() << "/../src/addons/T039/templates";
	config.addValue("GMSEC-SCHEMA-PATH", converter.str().c_str());

	try
	{
		MessageFactory factory(config);
		Status         status;
		
		GMSEC_INFO << "Case 1: if dependent field exists, required field becomes optional";
		{
			Message msg = factory.createMessage("MSG.1");
			msg.setSubject( test.getSubject("1").c_str() );

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 1: Message with missing required field passed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 1: Message with required field should have been marked as optional", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}

		GMSEC_INFO << "Case 2: if dependent field exists, optional field becomes required";
		{
			Message msg = factory.createMessage("MSG.2");
			msg.setSubject( test.getSubject("2").c_str() );

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 2: Message with missing optional field should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 2: Message with optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("FIELD", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 2: Optional field marked as required was added but validation still failed", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}

		GMSEC_INFO << "Case 3: if either of two dependent fields exist, optional field becomes required";
		{
			Message msg = factory.createMessage("MSG.3");
			msg.setSubject( test.getSubject("3").c_str() );

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Message with missing optional field should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY-1", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 3: Message with optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.clearField("DEPENDENCY-1");
			msg.setFieldValue("DEPENDENCY-2", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 3: Message with optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
		}

		GMSEC_INFO << "Case 4: if dependent field has a certain value, optional field becomes required";
		{
			Message msg = factory.createMessage("MSG.4");
			msg.setSubject( test.getSubject("4").c_str() );

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 4: Message with missing optional field should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "2");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 4: Message with missing optional field should not have been marked as required", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 4: Message with optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
		}

		GMSEC_INFO << "Case 5: if dependent field has over a certain value, optional field becomes required";
		{
			Message msg = factory.createMessage("MSG.5");
			msg.setSubject( test.getSubject("5").c_str() );

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 5: Message with missing optional field should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "0");

			if (status.hasError())
			{
				test.check("Case 5: Message with missing optional field should not have been marked as required", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 5: Message with missing optional field should not have been marked as required", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "2");

			if (status.hasError())
			{
				test.check("Case 5: Message with optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}

		GMSEC_INFO << "Case 6: if dependent field has less than a certain value, optional field becomes required";
		{
			Message msg = factory.createMessage("MSG.6");
			msg.setSubject( test.getSubject("6").c_str() );

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 6: Message with missing optional field should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "0");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 6: Message with optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 6: Message with missing optional field should not have been marked as required", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "2");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 6: Message with missing optional field should not have been marked as required", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}

		GMSEC_INFO << "Case 7: if dependent field has a certain value, required field must have certain type";
		{
			Message msg = factory.createMessage("MSG.7");
			msg.setSubject( test.getSubject("7").c_str() );

			msg.addField("FIELD", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 7: Field with correct type should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.addField("FIELD", (GMSEC_I16)1);
			msg.setFieldValue("FIELD", "signed");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 7: String Field marked as I16 should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.addField("FIELD", (GMSEC_U16)1);
			msg.setFieldValue("FIELD", "unsigned");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 7: String Field marked as U16 should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}

		GMSEC_INFO << "Case 8: if dependent field has a certain value, required field must have certain value";
		{
			Message msg = factory.createMessage("MSG.8");
			msg.setSubject( test.getSubject("8").c_str() );

			msg.setFieldValue("FIELD", "0");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 8: Field with correct value should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("FIELD", "1");
			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 8: Field marked with value 1 should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("FIELD", "2");
			msg.setFieldValue("DEPENDENCY", "2");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 8: Field marked with value 2 should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}

		GMSEC_INFO << "Case 9: if dependent field exists, optional field in an array is required";
		{
			Message msg = factory.createMessage("MSG.9");
			msg.setSubject( test.getSubject("9").c_str() );

			msg.setFieldValue("NUM-OF-ARRAYS", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 9: Message with missing optional field should have passed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 9: Optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("ARRAY.1.FIELD", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 9: Optional field that was marked as required was added but validation still failed", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("NUM-OF-ARRAYS", "2");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 9: Optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("ARRAY.2.FIELD", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 9: Optional field that was marked as required was added but validation still failed", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}
		
		GMSEC_INFO << "Case 10: if dependent field in an array exists, optional field in same array is required";
		{
			Message msg = factory.createMessage("MSG.10");
			msg.setSubject( test.getSubject("10").c_str() );

			msg.setFieldValue("NUM-OF-ARRAYS", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 10: Missing optional field in array should have passed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("ARRAY.1.DEPENDENCY", "1");
			msg.setFieldValue("DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 10: Optional field should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
		}

		GMSEC_INFO << "Case 11: if dependent field in an array exists, optional field in a child array is required";
		{
			Message msg = factory.createMessage("MSG.11");
			msg.setSubject( test.getSubject("11").c_str() );

			msg.setFieldValue("NUM-OF-ARRAYS", "1");
			msg.setFieldValue("ARRAY.1.NUM-OF-CHILDREN", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 11: Message with missing optional field in array should have passed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("ARRAY.1.DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 11: Optional field in array should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("ARRAY.1.CHILD.1.FIELD", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 11: Optional field in array that was marked as required was added but validation still failed", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("NUM-OF-ARRAYS", "2");
			msg.setFieldValue("ARRAY.2.NUM-OF-CHILDREN", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 11: Second array with missing optional field should not have failed validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}

			msg.setFieldValue("ARRAY.2.DEPENDENCY", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Passed", true);
			}
			else
			{
				test.check("Case 11: Optional field in second array should have been marked as required and fail validation", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}

			msg.setFieldValue("ARRAY.2.CHILD.1.FIELD", "1");

			status = msg.isCompliant();

			if (status.hasError())
			{
				test.check("Case 11: Optional field in second array that was marked as required was added but validation still failed", false);
				GMSEC_WARNING << "Unexpected Compliancy Result:\n" << status.get();
			}
			else
			{
				test.check("Passed", true);
			}
		}
	}
	catch (const GmsecException& e)
	{
		test.require(e.what(), false);
	}
}


int test_DependentFields(Test& test)
{
	test.setDescription("DependentFields");

	RunTest(test, GMSEC_MSG_SPEC_2019_00, 0);

	return 0;
}

TEST_DRIVER(test_DependentFields)
