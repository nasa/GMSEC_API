
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseCompoundFieldDescriptor.h
 *
 *  @brief This file contains the BaseCompoundFieldDescriptor class,
 *         which desribes Fields.
 *
**/

#ifndef gmsec_BaseCompoundFieldDescriptor_h
#define gmsec_BaseCompoundFieldDescriptor_h


#include <gmsec/mist/MIST_errors.h>

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/AbstractBaseFieldDescriptor.h>
#include <gmsec/internal/mist/BaseContext.h>
#include <gmsec/internal/mist/FieldAccessTemplate.h>
#include <gmsec/internal/mist/DescriptorList.h>

#include <gmsec/Message.h>

#include <gmsec/internal/strutil.h>


namespace gmsec
{

namespace mist
{

namespace internal
{
class AbstractFieldDescriptor;
class BaseTester;


/** @class BaseCompoundFieldDescriptor
 * @brief The BaseCompoundFieldDescriptor object represents a compound field
 * that contains subfields represented as BaseAbstractFieldDescriptors.  It 
 * extends the BaseAbstractFieldDescriptor and adds getters and setters for
 * the DescriptorList that stores the AbstractBaseFieldDescriptor list
 *
 * @sa gmsec::mist::internal::AbstractBaseFieldDescriptor
 * @sa gmsec::mist::CompoundFieldDescriptor
 * @sa gmsec::mist::internal::DescriptorList
 * @sa gmsec::mist::Context
*/
class GMSEC_API BaseCompoundFieldDescriptor : public AbstractBaseFieldDescriptor
{
public:

	/** @fn BaseCompoundFieldDescriptor()
	 * @brief Default constructor - initializes the BaseCompoundFieldDescriptor
	 * instance, calls the AbstractBaseFieldDescriptor default constructor and
	 * sets the children to null
	 *
	 */
	BaseCompoundFieldDescriptor();

	/** @fn BaseCompoundFieldDescriptor(util::List<AbstractFieldDescriptor *> &newChildren)
	 * @brief Constructor - initializes the BaseCompoundFieldDescriptor
	 * instance, calls the AbstractBaseFieldDescriptor default constructor and
	 * sets the children to a DescriptorList generated with newChildren, taking 
	 * ownership of the variable
	 *
	 * @param newChildren - list of AbstractFieldDescriptor used to generate
	 * DescriptorList.  Ownership is taken by BaseCompoundFieldDescriptor
	 *
	 */
	BaseCompoundFieldDescriptor(util::List<AbstractFieldDescriptor *> &newChildren);

	/** @fn ~BaseContext()
	 * @brief Destructor - cleans up the children list
	 */
	virtual ~BaseCompoundFieldDescriptor();

	/** @fn AddTo(Message &message, Context &context) const
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
	virtual Status AddTo(Message *message, Context &context) const;

	/** @fn Validate(Message *message, Context &context) const
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
	virtual Status Validate(Message *message, Context &context) const;

	/** @fn Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
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
	virtual Status Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const;

	/** @fn FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
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
	virtual Status FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const;

	/** @fn GetChildren(DescriptorList* &children) const
	 * @brief Returns a pointer to the children list, not a copy
	 *
	 * @param children - The pointer to a reference of a DescriptorList that
	 * is set to the children member 
	 *
	 * @return status - result of operation
	 */
	Status GetChildren(DescriptorList* &children) const;

	/** @fn AddChild(AbstractFieldDescriptor &fd)
	 * @brief Adds the AbstractFieldDescriptor parameter to the children list and
	 * takes ownership of the fd parameter
	 *
	 * @param fd - The AbstractFieldDescriptor is added directly to the children
	 * list, not a copy
	 *
	 * @return status - result of operation
	 */
	Status AddChild(AbstractFieldDescriptor &fd);

	/** @fn SetChildren(util::List<AbstractFieldDescriptor*> &fdlist)
	 * @brief Adds the AbstractFieldDescriptor list parameter as the children list
	 * and takes ownership of fdlist
	 *
	 * @param fdlist - The AbstractFieldDescriptor list is added as the children
	 * list, not a copy
	 *
	 * @return status - result of operation
	 */
	Status SetChildren(util::List<AbstractFieldDescriptor*> &fdlist);

private:
	BaseCompoundFieldDescriptor &operator=(const BaseCompoundFieldDescriptor &);
	BaseCompoundFieldDescriptor(const BaseCompoundFieldDescriptor &);

	DescriptorList *children;

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
