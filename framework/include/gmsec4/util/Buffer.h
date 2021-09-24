/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Buffer.h
 *
 *  @brief Interface to memory managed array.
 *
**/

#ifndef GMSEC_API_UTIL_BUFFER_H
#define GMSEC_API_UTIL_BUFFER_H

#include <gmsec4_defs.h>

#include <stddef.h>  // for size_t


namespace gmsec {
namespace api {
namespace util {


/* This is a simple but inefficient implementation.
   TODO: share reps between Buffers- see shared_ptr
*/


template<class T> class Buffer
{
public:
	typedef T data_t;

	Buffer(size_t count = 0)
		: ptr(0), owned(false), used(0), space(0)
	{
		if (count > 0)
		{
			ptr = new T[count];
			used = count;
			space = count;
			owned = true;
		}
	}

	Buffer(const T* p, size_t count, bool ownership)
		: ptr(p), owned(ownership), used(count), space(count)
	{
	}

	Buffer(const Buffer<T>& other)
		: ptr(0), owned(true), used(other.size()), space(other.size())
	{
		if (used > 0)
		{
			ptr = new T[other.size()];
		}
		for (size_t i = 0; i < used; ++i)
		{
			ptr[i] = other.ptr[i];
		}
	}

	~Buffer()
	{
		if (owned && ptr)
		{
			delete [] ptr;
		}

		ptr   = 0;
		owned = false;
		used  = 0;
		space = 0;
	}


	size_t size() const
	{
		return used;
	}


	const T* get() const
	{
		return ptr;
	}


	T* raw()
	{
		return const_cast<T*>(ptr);
	}


	bool is_owned() const
	{
		return owned;
	}


	bool equals(const Buffer<T>& other) const
	{
		if (this == &other)
		{
			return true;
		}
		if (used != other.used)
		{
			return false;
		}
		for (size_t i = 0; i < used; ++i)
		{
			if (ptr[i] != other.ptr[i])
			{
				return false;
			}
		}

		return true;
	}


	/** @method resize(size_t count, bool copy)
	 *  Post-condition: size() == space() == <count>
	 *  If <copy> is true, up to <count> items will be at the new location.
	 */
	bool resize(size_t count, bool copy = false)
	{
		// no need to do anything
		if (owned && count == used)
		{
			return true;
		}

		T* tmp = 0;

		if (count > 0)
		{
			tmp = new T[count];
		}

		if (copy)
		{
			size_t limit = count < used ? count : used;
			for (size_t i = 0; i < limit; ++i)
			{
				tmp[i] = ptr[i];
			}
		}

		used = space = count;

		if (owned)
		{
			delete [] ptr;
		}

		ptr   = tmp;
		owned = true;

		return true;
	}


	/** @method adjust(size_t count, bool copy)
	 *  Post-condition: space() >= size() == <count>.
	 *  If <copy> is true, up to <count> items will be at the new location.
	 */
	bool adjust(size_t count, bool copy = false)
	{
		if (space >= count)
		{
			// easy, no reallocation necessary
			used = count;
			return true;
		}

		return resize(count, copy);
	}


	/** @method release ()
	 *  if owned, release ownership and return the data otherwise return NULL
	 */
	T* release()
	{
		if (!owned)
		{
			return 0;
		}

		T* tmp = const_cast<T*>(ptr);
		owned  = false;
		ptr    = 0;

		return tmp;
	}


	void swap(Buffer<T>& other)
	{
		bool o = owned;
		owned = other.owned;
		other.owned = o;

		const T* p = ptr;
		ptr = other.ptr;
		other.ptr = p;

		size_t t = used;
		used = other.used;
		other.used = t;

		t = space;
		space = other.space;
		other.space = t;
	}


	void reset(const T* p, size_t count, bool ownership)
	{
		if (owned && ptr)
		{
			delete [] ptr;
		}

		ptr   = p;
		used  = count;
		space = count;
		owned = ownership;
	}


	void copy(const T* p, size_t count)
	{
		Buffer tmp(p, count, false);
		*this = tmp;
	}


	/** @method alias ()
	 *  Make this Buffer an alias to the passed Buffer
	 */
	void alias(const Buffer<T>& other)
	{
		if (&other == this)
			return;

		if (owned && ptr)
			delete [] ptr;

		ptr   = other.ptr;
		owned = false;
		used  = other.used;
		space = other.space;
	}


	Buffer<T>& operator=(const Buffer<T>& other)
	{
		if (&other == this)
			return *this;

		if (owned && ptr)
			delete [] ptr;

		used   = 0;
		space  = 0;
		T* tmp = new T[other.used];
		used   = other.used;
		space  = used;

		for (size_t i = 0; i < used; ++i)
		{
			tmp[i] = other.ptr[i];
		}

		ptr   = tmp;
		owned = true;
		return *this;
	}


private:
	const T* ptr;
	bool     owned;
	size_t   used;
	size_t   space;
};


typedef Buffer<GMSEC_U8> DataBuffer;


} // namespace util
} // namespace api
} // namespace gmsec


#endif
