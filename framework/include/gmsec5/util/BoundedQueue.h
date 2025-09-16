/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file BoundedQueue.h
 *
 *  @brief This file contains a class for cross platform BoundedQueue support
 */

#ifndef GMSEC_API5_UTIL_BOUNDED_QUEUE_H
#define GMSEC_API5_UTIL_BOUNDED_QUEUE_H


#include <gmsec5/GmsecException.h>

#include <gmsec5/util/Condition.h>
#include <gmsec5/util/Mutex.h>

#include <sstream>
#include <cstddef>


namespace gmsec
{
namespace api5
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
template <class T>
class BoundedQueue
{
public:
	/**
	 * @brief The minium queue size allowed.
	 */
	static const size_t MIN_QUEUE_SIZE = 1;


	/**
	 * @brief The maximum queue size allowed.
	 */
	static const size_t MAX_QUEUE_SIZE = 5000;


	/** @fn BoundedQueue(size_t size)
	 *
	 *	@brief Standard constructor. 
	 *
	 *	@params size - The maximum size of the queue
	 */
	BoundedQueue(size_t size)
		: capacity(size),
		  data(0),
		  front(0),
		  rear(0),
		  count(0),
		  someoneWaiting(0),
		  lock(),
		  condition(lock)
	{
		if (capacity < MIN_QUEUE_SIZE)
		{
			std::ostringstream oss;
			oss << "BoundedQueue size must be greater than or equal to " << MIN_QUEUE_SIZE;
			throw GmsecException(OTHER_ERROR, VALUE_OUT_OF_RANGE, oss.str().c_str());
		}
		else if (capacity > MAX_QUEUE_SIZE)
		{
			std::ostringstream oss;
			oss << "BoundedQueue size cannot be greater than " << MAX_QUEUE_SIZE;
			throw GmsecException(OTHER_ERROR, VALUE_OUT_OF_RANGE, oss.str().c_str());
		}

		data = new T[capacity];
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
	T take()
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


	/** @fn poll(int milliseconds, T& value)
	 *
	 *	@brief This function is similar to take(), however it accepts a timeout.
	 *
	 *	@param milliseconds - The number of millseconds to wait for an item.
	 *	@param value - The value, if any, being returned.
	 *
	 *  @return Returns true if an item is being returned; false otherwise.
	 */
	bool poll(int milliseconds, T& value)
	{
		AutoMutex hold(lock);

		while (count == 0)
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


	/** @fn put(T item)
	 *
	 *	@brief Places the item on the queue. If the queue is full, the call will block until
	 *	space becomes available.
	 *
	 *	@param item - The item to be placed upon the queue.
	 */
	void put(T item)
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
	

	/** @fn offer(T item)
	 *
	 *	@brief Attempts to place the item on the queue. If the queue is full, the call
	 *	will return immediately with the value false. True is returned if the item
	 *	was successfully placed.
	 *
	 * 	@param item - The item to be placed upon the queue.
	 */
	bool offer(T item)
	{
		AutoMutex hold(lock);

		if (count == capacity)
		{
			return false;
		}

		offerCommon(item);

		return true;
	}


	/** @fn offer(T item, int milliseconds)
	 *
	 *	@brief Attempts to place the item on the queue. If the queue is full, the call
	 *	will wait for the timeout specified. If there is no space within that time, the
	 *	function will return false. If the item is successfully place, the function returns
	 *	true.
	 *
	 *	@param item - The item to be placed upon the queue.
	 *	@param milliseconds - The number of milliseconds to wait.
	 */
	bool offer(T item, int milliseconds)
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
		AutoMutex hold(lock);

		return count;
	}


private:
	void offerCommon(T item)
	{
		data[rear] = item;

		rear = (rear + 1) % capacity;

		++count;

		if (someoneWaiting)
		{
			condition.signal(Condition::USER);
		}
	}


	T pollCommon()
	{
		T value = data[front];

		front = (front + 1) % capacity;

		--count;

		if (someoneWaiting)
		{
			condition.signal(Condition::USER);
		}

		return value;
	}


	size_t    capacity;
	T*        data;

	size_t    front;
	size_t    rear;
	size_t    count;

	int       someoneWaiting;
	Mutex     lock;
	Condition condition;
};


} // end namespace util
} // end namespace api5
} // end namespace gmsec

#endif
