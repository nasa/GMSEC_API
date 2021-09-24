/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef bolt_counted_ptr_h
#define bolt_counted_ptr_h

#include <gmsec4/util/Atomics.h>

template <class T> class counted_ptr
{
public:

	counted_ptr<T> ()
			: ptr(0), stash(0)
	{
	}

	counted_ptr<T> (T *t)
			: ptr(t), stash(0)
	{
		if (ptr)
			stash = new Stash();
	}

	counted_ptr<T>(const counted_ptr<T> &other)
			:
			ptr(other.ptr),
			stash(0)
	{
		attach(other.stash);
	}

	~counted_ptr<T>()
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
			stash = new Stash();
		ptr = t;
	}

	T* operator-> () const
	{
		return const_cast<T*>(ptr);
	}

	T& operator*() const
	{
		return const_cast<T&>(*ptr);
	}


	counted_ptr<T> &operator= (const counted_ptr<T> &rhs)
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
		Stash() : count(1) { }
		volatile bolt::i32 count;
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
			bolt::i32 count = gmsec::api::util::syncAddAndFetch(&stash->count, -1);
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
			gmsec::api::util::syncAddAndFetch(&stash->count, 1);
	}


	T *ptr;
	Stash *stash;
};


#endif /* bolt_counted_ptr_h */

