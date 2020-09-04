/*
 * Copyright 2007-2016 United States Government as represented by the
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

#ifndef GMSEC_MIST_ABSTRACT_BASE_FIELD_DESCRIPTOR_H
#define GMSEC_MIST_ABSTRACT_BASE_FIELD_DESCRIPTOR_H


#include <gmsec4/internal/mist/Context.h>
#include <gmsec4/internal/mist/MessageTemplate.h>

#include <gmsec4/Status.h>

#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace mist
{
namespace internal
{
	// Forward declaration(s)
	class AbstractFieldDescriptor;
	class Context;
	class InternalTester;


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
	/** @fn addTo(Message& message, Context& context) const
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
	virtual Status addTo(Message& message, Context& context) const = 0;


	/** @fn validate(Message& message, Context& context) const
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
	virtual Status validate(Message& message, Context& context) const = 0;


	/** @fn apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
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
	virtual Status apply(Message& message, Context& context, MIST_VALIDATE_VALS validate) const = 0;


	/** @fn findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const
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
	virtual Status findField(Message& message, Context& context, MIST_VALIDATE_VALS validate) const = 0;


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


	/** @fn const char* getName() const
	 * @brief Sets the parameter to the AbstractBaseFieldDescriptor's name
	 *
	 * @return A string with the name of the Field Descriptor.
	 */
	const char* getName() const;


	/** @fn getMode() const
	 * @brief Gets the AbstractBaseFieldDescriptor's mode
	 *
	 * @return FieldMode - mode of the AbstractFieldDescriptor
	 */
	FieldMode getMode() const;


	/** @fn getClass() const
	 * @brief Gets the AbstractBaseFieldDescriptor's class
	 *
	 * @return FieldClass - class of the AbstractFieldDescriptor
	 */
	FieldClass getClass() const;


	/** @fn getDescription() const
	 * @brief Sets the parameter to the AbstractBaseFieldDescriptor's description
	 *
	 * @return A string containing the Field Descriptor description.
	 */
	const char* getDescription() const;


	/** @fn setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description)
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
	 * @throw An Exception is thrown if either name or description are NULL or contain empty strings.
	 */
	void setFieldVariables(const char* name, FieldMode mode, FieldClass clss, const char* description);


	/** @fn setName(const char* name)
	 * @brief Sets the AbstractBaseFieldDescriptor's name variable
	 *
	 * @param name - The AbstractBaseFieldDescriptor's name member is set to
	 * the name parameter
	 *
	 * @throw An Exception is thrown if name is NULL or contains empty string.
	 */
	void setName(const char* name);


	/** @fn setMode(FieldMode mode)
	 * @brief Sets the AbstractBaseFieldDescriptor's mode variable
	 *
	 * @param mode - The AbstractBaseFieldDescriptor's mode member is set to
	 * the mode parameter
	 */
	void setMode(FieldMode mode);


	/** @fn setClass(FieldClass class)
	 * @brief Sets the AbstractBaseFieldDescriptor's class variable
	 *
	 * @param class - The AbstractBaseFieldDescriptor's class member is set to
	 * the class parameter
	 */
	void setClass(FieldClass clss);


	/** @fn setDescription(const char* description)
	 * @brief Sets the AbstractBaseFieldDescriptor's description variable
	 *
	 * @param description - The AbstractBaseFieldDescriptor's description 
	 * member is set to the name parameter
	 *
	 * @throw An Exception is thrown if description is NULL or contains empty string.
	 */
	void setDescription(const char* description);


protected:
	AbstractBaseFieldDescriptor(const AbstractBaseFieldDescriptor&);
	AbstractBaseFieldDescriptor& operator=(const AbstractBaseFieldDescriptor&);

	std::string m_name;
	FieldMode   m_mode;
	FieldClass  m_class;
	std::string m_description;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
