/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file StdThread.h
 *
 * @brief This file contains defines an thread class taking an approach
 * similar to the C++ 2011 thread.
 *
**/
#ifndef gmsec_util_StdThread_h
#define gmsec_util_StdThread_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/util/Mutex.h>
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


namespace gmsec {
namespace util {


/**	@class StdThread
 *
 *	@brief This provides a multi-platform thread implementation
 *
**/
class GMSEC_API StdThread
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

	GMSEC_DEPRECATED StdThread();

	template <class Func, class Arg> StdThread(Func f, Arg a)
		: fInvoker(new FunctionInvoker<Func, Arg>(f, a)) { zero(); }

	~StdThread();


	/** @fn start()
	 *
	 *	@brief This function causes the thread to begin execution
	 *
	**/
	int CALL_TYPE start();


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

	/** @fn yield()
	 *
	 *  @brief The thread suspends execution and yields its time on the CPU to
	 *	another thread.
	 *
	**/
	GMSEC_DEPRECATED static void CALL_TYPE yield();


	void CALL_TYPE setName(const char * name);


private:

	struct Info
	{
		Info();
		void set(int i);
		bool is(int i) const;
		int state;
		id_t id;
	};


	class GMSEC_API Invoker
	{
	public:
		Invoker() { }
		virtual ~Invoker() { }

		void setShared(gshared_ptr<Info> &sp);

		void invoke();

		virtual void CALL_TYPE _invoke();

	private:
		gshared_ptr<Info> info;
	};


	template <class Func, class Arg>
		class FunctionInvoker : public Invoker
	{
	public:
		FunctionInvoker(Func f, Arg a)
			: function(f), argument(a) { }

		virtual void CALL_TYPE _invoke()
		{
			function(argument);
		}

	private:
		Func function;
		Arg argument;
	};


	static void * process0(void *arg);

	void zero();
	void process();
	std::string describe(const char *tag = 0);


	Invoker				*fInvoker;
	handle_t			fHandle;
	gshared_ptr<Info>		fInfo;

	bool				fJoined;
	bool				fDetached;

	std::string			fName;

	// Declared, but not implemented.
	StdThread(const StdThread&);
	StdThread& operator=(const StdThread&);
};

} // namespace util
} // namespace gmsec

#endif /* gmsec_util_StdThread_h */

