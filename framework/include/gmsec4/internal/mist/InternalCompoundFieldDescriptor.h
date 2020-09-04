/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalCompoundFieldDescriptor.h
 *
 *  @brief This file contains the InternalCompoundFieldDescriptor class,
 *         which desribes Fields.
 *
**/

#ifndef GMSEC_MIST_INTERNAL_COMPOUND_FIELD_DESCRIPTOR_H
#define GMSEC_MIST_INTERNAL_COMPOUND_FIELD_DESCRIPTOR_H


#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/AbstractBaseFieldDescriptor.h>
#include <gmsec4/internal/mist/InternalContext.h>
#include <gmsec4/internal/mist/FieldAccessTemplate.h>
#include <gmsec4/internal/mist/DescriptorList.h>

#include <gmsec4/Message.h>

#include <gmsec4/internal/StringUtil.h>

#include <vector>


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


/** @class InternalCompoundFieldDescriptor
 * @brief The InternalCompoundFieldDescriptor object represents a compound field
 * that contains subfields represented as InternalAbstractFieldDescriptors.  It 
 * extends the InternalAbstractFieldDescriptor and adds getters and setters for
 * the DescriptorList that stores the AbstractInternalFieldDescriptor list
 *
 * @sa gmsec::mist::internal::AbstractInternalFieldDescriptor
 * @sa gmsec::mist::CompoundFieldDescriptor
 * @sa gmsec::mist::internal::DescriptorList
 * @sa gmsec::mist::Context
*/
class GMSEC_API InternalCompoundFieldDescriptor : public AbstractBaseFieldDescriptor
{
public:
	/** @fn InternalCompoundFieldDescriptor()
	 * @brief Default constructor - initializes the InternalCompoundFieldDescriptor
	 * instance, calls the AbstractInternalFieldDescriptor default constructor and
	 * sets the children to null
	 *
	 */
	InternalCompoundFieldDescriptor();


	/** @fn InternalCompoundFieldDescriptor(std::vector<AbstractFieldDescriptor*>& newChildren)
	 * @brief Constructor - initializes the InternalCompoundFieldDescriptor
	 * instance, calls the AbstractInternalFieldDescriptor default constructor and
	 * sets the children to a DescriptorList generated with newChildren, taking 
	 * ownership of the variable
	 *
	 * @param newChildren - list of AbstractFieldDescriptor used to generate
	 * DescriptorList.  Ownership is taken by InternalCompoundFieldDescriptor
	 *
	 */
	InternalCompoundFieldDescriptor(std::vector<AbstractFieldDescriptor*>& newChildren);


	/** @fn ~InternalContext()
	 * @brief Destructor - cleans up the children list
	 */
	virtual ~InternalCompoundFieldDescriptor();


	/** @fn addTo(Message& message, Context& context) const
	 * @brief AddTo function calls each child FieldDescriptor's AddTo function
	 * which generates the field(s) represented the childrens' field
	 *
	 * @param message - The fields represented by the children FieldDescriptors'
	 * are added to the reference Message
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	virtual Status addTo(Message& message, Context& context) const;


	/** @fn validate(Message& message, Context& context) const
	 * @brief Calls the children FieldDescriptors' Validate function
	 * which validates the Message based on the Field represented by each
	 * child FieldDescriptor
	 *
	 * @param message - The fields contained in the reference Message are
	 * validated against the fields represented by the children FieldDescriptor
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	virtual Status validate(Message& message, Context& context) const;


	/** @fn apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
	 * @brief Apply calls the children FieldDescriptors' Apply function
	 * which fills in the Message based on the Fields represented by the children
	 * and validates the fields already in Message if the enum validate is
	 * set to VALIDATE_ON_APPLY
	 *
	 * @param message - The fields represented by the children are added
	 * to the Message if they do not already exist.  The fields already contained
	 * in the reference Message are validated if enum validate is set to
	 * VALIDATE_ON_APPLY
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 * @param validate - The enum determines if validation occurs and can be set to
	 * either VALIDATE_ON_APPLY or NO_VALIDATE_ON_APPLY
	 *
	 * @return status - result of operation
	 */
	virtual Status apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const;


	/** @fn findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
	 * @brief Function that calls the children's FindField function
	 * which is the helper function to the Apply, Instantiate, and Validate
	 * functions
	 *
	 * @param message - The reference to the Message is updated based on either
	 * Apply, Instantiate, or Validate
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 * @param validate - The enum determines if validation occurs and can be set to
	 * VALIDATE, VALIDATE_ON_APPLY, or NO_VALIDATE_ON_APPLY
	 *
	 * @return status - result of operation
	 */
	virtual Status findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const;


	/** @fn getChildren(DescriptorList*& children) const
	 * @brief Returns a pointer to the children list, not a copy
	 *
	 * @param children - The pointer to a reference of a DescriptorList that
	 * is set to the children member 
	 *
	 * @return status - result of operation
	 */
	Status getChildren(DescriptorList*& children) const;


	/** @fn addChild(AbstractFieldDescriptor& fd)
	 * @brief Adds the AbstractFieldDescriptor parameter to the children list and
	 * takes ownership of the fd parameter
	 *
	 * @param fd - The AbstractFieldDescriptor is added directly to the children
	 * list, not a copy
	 *
	 * @return status - result of operation
	 */
	Status addChild(AbstractFieldDescriptor& fd);


	/** @fn setChildren(std::list<AbstractFieldDescriptor*>& fdlist)
	 * @brief Adds the AbstractFieldDescriptor list parameter as the children list
	 * and takes ownership of fdlist
	 *
	 * @param fdlist - The AbstractFieldDescriptor list is added as the children
	 * list, not a copy
	 *
	 * @return status - result of operation
	 */
	Status setChildren(std::vector<AbstractFieldDescriptor*>& fdlist);


private:
	// Defined, but not implemented.
	InternalCompoundFieldDescriptor(const InternalCompoundFieldDescriptor&);
	InternalCompoundFieldDescriptor& operator=(const InternalCompoundFieldDescriptor&);


	DescriptorList* m_children;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
