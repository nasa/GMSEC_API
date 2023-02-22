#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


void test_AtomicInteger(Test& test)
{
	GMSEC_INFO << "Test AtomicInteger";

	AtomicInteger atom(1);
	test.check("1: Expected an int value of 1", atom.get() == 1);

	atom.set(2);

	test.check("2: Expected a double value of 2", atom.doubleValue() == static_cast<double>(2));
	test.check("3: Expected a double value of 2", atom.floatValue() == static_cast<float>(2));
	test.check("4: Expected an int value of 2", atom.longValue() == static_cast<long>(2));

	test.check("5: Expectation is value is not 3", atom.compareAndSet(3, 2) == false);
	test.check("6: Expectation is value is 2", atom.compareAndSet(2, 3) == true);
	test.check("7: Expected an int value of 3", atom.get() == 3);

	test.check("8: Expected an int value of 4", atom.addAndGet(1) == 4);
	test.check("9: Expected an int value of 3", atom.decrementAndGet() == 3);

	test.check("10: Expected an int value of 3", atom.getAndAdd(1) == 3);
	test.check("11: Expected an int value of 4", atom.getAndDecrement() == 4);
	test.check("12: Expected an int value of 3", atom.getAndIncrement() == 3);

	test.check("13: Expected an int value of 4", atom.getAndSet(5) == 4);
	test.check("14: Expected an int value of 6", atom.incrementAndGet() == 6);
}


void test_AtomicBoolean(Test& test)
{
	GMSEC_INFO << "Test AtomicBoolean";

	AtomicBoolean atom(true);
	test.check("1: Expected a value of true", atom.get());

	atom.set(false);

	test.check("2: Expected comparison to be true", atom.compareAndSet(false, true));
	test.check("3: Expected comparison to be false", atom.compareAndSet(false, true) == false);

	test.check("4: Expected value to be true", atom.getAndSet(false));
}


void test_AtomicLong(Test& test)
{
	GMSEC_INFO << "Test AtomicLong";

	AtomicLong atom(static_cast<long>(1));
	test.check("1: Expected a long value of 1", atom.get() == static_cast<long>(1));

	atom.set(static_cast<long>(2));

	test.check("2: Expected a double value of 2", atom.doubleValue() == static_cast<double>(2));

	test.check("3: Expectation is value is not 3", atom.compareAndSet(static_cast<long>(3), static_cast<long>(2)) == false);
	test.check("4: Expectation is value is 2", atom.compareAndSet(static_cast<long>(2), static_cast<long>(3)) == true);
	test.check("5: Expected a value of 3", atom.get() == static_cast<long>(3));

	test.check("6: Expected a value of 4", atom.addAndGet(static_cast<long>(1)) == static_cast<long>(4));
	test.check("7: Expected a value of 3", atom.decrementAndGet() == static_cast<long>(3));

	test.check("9: Expected a value of 3", atom.getAndAdd(static_cast<long>(1)) == static_cast<long>(3));
	test.check("10: Expected a value of 4", atom.getAndDecrement() == static_cast<long>(4));
	test.check("11: Expected an int value of 3", atom.getAndIncrement() == static_cast<long>(3));

	test.check("12: Expected an int value of 4", atom.getAndSet(static_cast<long>(5)) == static_cast<long>(4));
	test.check("13: Expected an int value of 6", atom.incrementAndGet() == static_cast<long>(6));
}


int test_Atomics(Test& test)
{
	test.setDescription("Test Atomics");

	test_AtomicInteger(test);
	test_AtomicBoolean(test);
	test_AtomicLong(test);

	return 0;
}

TEST_DRIVER(test_Atomics)
