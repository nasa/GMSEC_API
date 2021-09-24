
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Thread.h
 *
 *  @brief This file contains a multi-platform abstraction implementation of a Thread class
 *
**/
#ifndef gmsec_util_Thread_h
#define gmsec_util_Thread_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/util/gshared_ptr.h>

#include <string>


// This prevents having to include windows.h
//------------------------------------------
#ifdef WIN32
typedef void * HANDLE;
typedef unsigned long DWORD;
#else
#include <pthread.h>
#include <sched.h>
#endif

namespace gmsec
{

namespace util
{


/**	@class Thread
 *
 *	@brief This provides a multi-platform thread implementation
 *
**/
class GMSEC_API Thread
{
public:

#ifdef WIN32

	typedef HANDLE handle_t;
	typedef DWORD id_t;

#else

	typedef pthread_t handle_t;

#if GMSEC_THREAD_BIG_ID
	typedef unsigned long long id_t;
#	else
	typedef unsigned long id_t;
#	endif

#endif

public:

	GMSEC_DEPRECATED Thread(bool selfReferential = false);
	virtual ~Thread();


	/** @fn start()
	 *
	 *	@brief This function causes the thread to begin execution
	 *
	**/
	int CALL_TYPE start();


	/** @fn run()
	 *
	 *	@brief The action of the thread occurs here
	 *
	**/
	virtual void CALL_TYPE run(void) = 0;


	/**	@fn isCurrentContext()
	 *
	 *	@brief this function return true if it is called from the
	 *	context of the run function or 0 if it is called from
	 *	a different thread
	 *
	**/
	bool CALL_TYPE isCurrentContext();


	/**	@fn getID()
	 *	@brief Returns the ID of the thread object.
	**/
	id_t CALL_TYPE getID();


	/**	@fn getCurrentID()
	 *
	 *	@brief Returns the ID of the calling thread
	 *
	**/
	GMSEC_DEPRECATED static id_t CALL_TYPE getCurrentID();


	/**	@fn join()
	 *
	 *	@brief Blocks the calling thread until the other thread terminates
	 *
	**/
	int CALL_TYPE join();


	int CALL_TYPE detach();

	void attach(gshared_ptr<Thread> &mine);

	/** @fn yield()
	 *
	 *  @brief The thread suspends execution and yields its time on the CPU to
	 *	another thread.
	 *
	**/
	GMSEC_DEPRECATED static void CALL_TYPE yield();


protected:

	void CALL_TYPE setName(const char * name);


private:

	static void * process0(void *arg);
	void process();


	handle_t			fHandle;
	id_t				fID;

	bool				fJoined;
	bool				fDetached;

	gshared_ptr<Thread>	fLifeline;
	std::string			fName;

};

} // namespace util
} // namespace gmsec

#endif /* gmsec_util_Thread_h */

