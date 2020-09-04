/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file DescriptorList.h
 *
 *  @brief This file contains a class to store a list of FieldDescriptors
 *
**/

#ifndef GMSEC_MIST_DESCRIPTOR_LIST_H
#define GMSEC_MIST_DESCRIPTOR_LIST_H

#include <gmsec4/Status.h>

#include <vector>

#include <cstddef>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{
	// Forward declaration(s)
	class AbstractFieldDescriptor;
	class InternalTester;


/** @class DescriptorList
 * @brief The DescriptorList object is a list for AbstractFieldDescriptor.  The
 * methods of this class allow for adding and removing of AbstractFieldDescriptor
 * objects and cleans up memory used by the AbstractFieldDescriptor elements
 * on delete
 *
 * @sa gmsec::mist::AbstractFieldDescriptor
*/
class GMSEC_API DescriptorList
{
public:
	DescriptorList();

	DescriptorList(std::vector<AbstractFieldDescriptor*>& newFields);

	~DescriptorList();

	void setFields(std::vector<AbstractFieldDescriptor*>& newFields);

	void addDescriptor(AbstractFieldDescriptor* fd);

	bool isEmpty() const;

	void resetIterator();

	bool hasNext() const;

	AbstractFieldDescriptor& next();

	size_t size() const;

	AbstractFieldDescriptor& getDescriptor(size_t index) const;

private:
	typedef std::vector<AbstractFieldDescriptor*> FieldDescriptorList;

	FieldDescriptorList           m_fields;
	FieldDescriptorList::iterator m_iter;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
