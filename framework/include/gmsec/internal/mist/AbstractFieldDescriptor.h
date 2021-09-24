
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file AbstractFieldDescriptor.h
 *
 *  @brief This file contains the AbstractFieldDescriptor class which is the base class
 *         of the CompoundFieldDescriptor class and the SimpleFieldDescriptor class which
 *         desribe Fields.
 *
**/

#ifndef gmsec_AbstractFieldDescriptor_h
#define gmsec_AbstractFieldDescriptor_h


#include <gmsec/internal/mist/MessageTemplate.h>

#include <gmsec/Status.h>


namespace gmsec
{

namespace mist
{
namespace internal
{
class BaseTester;


/** @class AbstractFieldDescriptor
 * @brief The AbstractFieldDescriptor is the virtual base class for the 
 * different FieldDescriptor classes.  It is a wrapper to the AbstractBaseFieldDescriptor.
 *
 * @sa gmsec::mist::AbstractBaseFieldDescriptor
 * @sa gmsec::mist::Context
*/
class GMSEC_API AbstractFieldDescriptor
{
public:
	/** @fn ~AbstractFieldDescriptor()
	 * @brief Virtual deconstructor that defines the AbstractFieldDescriptor
	 * as a virtual class
	 *
	 */
	virtual ~AbstractFieldDescriptor() = 0;

	/** @fn SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description)
	 * @brief Virtual function that calls the dervied class's SetFieldVariables
	 * which sets the AbstractBaseFieldDescriptor's name, description,
	 * mode, and class variables
	 *
	 * @param name - The AbstractBaseFieldDescriptor's name member is set to
	 * the name parameter
	 * @param mode - The AbstractBaseFieldDescriptor's mode member is set to
	 * the mode parameter
	 * @param clss - The AbstractBaseFieldDescriptor's class member is set to
	 * the clss parameter
	 * @param description - The AbstractBaseFieldDescriptor's description 
	 * member is set to the description parameter
	 *
	 * @return status - result of operation
	 */
	virtual Status SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description) = 0;

protected:
	/** @fn AddTo(Message *message, Context &context) const
	 * @brief Virtual function that calls the derived class's AddTo function
	 * which generates the field(s) represented by the derived class
	 *
	 * @param message - The fields represented by the derived class are added
	 * to the reference Message
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	virtual Status AddTo(Message *message, Context &context) const = 0;

	/** @fn Validate(Message *message, Context &context) const
	 * @brief Virtual function that calls the derived class's Validate function
	 * which validates the Message based on the Field represented by the derived
	 * class
	 *
	 * @param message - The fields contained in the reference Message are
	 * validated against the fields represented by the derived class
	 * @param context - The reference to the Context object stores the statuses
	 * that are generated
	 *
	 * @return status - result of operation
	 */
	virtual Status Validate(Message *message, Context &context) const = 0;

	/** @fn Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
	 * @brief Virtual function that calls the derived class's Apply function
	 * which fills in the Message based on the Field represented by the derived
	 * class and validates the fields already in Message if the enum validate is
	 * set to VALIDATE_ON_APPLY
	 *
	 * @param message - The fields represented by the derived class are added
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
	virtual Status Apply(Message *message, Context &context, MIST_VALIDATE_VALS validate) const = 0;

	/** @fn FindField(Message *message, Context &context, MIST_VALIDATE_VALUES validate) const
	 * @brief Virtual function that calls the derived class's FindField function
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
	virtual Status FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const = 0;

	friend class internal::BaseMessageTemplate;
	friend class internal::BaseCompoundFieldDescriptor;

	friend class internal::BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
