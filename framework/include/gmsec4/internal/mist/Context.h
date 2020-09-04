/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Context.h
 *
 *  @brief This file contains a class to store the results of a Message validation
 *
**/

#ifndef GMSEC_MIST_CONTEXT_H
#define GMSEC_MIST_CONTEXT_H

#include <gmsec4/Status.h>

#include <string>
#include <stddef.h>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{
	// Forward declaration(s)
	class InternalCompoundFieldDescriptor;
	class InternalContext;
	class InternalMessageTemplate;
	class MessageTemplate;
	template<typename T> class InternalSimpleFieldDescriptor;
	class VariableFieldDescriptor;
	class InternalTester;


/**
 * @class ContextStatus
 * @brief Detailed status information should error occur when processing a Field.
 */
struct ContextStatus
{
	/**
	 * @desc Name of the Field in which the error occurred.
	 */
	std::string fieldName;

	/**
	 * @desc Brief description of the error.
	 */
	std::string description;

	/**
	 * @desc Level at which the error occurred.
	 */
	int errorLevel;
};


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


	/** @fn Context(const char* templateId, bool validateOnApply)
	 * @brief Constructor - initializes the BaseContext instance
	 *
	 * @param templateId - The const char * that matches a template id
	 * saved in Specification's MessageTemplate list
	 * @param validateOnApply - bool that sets the whether validation
	 * should be performed on Apply
	 */
	Context(const char* templateId, bool validate);


	/** @fn ~Context()
	 * @brief Destructor - cleans up the pointer to BaseContext
	 */
	~Context();


	/** @fn const char* getTemplateId() const
	 * @brief Creates a new const char * with the current templateId
	 * and returns it in the templateId parameter
	 *
	 * @return The template ID string currently assigned to this context.
	 */
	const char* getTemplateId() const;


	/** @fn reset(const char* templateId)
	 * @brief Replaces the BaseContext's current templateId with the parameter
	 * templateId.  All the fields in BaseContext are also cleared including
	 * visited, statuses, namePath.  The bool validateOnApply remains the 
	 * same.
	 *
	 * @param templateId - The const char * is used to set the new
	 * value for BaseContext's templateId
	 */
	void reset(const char* templateId);


	/** @fn validateOnApply(bool validate)
	 * @brief Dictates if validation will occur when calling apply on a Message
	 *
	 * @param validate - the bool value to replace the old validateOnApply
	 */
	void validateOnApply(bool validate);


	/** @fn getValidate() const
	 * @brief Returns the value of the validateOnApply bool
	 *
	 * @return bool - if validateOnApply is true or false
	 */
	bool getValidate() const;


	/** @fn visited(const char* name) const
	 * @brief Determines if the passed const char * is contained in BaseContext's 
	 * visited list
	 *
	 * @param name - name is checked against the elements of BaseContext's visited 
	 * to see if it exists in the list
	 *
	 * @return status - result of operation
	 */
	bool visited(const char* name) const;


	/** @fn getStatusCount()
	 * @brief Returns the number of statuses stored in the BaseContext
	 *
	 * @return size_t - total number of statuses
	 */
	size_t getStatusCount();


	/** @fn ContextStatus getStatus(size_t index)
	 * @brief Returns a copy of the ContextStatus stored at the given index, if available.
	 *
	 * @param index - index of status to look up
	 *
	 * @return Object containing ContextStatus.
	 *
	 * @throw An Exception is thrown if the index is out of range.
	 */
	ContextStatus getStatus(size_t index);


private:
	// Defined, but not implemented
	Context(const Context&);
	Context& operator=(const Context&);


	void resetContext(internal::InternalContext* bc);


	internal::InternalContext* ptr;

	friend class InternalCompoundFieldDescriptor;
	friend class InternalMessageTemplate;
	template<typename T> friend class InternalSimpleFieldDescriptor;
	friend class VariableFieldDescriptor;
	friend class MessageTemplate;
	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
