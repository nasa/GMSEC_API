/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_util_gshared_ptr_h
#define gmsec_util_gshared_ptr_h


#include <gmsec/util/Atomics.h>
#include <gmsec/util/Deprecated.h>


namespace gmsec
{
namespace util
{

template <class T> class gshared_ptr
{
public:

	GMSEC_DEPRECATED gshared_ptr<T> ()
			: ptr(0), stash(0)
	{
	}

	GMSEC_DEPRECATED gshared_ptr<T> (T *t)
			: ptr(t), stash(0)
	{
		if (ptr)
			stash = new Stash;
	}

	gshared_ptr<T>(const gshared_ptr<T> &other)
			:
			ptr(other.ptr),
			stash(0)
	{
		if (other.stash)
			attach(other.stash);
	}

	~gshared_ptr<T>()
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

	gshared_ptr<T> &operator= (gshared_ptr<T> &rhs)
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
} // namespace gmsec

#endif /*  gmsec_util_gshared_ptr_h */
