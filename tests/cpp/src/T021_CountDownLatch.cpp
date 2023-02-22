#include "TestCase.h"

using namespace gmsec::api5;
using namespace gmsec::api5::util;


class MyWorker
{
public:
	static void runWorker(StdSharedPtr<MyWorker> mw) {
		mw->run();
	}

	MyWorker(CountDownLatch& cdl, int wakeUpPeriod) : m_cdl(cdl), m_wakeUpPeriod(wakeUpPeriod) {
	}

	~MyWorker() {
	}

	void run() {
		for (int i = 0; i < m_wakeUpPeriod; ++i) {
			GMSEC_INFO << "Worker is counting down...";
			m_cdl.countDown();
			TimeUtil::millisleep(1000);
		}
		m_cdl.countDown();
	}

private:
	CountDownLatch& m_cdl;
	int m_wakeUpPeriod;
};


void test_1(Test& test)
{
	GMSEC_INFO << "Test 1";

	int count = 5;
	CountDownLatch cdl(count);
	test.check("Expected CountDownLatch count to be 5", cdl.getCount() == count);

	StdSharedPtr<MyWorker>  mw( new MyWorker(cdl, count) );
	StdUniquePtr<StdThread> thread( new StdThread(&MyWorker::runWorker, mw) );

	thread->start();

	cdl.await();  // blocks

	test.check("Expected CountDownLatch count to be 5", cdl.getCount() == 0);

	thread->join();
}


void test_2(Test& test)
{
	GMSEC_INFO << "Test 2";

	int count = 5;
	CountDownLatch cdl(count);
	test.check("Expected CountDownLatch count to be 5", cdl.getCount() == count);

	StdSharedPtr<MyWorker>  mw( new MyWorker(cdl, count) );
	StdUniquePtr<StdThread> thread( new StdThread(&MyWorker::runWorker, mw) );

	thread->start();

	while (!cdl.await(1000)) {
		GMSEC_INFO << "Main thread is waiting...";
	}

	test.check("Expected CountDownLatch count to be 5", cdl.getCount() == 0);

	thread->join();
}


int test_CountDownLatch(Test& test)
{
	test.setDescription("Test CountDownLatch");

	test_1(test);   // use await()
	test_2(test);   // use await(timeout)

	return 0;
}

TEST_DRIVER(test_CountDownLatch)
