/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Future.h
 *
 *  @brief This file contains a cross-platform Future that functions the same
 *	as the Java interface of the same name.
**/


#ifndef gmsec_util_Future_h
#define gmsec_util_Future_h

namespace gmsec
{
namespace internal
{

template <class elemType>
class Future
{
public:

	/*	@fn Future()
	*
	*	@brief Standard constructor
	*
	*/
	Future() : computationDone(false), cond(mutex)
	{
		
	}

	/*	@fn get()
	*
	*	@brief This retrieves the value returned by the performTask function that extentions
	*	of Future must implement. This call will block until the value is set by performTask
	*	or an external source calling set.
	*
	*/
	elemType CALL_TYPE get()
	{
		AutoMutex hold(mutex);
		if(!computationDone){
			cond.Wait();
		}
		return value;
	}

	/*	@fn get(long timeout, elemType &value)
	*
	*	@brief This retrieves the value returned by the performTask function that extentions
	*	of Future must implement. This call will block until the value is set by performTask
	*	or an external source calling set, or until the timeout is met. THe function will
	*	return true if a value was generated in time or false if a timeout occured.
	*
	*	@param timeout - The amount of time, in millis, to wait
	*	@param value - The reference to where to store the returned value
	*/
	bool CALL_TYPE get(long timeout, elemType &val)
	{
		AutoMutex hold(mutex);
		if(!computationDone){
			if(cond.Wait(timeout) == Condition::TIMEOUT){
				return false;
			}			
		}

		val = value;

		return true;
	}

	/*	@fn isDone()
	*
	*	@brief Checks whether or not the future is finished performing its task.
	*
	*/
	bool CALL_TYPE isDone()
	{
		return computationDone;
	}
	
	/*	@fn performTask()
	*
	*	@brief This method must be implemented by all extensions of the Future class.
	*	This function does the work to compute the value which will be returned.
	*
	*/
	virtual elemType CALL_TYPE performTask() = 0;

	/*	@fn run()
	*
	*	@brief This method is inherited from Thread
	*
	*/
	void CALL_TYPE execute()
	{
		elemType val = performTask();
		AutoMutex hold(mutex);
		if(!computationDone){
			done();
			//printf("Setting value via execute\n");
			value = val;
		}
	}

	/*	@fn set(elemType setVal)
	*
	*	@brief This method is sets the value of the Future if the performTask method has
	*	not already been returned.
	*
	*	@param value - The value to set as the Future's value
	*/
	void CALL_TYPE set(elemType setVal)
	{
		AutoMutex lock(mutex);
		if(!computationDone){
			//printf("Setting value via set\n");
			done();
			value = setVal;
			
		}
	}


private:

	// Declared, but not implemented.
	Future(const Future &);
	Future &operator=(const Future &);

	void CALL_TYPE done()
	{
		AutoMutex hold(mutex);
		computationDone = true;
		cond.Broadcast(Condition::USER);
	}			

	bool computationDone;
	elemType value;
	Mutex mutex;
	Condition cond;

};

}
}


#endif

