/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalContext.h
 *
 *  @brief This file contains a class to store the results of a Message validation
 *
**/

#ifndef GMSEC_MIST_INTERNAL_CONTEXT_H
#define GMSEC_MIST_INTERNAL_CONTEXT_H


#include <gmsec4/internal/mist/Context.h>

#include <list>
#include <string>
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
	class InternalTester;


/** @class InternalContext
 * @brief The InternalContext object is an object that stores results of Specification
 * functions.  This stores results from either the Instantiate, Validate, or Apply
 * functions and allows the review of errors that were generated at each step 
 * within the function.
 *
 * @sa gmsec::mist::Context
*/
class GMSEC_API InternalContext
{
public:
	/**
	 * @fn InternalContext()
	 * @brief Default constructor - initializes the InternalContext instance
	 */
	InternalContext();


	/**
	 * @fn InternalContext(const char * templateId, bool validateOnApply)
	 * @brief Constructor - initializes the InternalContext instance
	 *
	 * @param templateId - The const char * that matches a template id
	 * saved in Specification's MessageTemplate list
	 * @param validateOnApply - bool that sets the whether validation
	 * should be performed on Apply
	 */
	InternalContext(const char * templateId, bool validateOnApply);


	/**
	 * @fn ~InternalContext()
	 * @brief Destructor - cleans up the message instance
	 */
	~InternalContext();


	/**
	 * @fn const char* getTemplateId() const
	 * @brief Creates a new const char * with the current templateId
	 * and returns it in the templateId parameter
	 *
	 * @return The template ID string currently assigned to this context.
	 */
	const char* getTemplateId() const;


	/**
	 * @fn reset(const char* templateId)
	 * @brief Replaces the InternalContext's current templateId to the parameter
	 * templateId.  All the fields in InternalContext are also cleared including
	 * visited, statuses, namePath.  The bool validateOnApply remains the 
	 * same.
	 *
	 * @param templateId - The const char * is used to set the new
	 * value for InternalContext's templateId
	 */
	void reset(const char* templateId);


	/**
	 * @fn validateOnApply(bool validate)
	 * @brief Updates the bool that dictates if validation will occur when 
	 * calling Apply on a Message
	 *
	 * @param validate - the bool value to indicate whether to validate when Fields are
	 * applied to a Message.
	 */
	void validateOnApply(bool validate);


	/**
	 * @fn getValidate() const
	 * @brief Returns the value of the validateOnApply bool
	 *
	 * @return bool - if validateOnApply is true or false
	 */
	bool getValidate() const;


	/**
	 * @fn std::string fullNamePath() const
	 * @brief Generates a std::string from the namePath list, concatenates
	 * then into the dot deliminated field name format
	 *
	 * @return A string containing the full name path.
	 */
	std::string fullNamePath() const;


	/**
	 * @fn addVisited()
	 * @desc Adds the field currently represented in the namePath list
	 * to the visited list.
	 */
	void addVisited();


	/**
	 * @fn visited(const char* name) const
	 * @brief Determines if the passed const char * is contained in the 
	 * visited list
	 *
	 * @param name - name is checked against the elements of visited to
	 * see if it exists in the list
	 *
	 * @return status - result of operation
	 */
	bool visited(const char* name) const;


	/**
	 * @fn incrementDepth(const std::string& fieldName)
	 * @brief Adds the fieldName parameter to the namePath list to keep track
	 * of the current depth in the FieldDescriptor list
	 *
	 * @param fieldName - the current FieldDescriptor name
	 */
	void incrementDepth(const std::string& fieldName);


	/**
	 * @fn decrementDepth()
	 * @desc Removes the last entry from the namePath list to keep track
	 * of the current depth in the FieldDescriptor list
	 */
	void decrementDepth();


	/**
	 * @fn addStatus(const std::string& fieldName, int errorLevel, const std::string& description)
	 * @brief Adds a ContextStatus with the fields set set to the passed in
	 * parameters
	 *
	 * @param status - ContextStatus to add to the Context.
	 */
	void addStatus(const ContextStatus& status);


	/**
	 * @fn getStatusCount()
	 * @brief Returns the number of statuses stored in the InternalContext
	 *
	 * @return int - total number of statuses
	 */
	size_t getStatusCount() const;


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
	InternalContext(const InternalContext&);
	InternalContext &operator=(const InternalContext&);

	std::string                m_templateId;
	std::vector<ContextStatus> m_statuses;
	std::list<std::string>     m_visited;
	std::list<std::string>     m_namePath;
	bool                       m_validateOnApply;

	friend class InternalTester;
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
