
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file AbstractBaseFieldDescriptor.h
 *
 *  @brief This file contains the AbstractBaseFieldDescriptor class,
 *         which is the parent class of the BaseCompoundFieldDescriptor
 *         class and the BaseSimpleFieldDescriptor class which desribe
 *         Fields.
 *
**/

#ifndef gmsec_AbstractBaseFieldDescriptor_h
#define gmsec_AbstractBaseFieldDescriptor_h


#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/MessageTemplate.h>


#include <gmsec/Message.h>


namespace gmsec
{

namespace mist
{

namespace internal
{
class AbstractFieldDescriptor;
class BaseTester;

/** @class AbstractBaseFieldDescriptor
 * @brief The AbstractBaseFieldDescriptor is the virtual base class for the 
 * different BaseFieldDescriptor classes.  It contains getters and setters for
 * for the FieldDescriptor members and virtual functions for the functions 
 * that are used in the instantiate, valiate, and apply functionality
 *
 * @sa gmsec::mist::AbstractFieldDescriptor
 * @sa gmsec::mist::Context
*/
class GMSEC_API AbstractBaseFieldDescriptor
{
public:
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

	/** @fn FindField(Message *message, Context &context, MIST_VALIDATE_VALS validate) const
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

	/** @fn AbstractBaseFieldDescriptor()
	 * @brief Default constructor - initializes the AbstractBaseFieldDescriptor 
	 * instance with fname set to "unset", fdescription set to "unset", fmode set
	 * to F_OPTIONAL, and fclass set to F_NONE
	 *
	 */
	AbstractBaseFieldDescriptor();

	/** @fn ~AbstractBaseFieldDescriptor()
	 *  @brief Pure virtual destructor for AbstractBaseFieldDescriptor; implemented
     *   by BaseSimpleFieldDescriptor and BaseCompoundFieldDescriptor derived classes.
     */ 
	virtual ~AbstractBaseFieldDescriptor() = 0;

	/** @fn GetName(const char * &name) const
	 * @brief Sets the parameter to the AbstractBaseFieldDescriptor's name
	 *
	 * @param name - The const char * reference is set to a copy of the name
	 * member
	 *
	 * @return status - result of operation
	 */
	Status GetName(const char * &name) const;

	/** @fn GetMode() const
	 * @brief Gets the AbstractBaseFieldDescriptor's mode
	 *
	 * @return FieldMode - mode of the AbstractFieldDescriptor
	 */
	FieldMode GetMode() const;

	/** @fn GetClass() const
	 * @brief Gets the AbstractBaseFieldDescriptor's class
	 *
	 * @return FieldClass - class of the AbstractFieldDescriptor
	 */
	FieldClass GetClass() const;

	/** @fn GetDescription(const char * &description) const
	 * @brief Sets the parameter to the AbstractBaseFieldDescriptor's description
	 *
	 * @param description - The const char * reference is set to a copy of the 
	 * description member
	 *
	 * @return status - result of operation
	 */
	Status GetDescription(const char * &description) const;

	/** @fn SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description)
	 * @brief Sets the AbstractBaseFieldDescriptor's name, description,
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
	Status SetFieldVariables(const char * name, FieldMode mode,
		FieldClass clss, const char * description);

	/** @fn SetName(const char * name)
	 * @brief Sets the AbstractBaseFieldDescriptor's name variable
	 *
	 * @param name - The AbstractBaseFieldDescriptor's name member is set to
	 * the name parameter
	 *
	 * @return status - result of operation
	 */
	Status SetName(const char * name);

	/** @fn SetMode(FieldMode mode)
	 * @brief Sets the AbstractBaseFieldDescriptor's mode variable
	 *
	 * @param mode - The AbstractBaseFieldDescriptor's mode member is set to
	 * the mode parameter
	 *
	 * @return status - result of operation
	 */
	Status SetMode(FieldMode mode);

	/** @fn SetClass(FieldClass class)
	 * @brief Sets the AbstractBaseFieldDescriptor's class variable
	 *
	 * @param class - The AbstractBaseFieldDescriptor's class member is set to
	 * the class parameter
	 *
	 * @return status - result of operation
	 */
	Status SetClass(FieldClass clss);

	/** @fn SetDescription(const char * description)
	 * @brief Sets the AbstractBaseFieldDescriptor's description variable
	 *
	 * @param description - The AbstractBaseFieldDescriptor's description 
	 * member is set to the name parameter
	 *
	 * @return status - result of operation
	 */
	Status SetDescription(const char * description);

protected:
	AbstractBaseFieldDescriptor &operator=(const AbstractBaseFieldDescriptor &);
	AbstractBaseFieldDescriptor(const AbstractBaseFieldDescriptor &);

	std::string fname;
	FieldMode fmode;
	FieldClass fclass;
	std::string fdescription;

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
