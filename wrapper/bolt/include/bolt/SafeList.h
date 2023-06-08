/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef bolt_safe_list_h
#define bolt_safe_list_h

/*
 * @file SafeList.h
 *
 * @brief Provides a thread-safe list, which in essence is an std::list protected by a mutex lock.
 *
 * @note Not all std::list methods are implemented; just those that are needed by Bolt.
 */

#include <list>
#include <mutex>


namespace bolt
{

template <class T>
class SafeList
{
public:
	typedef typename std::list<T>::iterator       SafeListIterator;
	typedef typename std::list<T>::const_iterator SafeListConstIterator;


	SafeList()
		: m_mutex(), m_list()
	{
	}

	SafeList(const SafeList<T>& other)
		: m_mutex(), m_list()
	{
		const std::lock_guard<std::mutex> lock(other.m_mutex);

		m_list = other.m_list;
	}

	~SafeList()
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		m_list.clear();
	}

	SafeList& operator=(const SafeList<T>& other)
	{
		if (this != &other)
		{
			const std::lock_guard<std::mutex> lock1(m_mutex);
			const std::lock_guard<std::mutex> lock2(other.m_mutex);

			m_list = other.m_list;
		}

		return *this;
	}

	SafeListIterator begin()
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.begin();
	}

	SafeListConstIterator begin() const
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.begin();
	}

	SafeListIterator end()
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.end();
	}

	SafeListConstIterator end() const
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.end();
	}

	bool empty() const
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.empty();
	}

	size_t size() const
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.size();
	}

	void push_front(const T& elem)
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		m_list.push_front(elem);
	}

	void push_back(const T& elem)
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		m_list.push_back(elem);
	}

	SafeListIterator insert(SafeListIterator& pos, const T& elem)
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.insert(pos, elem);
	}

	SafeListIterator erase(SafeListIterator& pos)
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		return m_list.erase(pos);
	}

	void clear()
	{
		const std::lock_guard<std::mutex> lock(m_mutex);

		m_list.clear();
	}

private:
	mutable std::mutex  m_mutex;
	std::list<T>        m_list;
};

} // end namespace

#endif
