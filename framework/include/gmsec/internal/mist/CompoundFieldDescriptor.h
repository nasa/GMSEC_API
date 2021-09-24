
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file CompoundFieldDescriptor.h
 *
 *  @brief This file contains the the CompoundFieldDescriptor class,
 *  which is one of the MIST classes which desribe Fields.
 *
**/

#ifndef gmsec_CompoundFieldDescriptor_h
#define gmsec_CompoundFieldDescriptor_h


#include <gmsec/internal/mist/AbstractFieldDescriptor.h>


namespace gmsec
{

namespace mist
{
namespace internal
{
class BaseTester;


/** @class CompoundFieldDescriptor
 * @brief The CompoundFieldDescriptor object is a wrapper class for
 * BaseCompoundFieldDescriptor and extends the AbstractFieldDescriptor. 
 * It allows for the setting of AbstractFieldDescriptors representing
 * subfields through the constructor.
 *
 * @sa gmsec::mist::AbstractFieldDescriptor
 * @sa gmsec::mist::internal::BaseCompoundFieldDescriptor
 * @sa gmsec::mist::Context
*/
class GMSEC_API CompoundFieldDescriptor : public AbstractFieldDescriptor
{
public:

	/** @fn CompoundFieldDescriptor()
	 * @brief Default constructor - initializes the CompoundFieldDescriptor
	 * instance and BaseCompoundFieldDescriptor pointer, calls the
	 * AbstractBaseFieldDescriptor default constructor and sets the children to null
	 *
	 */
	CompoundFieldDescriptor();

	/** @fn CompoundFieldDescriptor(util::List<AbstractFieldDescriptor *> &newChildren)
	 * @brief Constructor - initializes the CompoundFieldDescriptor instance and
	 * BaseCompoundFieldDescriptor pointer, calls the AbstractBaseFieldDescriptor
	 * default constructor and sets the children to a DescriptorList generated with
	 * newChildren, taking ownership of the variable
	 *
	 * @param newChildren - list of AbstractFieldDescriptor used to generate
	 * DescriptorList.  Ownership is taken by BaseCompoundFieldDescriptor
	 *
	 */
	CompoundFieldDescriptor(util::List<AbstractFieldDescriptor *> &newChildren);

	/** @fn ~CompoundFieldDescriptor()
	 * @brief Destructor - cleans up the BaseCompoundFieldDescriptor pointer
	 */
	~CompoundFieldDescriptor();

	/** @fn SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description)
	 * @brief Sets the BaseCompoundFieldDescriptor's name, description,
	 * mode, and class variables
	 *
	 * @param name - The BaseCompoundFieldDescriptor's name member is set to
	 * the name parameter
	 * @param mode - The BaseCompoundFieldDescriptor's mode member is set to
	 * the mode parameter
	 * @param clss - The BaseCompoundFieldDescriptor's class member is set to
	 * the clss parameter
	 * @param description - The BaseCompoundFieldDescriptor's description 
	 * member is set to the description parameter
	 *
	 * @return status - result of operation
	 */
	Status SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description);

private:
	// not implemented
	CompoundFieldDescriptor &operator=(const CompoundFieldDescriptor &);
	CompoundFieldDescriptor(const CompoundFieldDescriptor &);

	/** @fn AddTo(Message *message, Context &context) const
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

	internal::BaseCompoundFieldDescriptor* ptr;

	friend class internal::BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif

