/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file List.h
 *
 * @brief This file contains the declaration for a list container, which is iterable.
 */
#ifndef GMSEC_API5_UTIL_LIST_H
#define GMSEC_API5_UTIL_LIST_H

#include <gmsec5/GmsecException.h>

#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api5
{
namespace util
{

/**
 * @class List
 *
 * @brief A container that models the STL list.
 *
 * Example usage:
 * @code
 * List<Field*> myFields;
 *
 * myFields.push_back(new StringField("FIELD-1", "one"));
 * myFields.push_back(new StringField("FIELD-2", "two"));
 * myFields.push_back(new StringField("FIELD-3", "three"));
 *
 * for (List<Field*>::const_iterator it = myFields.begin(); it!= myFields.end(); ++it)
 * {
 *     std::cout << (*it)->toXML() << std::endl;
 * }
 * @endcode
 */
template <class T>
class List
{
private:
	class ListIterator;

public:
	typedef ListIterator       iterator;
	typedef const ListIterator const_iterator;
	typedef T                  value_type;


	/**
	 * @fn List()
	 *
	 * @brief Default constructor
	 */
	List()
		: m_head(0),
		  m_tail(0),
		  m_size(0)
	{
	}


	/**
	 * @fn List(const T data[], size_t numElements)
	 *
	 * @brief Default constructor
	 */
	List(T elements[], size_t numElements)
		: m_head(0),
		  m_tail(0),
		  m_size(numElements)
	{
		assign(elements, numElements);
	}


	/**
	 * @fn List(const List<T>& other)
	 *
	 * @brief Copy constructor
	 *
	 * @param other - the other List to copy
	 */
	List(const List<T>& other)
		: m_head(0),
		  m_tail(0),
		  m_size(0)
	{
		for (const_iterator it = other.begin(); it != other.end(); ++it)
		{
			push_back(*it);
		}
	}


	/**
	 * @fn ~List()
	 *
	 * @brief Destructor; calls clear()
	 */
	~List()
	{
		clear();
	}


	/**
	 * @fn List<T>& operator=(const List<T>& other)
	 *
	 * @brief Overloaded assignment operator that makes a copy of the given List object.
	 *
	 * @param other - the other List to copy
	 *
	 * @returns The lhs List object.
	 */
	List<T>&  operator=(const List<T>& other)
	{
		if (this != &other)
		{
			clear();

      		for (const_iterator it = other.begin(); it != other.end(); ++it)
      		{
        		push_back(*it);
      		}
		}

		return *this;
	}


	/**
	 * @fn void push_front(const T& elem)
	 *
	 * @brief Inserts the given element to the front of the List.
	 *
	 * @param elem - the element to insert
	 */
	void  push_front(const T& elem)
	{
		if (m_head == 0)
		{
			m_head = new Node(elem, 0, 0);
			m_tail = m_head;
		}
		else
		{
			m_head = new Node(elem, 0, m_head);
		}
		++m_size;
	}


	/**
	 * @fn void push_back(const T& elem)
	 *
	 * @brief Inserts the given element to the end of the List.
	 *
	 * @param elem - the element to insert
	 */
	void  push_back(const T& elem)   // inserts at the end
	{
		if (m_head == 0)
		{
			m_head = new Node(elem, 0, 0);
			m_tail = m_head;
		}
		else
		{
			m_tail->m_next = new Node(elem, m_tail, 0);
			m_tail = m_tail->m_next;
		}
		++m_size;
	}


	/**
	 * @fn void pop_front()
	 *
	 * @brief Removes the element at the front of the List.
	 */
	void  pop_front()
	{
		Node* tmp = m_head->m_next;
		delete m_head;
		m_head = tmp;
		m_head->m_prev = 0;
		--m_size;
	}


	/**
	 * @fn void pop_back()
	 *
	 * @brief Removes the element at the end of the List.
	 */
	void  pop_back()         // deletes the last element
	{
		Node* tmp = m_tail;
		m_tail = tmp->m_prev;
		delete tmp;
		--m_size;
	}


	/**
	 * @fn void assign(T elements[], size_t numElements)
	 *
	 * @brief Clears the List, and then assigns the given element(s).
	 *
	 * @param elements    - the elements to add.
	 * @param numElements - the number of data elements.
	 */
	void  assign(T elements[], size_t numElements)
	{
		clear();

		for (size_t n = 0; n < numElements; ++n)
		{
			push_back(elements[n]);
		}
	}


	/**
	 * @fn void add(const T& elem)
	 *
	 * @brief Inserts the given element to the end of the List.
	 *
	 * @param elem - the element to insert
	 *
	 * @sa push_back()
	 */
	void  add(const T& elem)
	{
		push_back(elem);
	}


	/**
	 * @fn T& get(size_t index)
	 *
	 * @brief Returns the element located at the position indicated by index.
	 *
	 * @return The element at the given position index.
	 *
	 * @throw GmsecException is thrown if the index is out of range.
	 *
	 * @note This method is highly inefficient; it is better to iterate over
	 * a List than to attempt direct access based on an index.
	 */
	T&  get(size_t index)
	{
		listAssert("Index is out of range", index < m_size);

		iterator it = begin();

		for (size_t i = 0; i < index; ++i, ++it);

		return *it;
	}


	/**
	 * @fn const T& get(size_t index) const
	 *
	 * @brief Returns the element located at the position indicated by index.
	 *
	 * @return The element at the given position index.
	 *
	 * @throw GmsecException is thrown if the index is out of range.
	 *
	 * @note This method is highly inefficient; it is better to iterate over
	 * a List than to attempt direct access based on an index.
	 */
	const T&  get(size_t index) const
	{
		listAssert("Index is out of range", index < m_size);

		const_iterator it = begin();

		for (size_t i = 0; i < index; ++i, ++it);

		return *it;
	}


	/**
	 * @fn iterator insert(iterator& position, const T& elem)
	 *
	 * @brief Inserts the element immediately after the given iterator position.
	 *
	 * @param position - the iterator position
	 * @param elem     - the element to insert
	 *
	 * @return The iterator position of the element that was inserted.
	 */
	iterator  insert(iterator& position, const T& elem)
	{
		iterator it = begin();

		for (; it != position; ++it);

		it.m_node->m_prev->m_next = new Node(elem, it.m_node->m_prev, it.m_node);
		++m_size;

		return it.m_node->m_prev->m_next;
	}


	/**
	 * @fn iterator erase(iterator position)
	 *
	 * @brief Erases the element at the given interator position.
	 *
	 * @param position - the position of where to remove the element.
	 *
	 * @return The iterator position after the element that was removed.
	 */
	iterator  erase(iterator position)
	{
		if (position == end())
		{
			return position;
		}

		iterator it = begin();

		for (; it != position; ++it);

		it.m_node->m_prev->m_next = it.m_node->m_next;
		it.m_node->m_next->m_prev = it.m_node->m_prev;

		iterator next = it.m_node->m_next;

		delete it.m_node;

		--m_size;

		return next;
	}


	/**
	 * @fn void clear()
	 *
	 * @brief Clears the contents of the list.
	 *
	 * @note The user is responsible for managing the objects that are stored in the list.
	 */
	void  clear()
	{
		iterator it = begin();

		while (it != 0)
		{
			Node* node = it.m_node;
			++it;
			delete node;
		}

		m_head = 0;
		m_tail = 0;
		m_size = 0;
	}


	/**
	 * @fn size_t size() const
	 *
	 * @brief Returns the size of the List.
	 */
	size_t  size() const
	{
		return m_size;
	}


	/**
	 * @fn bool empty() const
	 *
	 * @brief Test whether list is empty.
	 */
	bool  empty() const
	{
		return m_size == 0;
	}


	/**
	 * @fn iterator begin()
	 *
	 * @brief Return an iterator to beginning.
	 */
	iterator  begin()
	{
		return m_head;
	}


	/**
	 * @fn const_iterator begin() const
	 *
	 * @brief Return a const iterator to beginning.
	 */
	const_iterator  begin() const
	{
		return m_head;
	}


	/**
	 * @fn iterator rbegin()
	 *
	 * @brief Return a reverse iterator to reverse beginning.
	 */
	iterator  rbegin()
	{
		return ListIterator(m_tail, true);
	}


	/**
	 * @fn const_iterator rbegin() const
	 *
	 * @brief Return a const reverse iterator to reverse beginning.
	 */
	const_iterator  rbegin() const
	{
		return ListIterator(m_tail, true);
	}


	/**
	 * @fn iterator end()
	 *
	 * @brief Return an iterator to end.
	 */
	iterator  end()
	{
		return 0;
	}


	/**
	 * @fn iterator end()
	 *
	 * @brief Return a const iterator to end.
	 */
	const_iterator  end() const
	{
		return 0;
	}


	/**
	 * @fn iterator rend()
	 *
	 * @brief Return a reverse iterator to reverse end.
	 */
	iterator  rend()
	{
		return 0;
	}


	/**
	 * @fn const_iterator rend() const
	 *
	 * @brief Return a const reverse iterator to reverse end.
	 */
	const_iterator  rend() const
	{
		return 0;
	}


	/**
	 * @fn T& front()
	 *
	 * @brief Return reference to element at front of list.
	 *
	 * @throw GmsecException is thrown if the list is empty.
	 */
	T&  front()
	{
		listAssert("List is empty", m_size != 0);
		return m_head->m_elem;
	}


	/**
	 * @fn const T& front() const
	 *
	 * @brief Return reference to element at front of list.
	 *
	 * @throw GmsecException is thrown if the list is empty.
	 */
	const T&  front() const
	{
		listAssert("List is empty", m_size != 0);
		return m_head->m_elem;
	}


	/**
	 * @fn T& back()
	 *
	 * @brief Return reference to element at end of list.
	 *
	 * @throw GmsecException is thrown if the list is empty.
	 */
	T&  back()
	{
		listAssert("List is empty", m_size != 0);
		return m_tail->m_elem;
	}


	/**
	 * @fn const T& back() const
	 *
	 * @brief Return reference to element at end of list.
	 *
	 * @throw GmsecException is thrown if the list is empty.
	 */
	const T&  back() const
	{
		listAssert("List is empty", m_size != 0);
		return m_tail->m_elem;
	}


private:
	static void listAssert(const char* reason, bool asserted)
	{
		if (!asserted)
		{
			throw gmsec::api5::GmsecException(OTHER_ERROR, ITER_INVALID_NEXT, reason);
		}
	}

	struct Node
	{
		Node(T elem, Node* prev, Node* next) : m_elem(elem), m_prev(prev), m_next(next) {}
		bool operator>(const Node& other)  { return m_elem > other.m_elem;  }
		bool operator<(const Node& other)  { return m_elem < other.m_elem;  }
		bool operator==(const Node& other) { return m_elem == other.m_elem; }

		T     m_elem;
		Node* m_prev;
		Node* m_next;
	};


	class ListIterator
	{
	public:
		typedef T iterator_category;

		ListIterator(Node* head, bool reverse = false) : m_node(head), m_reverse(reverse) {}

		ListIterator(const ListIterator& it) : m_node(it.m_node) {}

		bool operator==(const ListIterator& other) const { return m_node == other.m_node; }
		bool operator!=(const ListIterator& other) const { return m_node != other.m_node; }

		void operator++() const // goto the next element
		{
			if (m_reverse)
			{
				listAssert("At the end of the list; unable to advance iterator", m_node != 0);
				m_node = m_node->m_prev;
			}
			else
			{
				listAssert("At the end of the list; unable to advance iterator", m_node != 0);
				m_node = m_node->m_next;
			}
		}

		void operator++(int) const
		{
			if (m_reverse)
			{
				listAssert("At the end of the list; unable to advance iterator", m_node != 0);
				m_node = m_node->m_prev;
			}
			else
			{
				listAssert("At the end of the list; unable to advance iterator", m_node != 0);
				m_node = m_node->m_next;
			}
		}

		void operator--() const // goto the previous element
		{
			if (m_reverse)
			{
				listAssert("At the beginning of the list; unable to decrement iterator", m_node != 0);
				m_node = m_node->m_next;
			}
			else
			{
				listAssert("At the beginning of the list; unable to decrement iterator", m_node != 0);
				m_node = m_node->m_prev;
			}
		}

		void operator--(int) const
		{
			if (m_reverse)
			{
				listAssert("At the beginning of the list; unable to decrement iterator", m_node != 0);
				m_node = m_node->m_next;
			}
			else
			{
				listAssert("At the beginning of the list; unable to decrement iterator", m_node != 0);
				m_node = m_node->m_prev;
			}
		}

		const T& operator*() const // access the current element
		{
			listAssert("No data to reference", m_node != 0);
			return m_node->m_elem;
		}

		T& operator*() // access the current element
		{
			listAssert("No data to reference", m_node != 0);
			return m_node->m_elem;
		}

	private:
		friend class List<T>;
		mutable Node* m_node;
		bool          m_reverse;
	};


	// Data Members
	Node*  m_head;
	Node*  m_tail;
	size_t m_size;
};

} // end namespace util
} // end namespace api5
} // end namespace gmsec

#endif
