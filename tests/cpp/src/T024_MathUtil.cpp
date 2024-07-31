#include "TestCase.h"

#include <gmsec5/internal/MathUtil.h>
#include <gmsec5/internal/StringUtil.h>

#include <limits>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


void test_float_equals(Test& test)
{
	GMSEC_INFO << "Test floatEquals()";

	GMSEC_F32 f1 = 10.0f / 3.14f;
	GMSEC_F32 f2 = f1 * 3.14f;

	test.check("Floats should NOT be equal", !MathUtil::floatEquals(f1, f2));

	f2 /= 3.14f;

	test.check("Floats should be equal", MathUtil::floatEquals(f1, f2));
}


void test_double_equals(Test& test)
{
	GMSEC_INFO << "Test doubleEquals()";

	GMSEC_F64 d1 = 10.0 / 3.14;
	GMSEC_F64 d2 = d1 * 3.14;

	test.check("Doubles should NOT be equal", !MathUtil::doubleEquals(d1, d2));

	d2 /= 3.14;

	test.check("DOubles should be equal", MathUtil::doubleEquals(d1, d2));
}


void test_extract_f32(Test& test)
{
	GMSEC_INFO << "Test extractF32()";

	const char* hex1 = "3e7fffff";
	const char* hex2 = "3F600000";
	const char* hex3 = "FF";
	const char* hex4 = NULL;

	GMSEC_F32 f1 = MathUtil::extractF32(hex1);
	test.check("Unexpected F32 extraction", MathUtil::floatEquals(f1, 0.25f));

	GMSEC_F32 f2 = MathUtil::extractF32(hex2);
	test.check("Unexpected F32 extraction", MathUtil::floatEquals(f2, 0.875f));

	try {
		(void) MathUtil::extractF32(hex3);
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("must be 8 hex digits") != std::string::npos);
	}

	try {
		(void) MathUtil::extractF32(hex4);
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("must be 8 hex digits") != std::string::npos);
	}
}


void test_extract_f64(Test& test)
{
	GMSEC_INFO << "Test extractF64()";

	const char* hex1 = "3fd0000000000000";
	const char* hex2 = "3F80000000000000";
	const char* hex3 = "FF";
	const char* hex4 = NULL;

	GMSEC_F64 d1 = MathUtil::extractF64(hex1);
	test.check("Unexpected F64 extraction", MathUtil::doubleEquals(d1, 0.25));

	GMSEC_F64 d2 = MathUtil::extractF64(hex2);
	test.check("Unexpected F64 extraction", MathUtil::doubleEquals(d2, 0.0078125));

	try {
		(void) MathUtil::extractF64(hex3);
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("must be 16 hex digits") != std::string::npos);
	}

	try {
		(void) MathUtil::extractF64(hex4);
		test.check("Expected an exception", false);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("must be 16 hex digits") != std::string::npos);
	}
}


void test_get_infinity(Test& test)
{
	GMSEC_INFO << "Test getInfinity()";

	GMSEC_F64 d = MathUtil::getInfinity();
	test.check("Expected infinity", d == std::numeric_limits<GMSEC_F64>::infinity());
}


void test_get_infinityf(Test& test)
{
	GMSEC_INFO << "Test getInfinityf()";

	GMSEC_F32 f = MathUtil::getInfinityf();
	test.check("Expected infinity", f == std::numeric_limits<GMSEC_F32>::infinity());
}


void test_get_nan(Test& test)
{
	GMSEC_INFO << "Test getNaN()";

	GMSEC_F64 d = MathUtil::getNaN();
	test.check("Expected NaN", std::isnan(d));
}


void test_get_nanf(Test& test)
{
	GMSEC_INFO << "Test getNaNf()";

	GMSEC_F32 f = MathUtil::getNaNf();
	test.check("Expected NaN", std::isnan(f));
}


void test_real_string_constant(Test& test)
{
	GMSEC_INFO << "Test realStringConstant()";

	//o NaN
	test.check("Expected NaN", StringUtil::stringEquals(MathUtil::realStringConstant( MathUtil::getNaN() ), "NAN"));

	//o -INF
	test.check("Expected -INF", StringUtil::stringEquals(MathUtil::realStringConstant( -1 * MathUtil::getInfinity() ), "-INF"));

	//o +INF
	test.check("Expected +INF", StringUtil::stringEquals(MathUtil::realStringConstant( MathUtil::getInfinity() ), "+INF"));

	//o Regular number
	test.check("Expected NULL", MathUtil::realStringConstant(1) == NULL);
}


void test_string_real_constant(Test& test)
{
	GMSEC_INFO << "Test stringRealConstant()";

	//o NaN
	test.check("Expected NaN", std::isnan( MathUtil::stringRealConstant("NAN") ));

	//o -INF
	test.check("Expected -INF", MathUtil::stringRealConstant("-INF") == (-1.0 * std::numeric_limits<GMSEC_F32>::infinity()));

	//o +INF
	test.check("Expected +INF", MathUtil::stringRealConstant("+INF") == std::numeric_limits<GMSEC_F32>::infinity());

	//o Regular number
	test.check("Expected 1.5", MathUtil::stringRealConstant("FOOBAR") == 1.5f);
}


int test_MathUtil(Test& test)
{
	test.setDescription("MathUtil");

	test_float_equals(test);
	test_double_equals(test);
	test_extract_f32(test);
	test_extract_f64(test);
	test_get_infinity(test);
	test_get_infinityf(test);
	test_get_nan(test);
	test_get_nanf(test);
	test_real_string_constant(test);
	test_string_real_constant(test);

	return 0;
}

TEST_DRIVER(test_MathUtil)
