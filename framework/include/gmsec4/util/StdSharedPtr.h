/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_UTIL_GSHARED_PTR_H
#define GMSEC_API_UTIL_GSHARED_PTR_H


#include <gmsec4_defs.h>

#include <gmsec4/util/Atomics.h>


namespace gmsec {
namespace api {
namespace util {


template <class T> class StdSharedPtr
{
public:

	StdSharedPtr<T> ()
			: ptr(0), stash(0)
	{
	}

	StdSharedPtr<T> (T *t)
			: ptr(t), stash(0)
	{
		if (ptr)
			stash = new Stash;
	}

	StdSharedPtr<T>(const StdSharedPtr<T> &other)
			:
			ptr(other.ptr),
			stash(0)
	{
		if (other.stash)
			attach(other.stash);
	}

	~StdSharedPtr<T>()
	{
		release();
	}

	int count()
	{
		if (!stash)
			return 0;
		return stash->count;
	}

	T* get()
	{
		return ptr;
	}

	const T* get() const
	{
		return ptr;
	}

	void reset()
	{
		release();
	}

	void reset(T *t)
	{
		release();
		if (t)
			stash = new Stash;
		ptr = t;
	}

	T* operator-> ()
	{
		return ptr;
	}

	T& operator*()
	{
		return *ptr;
	}

	StdSharedPtr<T> &operator= (StdSharedPtr<T> &rhs)
	{
		if (this == &rhs)
			return *this;
		release();
		ptr = rhs.ptr;
		attach(rhs.stash);
		return *this;
	}

	operator bool ()
	{
		return ptr != 0;
	}


private:

	struct Stash
	{
		Stash() : count(GMSEC_I32(1)) { }
		volatile GMSEC_I32 count;
	private:
		// not allowed
		Stash(const Stash &);
		Stash &operator= (const Stash &);
	};

	void release()
	{
		if (!stash)
			return;
		T* goner = 0;
		{
			GMSEC_I32 count = syncAddAndFetch(&stash->count, GMSEC_I32(-1));
			if (count == 0)
			{
				goner = ptr;
				delete stash;
			}
			ptr = 0;
			stash = 0;
		}
		if (goner)
			delete goner;
	}

	void attach(Stash *s)
	{
		stash = s;
		if (stash)
			(void) syncAddAndFetch(&stash->count, GMSEC_I32(1));
	}

	T *ptr;
	Stash *stash;
};

} // namespace util
} // namespace api
} // namespace gmsec

#endif
