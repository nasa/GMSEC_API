/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_UTIL_GSHARED_PTR_H
#define GMSEC_API5_UTIL_GSHARED_PTR_H


#include <gmsec5/util/Atomics.h>

#include <gmsec5_defs.h>


namespace gmsec {
namespace api5 {
namespace util {


/**
 * @class StdSharedPtr
 *
 * @brief A smart pointer that retains shared ownership of an
 * object of type-T through a pointer. The retained object is
 * deallocated when the last object owning the shared smart
 * pointer is destroyed, or the object is reset with another.
 */
template <class T> class StdSharedPtr
{
public:
	/**
	 * @fn StdSharedPtr<T>()
	 *
	 * @brief Constructs a shared smart pointer that is initialized to null.
	 */
	StdSharedPtr<T> ()
			: ptr(0), stash(0)
	{
	}


	/**
	 * @fn StdSharedPtr<T>(T *t)
	 *
	 * @brief Constructs a shared smart pointer that is initialized to the
	 * given object of type-T.
	 *
	 * @param t - the pointer to the object that will be managed by the smart pointer.
	 */
	StdSharedPtr<T> (T *t)
			: ptr(t), stash(0)
	{
		if (ptr)
			stash = new Stash;
	}


	/**
	 * @fn StdSharedPtr<T>(const StdSharedPtr<T> &other)
	 *
	 * @brief The copy-constructor.
	 *
	 * @param other - the smart pointer object to copy.
	 *
	 * @note Both the original and the new smart pointer will share
	 * the same underlying object of type-T.
	 */
	StdSharedPtr<T>(const StdSharedPtr<T> &other)
			:
			ptr(other.ptr),
			stash(0)
	{
		if (other.stash)
			attach(other.stash);
	}


	/**
	 * @fn ~StdSharedPtr<T>()
	 *
	 * @brief The destructor that releases (deallocates) the underlying object.
	 */
	~StdSharedPtr<T>()
	{
		release();
	}


	/**
	 * @fn count() const
	 *
	 * @brief Provides the reference count of the underlying object, if any.
	 *
	 * @returns A number greater than zero if the smart pointer is associated
	 * with an object, otherwise zero.
	 */
	int  count() const
	{
		if (!stash)
			return 0;
		return stash->count;
	}


	/**
	 * @fn get()
	 *
	 * @brief Returns a non-const pointer that references the underlying object
	 * managed by the smart pointer.
	 *
	 * @returns A pointer to an object of type-T, or NULL if no object is associated with the smart pointer.
	 */
	T*  get()
	{
		return ptr;
	}


	/**
	 * @fn get()
	 *
	 * @brief Returns a const pointer that references the underlying object
	 * managed by the smart pointer.
	 *
	 * @returns A pointer to an object of type-T, or NULL if no object is associated with the smart pointer.
	 */
	const T*  get() const
	{
		return ptr;
	}


	/**
	 * @fn reset()
	 *
	 * @brief Releases (deallocates) the underlying object managed by the
	 * smart pointer.
	 */
	void  reset()
	{
		release();
	}


	/**
	 * @fn reset(T *t)
	 *
	 * @brief Releases (deallocates) the underlying object managed by the
	 * smart pointer, and then replaces such with the given object, if any.
	 */
	void  reset(T *t)
	{
		release();
		if (t)
			stash = new Stash;
		ptr = t;
	}


	/**
	 * @fn operator->()
	 *
	 * @brief Returns a pointer that references the underlying object managed by the smart pointer.
	 *
	 * @returns A pointer to an object of type-T, or NULL if no object is associated with the smart pointer.
	 */
	T*  operator-> ()
	{
		return ptr;
	}


	/**
	 * @fn operator->() const
	 *
	 * @brief Returns a pointer that references the underlying object managed by the smart pointer.
	 *
	 * @returns A pointer to an object of type-T, or NULL if no object is associated with the smart pointer.
	 */
	const T*  operator-> () const
	{
		return ptr;
	}


	/**
	 * @fn operator*()
	 *
	 * @brief Returns a reference to the underlying object managed by the smart pointer.
	 *
	 * @returns A pointer to an object of type-T, or NULL if no object is associated with the smart pointer.
	 */
	T&  operator*()
	{
		return *ptr;
	}


	/**
	 * @fn operator=(StdSharedPtr<T> &rhs)
	 *
	 * @brief The overloaded assignment operator method that can be used
	 * to assign one smart pointer to another.
	 *
	 * @param rhs - the smart pointer object to assign/copy to the existing smart pointer.
	 *
	 * @note Both the original and the new smart pointer will share
	 * the same underlying object of type-T.
	 */
	StdSharedPtr<T>&  operator= (StdSharedPtr<T> &rhs)
	{
		if (this == &rhs)
			return *this;
		release();
		ptr = rhs.ptr;
		attach(rhs.stash);
		return *this;
	}

	/**
	 * @fn operator bool()
	 *
	 * @brief Reports whether an object is being managed by the smart pointer.
	 *
	 * @returns True if an object is being managed, false otherwise.
	 */
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
} // namespace api5
} // namespace gmsec

#endif
