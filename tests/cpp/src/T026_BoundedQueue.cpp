#include "TestCase.h"

#include <gmsec5/GmsecException.h>

#include <gmsec5/util/BoundedQueue.h>
#include <gmsec5/util/StdSharedPtr.h>
#include <gmsec5/util/StdThread.h>

#include <string>


using namespace gmsec::api5;
using namespace gmsec::api5::util;


typedef BoundedQueue<std::string> BoundedQueueStrType;
typedef BoundedQueue<int>         BoundedQueueIntType;


class OtherThread
{
public:
	BoundedQueueStrType& bq;
	bool match;

	OtherThread(BoundedQueueStrType& queue)
		: bq(queue),
		  match(false) {
	}

	virtual ~OtherThread() {
	}

	virtual void CALL_TYPE run() {
		match = (bq.take() == "TestWait");
	}
};


class PullThread
{
public:
	BoundedQueueStrType& bq;
	bool pulled;

	PullThread(BoundedQueueStrType& queue)
		: bq(queue),
		  pulled(false) {
	}

	virtual ~PullThread() {
	}

	virtual void CALL_TYPE run() {
		pulled = (bq.take() == "ValueOne");
	}
};


static void runOtherThread(StdSharedPtr<OtherThread> sharedOther)
{
	sharedOther->run();
}


static void runPullThread(StdSharedPtr<PullThread> sharedPull)
{
	sharedPull->run();
}


int test_BoundedQueue(Test& test)
{
	test.setDescription("BoundedQueue");

	BoundedQueueStrType testWaitQueue(5);
	BoundedQueueIntType queue(3);

	StdSharedPtr<OtherThread> sharedOther(new OtherThread(testWaitQueue));
	StdThread otherThread(&runOtherThread, sharedOther);
	otherThread.start();

	test.require("Empty queue", queue.empty());

	queue.put(1);
	queue.put(2);
	queue.put(3);

	test.require("Depth of 3", queue.queuedElements() == 3);
	test.require("Non-empty queue", !queue.empty());

	int v = queue.take();
	test.require("First value not 1", v == 1);

	queue.put(4);
	v = queue.take();
	test.require("Second value not 2", v == 2);
	v = queue.take();
	test.require("Third value not 3", v == 3);
	v = queue.take();
	test.require("Fourth value not 4", v == 4);

	test.require("Queue empty", queue.remainingCapacity() == 3);

	BoundedQueueStrType stringQueue(3);

	stringQueue.put("One");
	stringQueue.put("Two");
	stringQueue.put("Three");

	test.require("First value not One", stringQueue.take() == "One");

	stringQueue.put("Four");

	test.require("Second value not Two", stringQueue.take() == "Two");
	test.require("Third value not Three", stringQueue.take() == "Three");
	test.require("Fourth value not Four", stringQueue.take() == "Four");

	test.require("stringQueue empty", stringQueue.remainingCapacity() == 3);

	testWaitQueue.put("TestWait");

	BoundedQueueStrType testPushQueue(5);
	testPushQueue.put("ValueOne");
	testPushQueue.put("ValueTwo");
	testPushQueue.put("ValueThree");
	testPushQueue.put("ValueFour");
	testPushQueue.put("ValueFive");

	test.require("Queue should be full", testPushQueue.remainingCapacity() == 0);

	bool pushed = testPushQueue.offer("RejectMe");
	test.require("Additional value should have been rejected", !pushed);

	StdSharedPtr<PullThread> sharedPull(new PullThread(testPushQueue));
	StdThread pullThread(&runPullThread, sharedPull);
	pullThread.start();

	testPushQueue.put("ValueSix");

	test.require("Second value not ValueTwo", testPushQueue.take() == "ValueTwo");
	test.require("Third value not ValueThree", testPushQueue.take() == "ValueThree");
	test.require("Fourth value not ValueFour", testPushQueue.take() == "ValueFour");

	pushed = testPushQueue.offer("ValueSeven");
	test.require("Additional value should have been accepted", pushed);
	pushed = testPushQueue.offer("ValueEight");
	test.require("Additional value should have been accepted", pushed);
	pushed = testPushQueue.offer("ValueNine");
	test.require("Additional value should have been accepted", pushed);

	pushed = testPushQueue.offer("ShouldFail");
	test.require("Additional value should have been rejected", !pushed);
	test.require("Queue should be full", testPushQueue.remainingCapacity() == 0);

	pushed = testPushQueue.offer("ShouldFail", 500);
	test.require("Additional value should have been rejected", !pushed);
	test.require("Queue should be full", testPushQueue.remainingCapacity() == 0);

	test.require("Fifth value not ValueFive", testPushQueue.take() == "ValueFive");
	test.require("Sixth value not ValueSix", testPushQueue.take() == "ValueSix");
	test.require("Seventh value not ValueSeven", testPushQueue.take() == "ValueSeven");
	test.require("Eight value not ValueEight", testPushQueue.take() == "ValueEight");
	test.require("Night value not ValueNine", testPushQueue.take() == "ValueNine");

	test.require("Queue should be empty", testPushQueue.remainingCapacity() == 5);

	pushed = testPushQueue.offer("NewSeries");
	test.require("Additional value should have been accepted", pushed);

	test.require("Queue should be one element", testPushQueue.remainingCapacity() == 4);

	test.require("PullThread pulled messages", sharedPull->pulled);

	test.require("OtherThread got message", sharedOther->match);

	std::string s;
	test.require("Return false", testPushQueue.poll(500, s));
	test.require("NewSeries value not retreived", s == "NewSeries");
	
	test.require("NewSeries value not retreived", !testPushQueue.poll(500, s));

	//o Test off-nominal construction
	try {
		BoundedQueue<std::string> q(BoundedQueue<std::string>::MIN_QUEUE_SIZE - 1);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("BoundedQueue size must be greater than or equal to 1") != std::string::npos);
	}

	try {
		BoundedQueue<int> q(BoundedQueue<int>::MAX_QUEUE_SIZE + 1);
	}
	catch (const GmsecException& e) {
		test.check(e.what(), std::string(e.what()).find("BoundedQueue size cannot be greater than 5000") != std::string::npos);
	}

	return 0;
}

TEST_DRIVER(test_BoundedQueue)
