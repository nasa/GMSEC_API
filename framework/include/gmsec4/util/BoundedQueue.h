/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file BoundedQueue.h
 *
 *  @brief This file contains a class for cross platform BoundedQueue support
 */

#ifndef GMSEC_API_UTIL_BOUNDED_QUEUE_H
#define GMSEC_API_UTIL_BOUNDED_QUEUE_H


#include <gmsec4/util/Condition.h>
#include <gmsec4/util/Mutex.h>

#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api
{
namespace util
{

/**	@class BoundedQueue
 *
 *	@brief This provides a blocking, bounded queue implementation. This implementation is
 *	inherently thread safe and, should all operations be used concurrently, will not require
 *	an external mutex.
 *
 *	An example of proper use would be, instead of calling:
 *
 *	if (queue.isEmpty()) queue.push(item);
 *
 *	use the following type of operation
 *
 *	bool success = boundedqueue.offer(item);
 */
template <class elemType>
class BoundedQueue
{
public:
	/** @fn BoundedQueue(size_t size)
	 *
	 *	@brief Standard constructor. 
	 *
	 *	@params size - The maximum size of the queue
	 */
	BoundedQueue(size_t size)
		: capacity(size),
		  data(new elemType[size]),
		  front(0),
		  rear(0),
		  count(0),
		  someoneWaiting(0),
		  lock(),
		  condition(lock)
	{
	}	


	/** @fn ~BoundedQueue()
	 *
	 *	@brief Standard destructor. 
	 */
	~BoundedQueue()
	{
		delete[] data;
	}


	/** @fn take()
	 *
	 *	@brief This function retreives the next item in the queue. This call will block indefinitely
	 *	until another item becomes available.
	 */
	elemType take()
	{
		AutoMutex hold(lock);

		while (count == 0)
		{
			++someoneWaiting;
			condition.wait();
			--someoneWaiting;
		}
		
		return pollCommon();
	}


	/** @fn poll(int milliseconds)
	 *
	 *	@brief This function is similar to take(), however it accepts a timeout. If an item
	 *	is not available within the timeout limit, a TimeoutException is thrown. Since we don't
	 *	know what type of thing could be on the queue, we can't just return NULL;
	 *
	 *	@param milliseconds - The number of millseconds to wait for an item.
	 */
	bool poll(int milliseconds, elemType &value)
	{
		AutoMutex hold(lock);

		if (count == 0)
		{
			++someoneWaiting;
			int signal = condition.wait(milliseconds);
			--someoneWaiting;

			if (signal == Condition::TIMEOUT)
			{
				return false;
			}
		}
		
		value = pollCommon();

		return true;
	}


	/** @fn put(elemType item)
	 *
	 *	@brief Places the item on the queue. If the queue is full, the call will block until
	 *	space becomes available.
	 *
	 *	@param item - The item to be placed upon the queue.
	 */
	void put(elemType item)
	{
		AutoMutex hold(lock);

		bool inserted = false;

		while (!inserted)
		{
			inserted = offer(item);
			if (!inserted)
			{
				++someoneWaiting;
				condition.wait();
				--someoneWaiting;
			}
		}
	}
	

	/** @fn offer(elemType item)
	 *
	 *	@brief Attempts to place the item on the queue. If the queue is full, the call
	 *	will return immediately with the value false. True is returned if the item
	 *	was successfully placed.
	 *
	 * 	@param item - The item to be placed upon the queue.
	 */
	bool offer(elemType item)
	{
		AutoMutex hold(lock);

		if (count == capacity)
		{
			return false;
		}

		offerCommon(item);

		return true;
	}


	/** @fn offer(elemType item, int milliseconds)
	 *
	 *	@brief Attempts to place the item on the queue. If the queue is full, the call
	 *	will wait for the timeout specified. If there is no space within that time, the
	 *	function will return false. If the item is successfully place, the function returns
	 *	true.
	 *
	 *	@param item - The item to be placed upon the queue.
	 *	@param milliseconds - The number of milliseconds to wait.
	 */
	bool offer(elemType item, int milliseconds)
	{
		AutoMutex hold(lock);

		if (count == capacity)
		{
			++someoneWaiting;
			int signal = condition.wait(milliseconds);
			--someoneWaiting;

			if (signal == Condition::TIMEOUT)
			{
				return false;
			}			
		}
		
		offerCommon(item);

		return true;
	}


	/** @fn remainingCapacity()
	 *
	 *	@brief Returns the number of items that can be placed on the queue, assuming
	 *	no items are taken. Note that this method has been preserved from the Java 
	 *	specification, but it practice it is best not to use it because it breaks
	 *	the atomicity of the BoundedQueue and necessitates the use of an external mutex.
	 */
	size_t remainingCapacity()
	{
		AutoMutex hold(lock);

		return capacity - count;
	}


	/** @fn empty()
	 *
	 *	@brief Returns true if the queue is empty, false otherwise
	 */
	bool empty()
	{
		AutoMutex hold(lock);

		return count == 0;
	}


	/** @fn queuedElements()
	 *
	 *	@brief Returns the number of elements held in the queue
	 */
	size_t queuedElements()
	{
		return count;
	}


private:
	size_t    capacity;
	elemType* data;

	size_t    front;
	size_t    rear;
	size_t    count;

	int       someoneWaiting;
	Mutex     lock;
	Condition condition;


	void offerCommon(elemType item)
	{
		data[rear] = item;

		rear = (rear + 1) % capacity;

		++count;

		if (someoneWaiting)
		{
			condition.signal(Condition::USER);
		}
	}


	elemType pollCommon()
	{
		elemType value = data[front];

		front = (front + 1) % capacity;

		--count;

		if (someoneWaiting)
		{
			condition.signal(Condition::USER);
		}

		return value;
	}
};


}
}
}

#endif
