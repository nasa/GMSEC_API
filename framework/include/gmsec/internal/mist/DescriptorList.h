
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file DescriptorList.h
 *
 *  @brief This file contains a class to store a list of FieldDescriptors
 *
**/

#ifndef gmsec_DescriptorList_h
#define gmsec_DescriptorList_h

#include <gmsec/Status.h>

#include <gmsec/util/List.h>

namespace gmsec
{

namespace mist
{

namespace internal
{

class AbstractFieldDescriptor;

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
private:
	util::List<AbstractFieldDescriptor*> fields;

public:
	DescriptorList();
	DescriptorList(util::List<AbstractFieldDescriptor*> &newFields);
	~DescriptorList();
	Status SetFields(util::List<AbstractFieldDescriptor*> &newFields);
	Status AddDescriptor(AbstractFieldDescriptor &fd);
	bool IsEmpty();
	Status Get(int i, AbstractFieldDescriptor* &fd);
	int Size();

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
