
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Context.h
 *
 *  @brief This file contains a class to store the results of a Message validation
 *
**/

#ifndef gmsec_Context_h
#define gmsec_Context_h

#include <gmsec/Status.h>
#include <stddef.h>

namespace gmsec
{

namespace mist
{

namespace internal
{
template<typename T> class BaseSimpleFieldDescriptor;
class BaseContext;
class BaseCompoundFieldDescriptor;
class BaseMessageTemplate;
class BaseTester;


/** @class Context
 * @brief The Context object is a wrapper for the BaseContext class.  The
 * methods of this class allow for manipulation of Validate, Instatiate,
 * and Apply settings and review of the results of those functions
 *
 * @sa gmsec::mist::internal::BaseContext
*/
class GMSEC_API Context
{
public:

	/** @fn Context()
	 * @brief Default constructor - initializes the BaseContext instance
	 *
	 */
	Context();	

	/** @fn Context(const char * templateId, bool validateOnApply)
	 * @brief Constructor - initializes the BaseContext instance
	 *
	 * @param templateId - The const char * that matches a template id
	 * saved in Specification's MessageTemplate list
	 * @param validateOnApply - bool that sets the whether validation
	 * should be performed on Apply
	 */
	Context(const char * templateId, bool validate);

	/** @fn ~Context()
	 * @brief Destructor - cleans up the pointer to BaseContext
	 */
	~Context();

	/** @fn ValidateBoolean(bool validateOnApply)
	 * @brief Updates BaseContext's bool that dictates if validation will occur when 
	 * calling Apply on a Message
	 *
	 * @param validateOnApply - the bool value to replace the old validateOnApply
	 *
	 * @return status - result of operation
	 */
	Status ValidateBoolean(bool validateOnApply);

	/** @fn GetTemplateId(const char * &templateId) const
	 * @brief Creates a new const char * with the current templateId
	 * and returns it in the templateId parameter
	 *
	 * @param templateId - The reference to a const char * is set to a 
	 * copy of the BaseContext's current templateId
	 *
	 * @return status - result of operation, error if TemplateID had not
	 * been set
	 */
	Status GetTemplateId(const char * &id) const;

	/** @fn Reset(const char * templateId)
	 * @brief Replaces the BaseContext's current templateId with the parameter
	 * templateId.  All the fields in BaseContext are also cleared including
	 * visited, statuses, namePath.  The bool validateOnApply remains the 
	 * same.
	 *
	 * @param templateId - The const char * is used to set the new
	 * value for BaseContext's templateId
	 *
	 * @return status - result of operation, error if templateId is null
	 */
	Status Reset(const char * templateId);

	/** @fn Visited(const char * name) const
	 * @brief Determines if the passed const char * is contained in BaseContext's 
	 * visited list
	 *
	 * @param name - name is checked against the elements of BaseContext's visited 
	 * to see if it exists in the list
	 *
	 * @return status - result of operation
	 */
	bool Visited(const char * name) const;

	/** @fn GetValidate() const
	 * @brief Returns the value of the validateOnApply bool
	 *
	 * @return bool - if validateOnApply is true or false
	 */
	bool GetValidate() const;

	/** @fn GetStatusCount()
	 * @brief Returns the number of statuses stored in the BaseContext
	 *
	 * @return size_t - total number of statuses
	 */
	size_t GetStatusCount();

	/** @fn GetStatusIndex(size_t index, const char * &name, const char * &desc, int &code)
	 * @brief Looks up the status at index i and copies the name, description,
	 * and error code into the supplied parameter references
	 *
	 * @param index - index of status to look up
	 * @param name - std::string that is set to a copy of the status at index
	 * i's name
	 * @param desc - std::string that is set to a copy of the status at index
	 * i's description
	 * @param code - int that is set to the status at index i's code
	 *
	 * @return status - result of operation, error if i is out of bounds
	 */
	Status GetStatusIndex(size_t index, const char * &name, const char * &desc, int &code);

private:
	// not implemented
	Context &operator=(const Context &);
	Context(const Context &);

	Status ResetContext(internal::BaseContext* bc);

	internal::BaseContext* ptr;
	friend class internal::BaseCompoundFieldDescriptor;
	template<typename T> friend class internal::BaseSimpleFieldDescriptor;
	friend class internal::BaseMessageTemplate;
	friend class MessageTemplate;

	friend class internal::BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
