/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file List.h
 *
 *  @brief This file contains a class for cross platform List support
 *
 *  @note This implementation of the List class was created so that users
 *  can interact with objects that are stored in the GMSEC internal namespace.
 *
**/

#ifndef gmsec_util_List_h
#define gmsec_util_List_h

#include <gmsec/util/Deprecated.h>

#include <vector>
#include <cstddef>


namespace gmsec
{
namespace util
{

/**	@class List
 *
 *	@brief This provides a basic List, with an API like the Java list
 *
 *
**/
template <class elemType>
class List
{
#ifndef GMSEC_BINCOMPAT_LIST
public:

	/* @fn List()
	*
	*	@brief Standard constructor. 
	*
	*/
	GMSEC_DEPRECATED List(){

	}

	/* @fn add(elemType element)
	*
	*	@brief This function appends the element to the end of the list
	*
	*	@param element - The element to be appended.
	*
	*/
	void add (elemType element){
		internalVector.push_back(element);
	}

	/* @fn remove(elemType element)
	*
	*	@brief This function removes the element from the list, which is then destroyed.
	*
	*	@param element - The element to be removed.
	*
	*/
	bool remove(elemType element){
		bool found = false;
		size_t s = internalVector.size();

		for(size_t i = 0; i < s; i++)
		{
			if(internalVector.at(i) == element)
			{
				internalVector.erase(internalVector.begin() + i);
				found = true;
				break;
			}
		}

		return found;
	}

	/* @fn clear()
	*
	*	@brief This function clears the contents of the list
	*
	*	@note This function empties the list and calls the destructors
	*	of the objects it contains. Users are responsible for calling this
	*	function, or remove(elemType element), to destroy the data held
	*	within the List.
	* 
	*/
	void clear(){
		internalVector.clear();
	}

	/* @fn get(int idx)
	*
	*	@brief This function retrieves the element at the desired index
	*
	*	@param idx - The index of the element to be retreived
	*
	* 	@note Accessing elements in a gmsec::util::List is allowed, and was an intended choice
	* 	of the GMSEC API team
	*
	*/
	bool get(int idx, elemType &element){
		if((unsigned int)idx < internalVector.size()){
			element = internalVector.at(idx);
			return true;
		}else{
			return false;
		}
	}

	/* @fn size()
	*
	*	@brief This function retrieves length of the list
	*
	*/
	int size(){
		return (int) internalVector.size();
	}
private:
	std::vector<elemType> internalVector;
#else
private:
	struct Node
	{
		Node(elemType elem, Node* next, Node* prev)
			: m_elem(elem),
			m_next(next),
			m_prev(prev)
		{
		}

		elemType m_elem;
		Node* m_next;
		Node* m_prev;
	};
 
	size_t count;
	Node* m_head;
	Node* m_tail;

public:

	/* @fn List()
	*
	*	@brief Standard constructor. 
	*
	*/
	List()	: count(0),
		m_head(0),
		m_tail(0)
	{
	}

	/* @fn ~List()
 	* 
 	*	@brief List destructor
 	*
 	*	@note This function calls gmsec::util::List::clear(), which
 	*	deletes any data that was allocated for objects in the List.
 	*
 	*/
	~List() {
		clear();
	}

	/* @fn add(elemType element)
	*
	*	@brief This function appends the element to the end of the list
	*
	*	@param element - The element to be appended.
	*
	*/
	void add (elemType element){
		if(m_head == 0)
		{
			m_head = new Node(element, 0, 0);
			m_tail = m_head;
		}
		else
		{
			// Add a Node to the end of the list
			m_tail = new Node(element, 0, m_tail);
			m_tail->m_prev->m_next = m_tail;
		}

		count++;
	}

	/* @fn remove(elemType element)
	*
	*	@brief This function removes the first occurrence of the element from the list,
	*	then destroys it.
	*
	*	@param element - The element to be removed.
	*
	*/
	bool remove (elemType element){
		Node* node = m_head;
		bool found = false;

		while(node != 0)
		{
			if(node->m_elem == element)
			{
				// Element found in between head and tail, or is the tail
				if (node->m_prev)
				{
					node->m_prev->m_next = node->m_next;

					if (node->m_next)
					{
						node->m_next->m_prev = node->m_prev;
					}
					// Node is the tail
					else
					{
						m_tail = node->m_prev;
					}
				}
				// Node is the head
				else
				{
					m_head = node->m_next;
					m_head->m_prev = 0;
				}

				delete node;

				found = true;
				count--;
				break;
			}

			node = node->m_next;
		}

		return found;
	}

	/* @fn clear()
	*
	*	@brief This function clears the contents of the list
	*
	*	@note This function deletes any data that was allocated
	*	for the objects in the List.
	*
	*/
	void clear(){
		Node* node = m_head;

		while(node != 0)
		{
			Node* tmp = node;
			node = node->m_next;
			delete tmp;
		}

		m_head = 0;
		m_tail = 0;
		count = 0;
	}

	/* @fn get(int idx)
	*
	*	@brief This function retrieves the element at the desired index
	*
	*	@param idx - The array-numbered index of the element to be retreived
	*
	* 	@note Accessing elements in a gmsec::util::List is allowed, and was an intended choice
	* 	of the GMSEC API team
	*
	*/
	bool get(int idx, elemType &element){

		if (idx < 0 || idx > count || count == 0)
			return false;
		else
		{
			int counter = 0;
			Node* node = m_head;

			while(node != 0)
			{
				if(idx == counter)
				{
					element = node->m_elem;
					break;
				}

				node = node->m_next;
				counter++;
			}

			if(node == 0)
				return false;

			return true;
		}
	}

	/* @fn size()
	*
	*	@brief This function retrieves length of the list
	*
	*/
	size_t size(){
		return count;
	}
#endif
};

}//namespace util
}//namespace gmsec
#endif
