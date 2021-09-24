/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** @file Atomics.h
 *
 *  @brief This file contains the definition for a series of Atomic elements.
 *
**/

#ifndef gmsec_util_Atomics_h
#define gmsec_util_Atomics_h

#include <gmsec_defs.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/Mutex.h>


namespace gmsec
{
namespace util
{

class GMSEC_API AtomicInteger
{
public:
	/*	@fn AtomicInteger(int initialValue)
	*
	*	@brief Basic constructor that accepts the initial value of the object
	*	
	*	@param initialValue - The initial value of the object
	*/
	GMSEC_DEPRECATED AtomicInteger(int initialValue);

	/*	@fn ~AtomicInteger()
	*
	*	@brief Basic destructor
	*	
	*/
	~AtomicInteger();

	/*	@fn addAndGet(int addition)
	*
	*	@brief Adds the parameter to the object's value and returns the sum
	*	
	*	@param addition - The value to be added
	*/
	int addAndGet(int addition);

	/*	@fn compareAndSet(int expect, int update)
	*
	*	@brief Compares the internal value to an expected value and sets the internal
	*	value to a new int if the expected value is matched
	*	
	*	@param expect - The value expected to be held internally
	*	@param update - The value set if the expected value is found
	*/
	bool compareAndSet(int expect, int update);

	/*	@fn decrementAndGet()
	*
	*	@brief Decrements the internal value by one and returns the difference
	*	
	*/
	int decrementAndGet();

	/*	@fn doubleValue()
	*
	*	@brief Returns the value of the internal int as a double
	*	
	*/
	double doubleValue();

	/*	@fn floatValue()
	*
	*	@brief Returns the value of the internal int as a float
	*	
	*/
	float floatValue();

	/*	@fn get()
	*
	*	@brief Returns the value of the internal int
	*	
	*/
	int get();

	/*	@fn getAndAdd(int delta)
	*
	*	@brief Adds the delta to the internal value and returns the value before modification
	*	
	*	@param delta - The amount to be added to the internal int
	*/
	int getAndAdd(int delta);

	/*	@fn getAndDecrement()
	*
	*	@brief Returns the value of the internal integer and decrements it by one afterwards
	*	
	*/
	int getAndDecrement();

	/*	@fn getAndIncrement()
	*
	*	@brief Returns the value of the internal integer and increments it by one afterwards
	*	
	*/
	int getAndIncrement();

	/*	@fn getAndSet(int newValue)
	*
	*	@brief Returns the value of the internal integer and sets the internal integer
	*	to the new value afterwards.
	*	
	*	@param newValue - The new value of the internal integer
	*/
	int getAndSet(int newValue);

	/*	@fn incrementAndGet()
	*
	*	@brief Increments the internal int by one and returns it
	*	
	*/
	int incrementAndGet();

	/*	@fn longValue()
	*
	*	@brief Returns the value of the internal integer as a long
	*	
	*/
	long longValue(); 

	/*	@fn set(int newValue)
	*
	*	@brief Sets the value of the internal integer to the new value
	*	
	*	@param newValue - The new value of the internal integer
	*/
	void set(int newValue); 
private:
	// not implemented
	AtomicInteger();
	AtomicInteger(const AtomicInteger &);
	AtomicInteger &operator=(const AtomicInteger &);

	int value;
	gmsec::util::Mutex mutex;
};


class GMSEC_API AtomicBoolean
{
public:
	/*	@fn AtomicBoolean(bool initialValue)
	*
	*	@brief Basic constructor that accepts the initial value of the object
	*	
	*	@param initialValue - The initial value of the object
	*/
	GMSEC_DEPRECATED AtomicBoolean(bool initialValue = false);

	/*	@fn ~AtomicBoolean()
	*
	*	@brief Basic destructor
	*	
	*/
	~AtomicBoolean();

	/*	@fn compareAndSet(bool expect, bool update);
	*
	*	@brief Compares the internal value to an expected value and sets the internal
	*	value to a value if the expected value is matched
	*	
	*	@param expect - The value expected to be held internally
	*	@param update - The value set if the expected value is found
	*/
	bool compareAndSet(bool expect, bool update);

	/*	@fn get()
	*
	*	@brief Returns the value of the internal bool
	*	
	*/
	bool get();

	/*	@fn getAndSet(bool newValue)
	*
	*	@brief Returns the value of the internal bool and sets the internal bool
	*	to the new value afterwards.
	*	
	*	@param newValue - The new value of the internal bool
	*/
	bool getAndSet(bool newValue);

	/*	@fn set(bool newValue)
	*
	*	@brief Sets the value of the internal bool to the new value
	*	
	*	@param newValue - The new value of the internal bool
	*/
	void set(bool newValue); 
private:

	// not implemented
	AtomicBoolean(const AtomicBoolean &);
	AtomicBoolean &operator=(const AtomicBoolean &);

	bool value;
	gmsec::util::Mutex mutex;
};

class GMSEC_API AtomicLong
{
public:
	/*	@fn AtomicLong(long initialValue)
	*
	*	@brief Basic constructor that accepts the initial value of the object
	*	
	*	@param initialValue - The initial value of the object
	*/
	GMSEC_DEPRECATED AtomicLong(long initialValue);

	/*	@fn ~AtomicLong()
	*
	*	@brief Basic destructor
	*	
	*/
	~AtomicLong();

	/*	@fn addAndGet(long addition)
	*
	*	@brief Adds the parameter to the object's value and returns the sum
	*	
	*	@param addition - The value to be added
	*/
	long addAndGet(long addition);

	/*	@fn compareAndSet(long expect, long update)
	*
	*	@brief Compares the internal value to an expected value and sets the internal
	*	value to a new long if the expected value is matched
	*	
	*	@param expect - The value expected to be held internally
	*	@param update - The value set if the expected value is found
	*/
	bool compareAndSet(long expect, long update);

	/*	@fn decrementAndGet()
	*
	*	@brief Decrements the internal value by one and returns the difference
	*	
	*/
	long decrementAndGet();

	/*	@fn doubleValue()
	*
	*	@brief Returns the value of the internal long as a double
	*	
	*/
	double doubleValue();

	/*	@fn get()
	*
	*	@brief Returns the value of the internal long
	*	
	*/
	long get();

	/*	@fn getAndAdd(long delta)
	*
	*	@brief Adds the delta to the internal value and returns the value before modification
	*	
	*	@param delta - The amount to be added to the internal long
	*/
	long getAndAdd(long delta);

	/*	@fn getAndDecrement()
	*
	*	@brief Returns the value of the internal long and decrements it by one afterwards
	*	
	*/
	long getAndDecrement();

	/*	@fn getAndIncrement()
	*
	*	@brief Returns the value of the internal long and increments it by one afterwards
	*	
	*/
	long getAndIncrement();

	/*	@fn getAndSet(long newValue)
	*
	*	@brief Returns the value of the long integer and sets the internal long
	*	to the new value afterwards.
	*	
	*	@param newValue - The new value of the internal long
	*/
	long getAndSet(long newValue);

	/*	@fn incrementAndGet()
	*
	*	@brief Increments the internal long by one and returns it
	*	
	*/
	long incrementAndGet();

	/*	@fn set(long newValue)
	*
	*	@brief Sets the value of the internal long to the new value
	*	
	*	@param newValue - The new value of the internal long
	*/
	void set(long newValue); 
private:
	long value;
	gmsec::util::Mutex mutex;
};

GMSEC_DEPRECATED GMSEC_I32 GMSEC_API syncAddAndFetch(volatile GMSEC_I32 *p, GMSEC_I32 n);

}
}


#endif

