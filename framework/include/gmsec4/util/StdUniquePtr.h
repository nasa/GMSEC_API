/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file StdUniquePtr.h
 *
 * @brief This file serves as a wrapper for both the std::auto_ptr
 * and the std::unique_ptr, for cases when the latter is not available.
 *
 * @note This module exists in the GMSEC API's public space because
 * it is needed by certain utilities. Later, when GMSEC ceases
 * supporting legacy systems (such as RHEL 6), the purpose of this
 * module will be moot. Users should refrain from using this module.
 *
 */

#ifndef GMSEC_API_UTIL_STD_UNIQUE_PTR_H
#define GMSEC_API_UTIL_STD_UNIQUE_PTR_H

#include <gmsec4/util/cxx.h>

#include <memory>


namespace gmsec {
namespace api   {
namespace util  {


template <typename T>
class StdUniquePtr
{
public:
	/**
	 * @fn StdUniquePtr(T* p = 0)
	 *
	 * @brief Default constructor.
	 *
	 * @param p - Pointer to object of type T; if not provided, default is NULL.
	 */
	explicit StdUniquePtr(T* p = 0)
		: m_ptr(p)
	{
	}

	/**
	 * @fn ~StdUniquePtr()
	 *
	 * @brief Destructor; associated pointer of this class is automatically destroyed.
	 */
	~StdUniquePtr()
	{
	}

	/**
	 * @fn StdUniquePtr(StdUniquePtr<T>& other)
	 *
	 * @brief Copy constructor
	 *
	 * @param other - The other object whose contents should be copied.
	 */
	StdUniquePtr(StdUniquePtr<T>& other)
		: m_ptr(new T(*(other.get())))
	{
	}

	/**
	 * @fn T* get() const
	 *
	 * @brief Returns the immutable pointer to the contents of the StdUniquePtr.
	 */
	T* get() const
	{
		return m_ptr.get();
	}

	/**
	 * @fn T* get()
	 *
	 * @brief Returns the pointer to the contents of the StdUniquePtr.
	 */
	T* get()
	{
		return m_ptr.get();
	}

	/**
	 * @fn T* release()
	 *
	 * @brief Releases ownership of the contents of the StdUniquePtr.
	 *
	 * @return The pointer previously held by the StdUniquePtr.
	 */
	T* release()
	{
		return m_ptr.release();
	}

	/**
	 * @fn void reset(T* p = 0)
	 *
	 * @brief Resets the contents of the StdUniquePtr with a new object, or NULL if none is provided.
	 *
	 * @param p - Pointer to object of type T; if not provided, default is NULL.
	 */
	void reset(T* p = 0)
	{
		if (p != m_ptr.get())
		{
			m_ptr.reset(p);
		}
	}

	/**
	 * @fn void swap(StdUniquePtr<T>& other)
	 *
	 * @brief Swaps the contents of another StdUniquePtr object with this one.
	 *
	 * @param other - The other StdUniquePtr object with whom to swap content values.
	 */
	void swap(StdUniquePtr<T>& other)
	{
		T* tmp = m_ptr.release();
		m_ptr.reset(other.release());
		other.reset(tmp);
	}

	/**
	 * @fn StdUniquePtr<T>& operator=(StdUniquePtr<T>& other)
	 *
	 * @brief Assignment operator.
	 *
	 * @param other - The other StdUniquePtr whose contents to assign.
	 *
	 * @note The given StdUniquePtr, by its very nature, will stripped of its contents
	 * following the call to this method.
	 */
	StdUniquePtr<T>& operator=(StdUniquePtr<T>& other)
	{
		reset(other.release());
		return *this;
	}

	/**
	 * @fn T& operator*() const
	 *
	 * @brief Overloaded operator to return an immutable reference to the contents of the StdUniquePtr.
	 */
	T& operator*() const
	{
		return *m_ptr;
	}

	/**
	 * @fn T& operator*()
	 *
	 * @brief Overloaded operator to return a reference to the contents of the StdUniquePtr.
	 */
	T& operator*()
	{
		return *m_ptr;
	}

	/**
	 * @fn T* operator->() const
	 *
	 * @brief Overloaded operator to return an immutable pointer to the contents of the StdUniquePtr.
	 */
	T* operator->() const
	{
		return &(*m_ptr);
	}

	/**
	 * @fn T* operator->()
	 *
	 * @brief Overloaded operator to return a pointer to the contents of the StdUniquePtr.
	 */
	T* operator->()
	{
		return &(*m_ptr);
	}

private:
#if GMSEC_CXX_11_AVAILABLE == 1
	std::unique_ptr<T> m_ptr;
#else
	std::auto_ptr<T>   m_ptr;
#endif
};

} // end namespace util
} // end namespace api
} // end namespace gmsec

#endif
