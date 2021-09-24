
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseContext.h
 *
 *  @brief This file contains a class to store the results of a Message validation
 *
**/

#ifndef gmsec_BaseContext_h
#define gmsec_BaseContext_h


#include <gmsec/internal/strutil.h>

#include <list>

namespace gmsec
{

namespace mist
{

namespace internal
{
struct ContextStatus
{
	std::string fieldName;
	std::string description;
	int errorLevel;

};

class BaseTester;
class BaseCompoundFieldDescriptor;
class BaseMessageTemplate;

/** @class BaseContext
 * @brief The BaseContext object is an object that stores results of Specification
 * functions.  This stores results from either the Instantiate, Validate, or Apply
 * functions and allows the review of errors that were generated at each step 
 * within the function.
 *
 * @sa gmsec::mist::Context
*/

class GMSEC_API BaseContext
{
public:
	/** @fn BaseContext()
	 * @brief Default constructor - initializes the BaseContext instance
	 *
	 */
	BaseContext();

	/** @fn BaseContext(const char * templateId, bool validateOnApply)
	 * @brief Constructor - initializes the BaseContext instance
	 *
	 * @param templateId - The const char * that matches a template id
	 * saved in Specification's MessageTemplate list
	 * @param validateOnApply - bool that sets the whether validation
	 * should be performed on Apply
	 */
	BaseContext(const char * templateId, bool validateOnApply);

	/** @fn ~BaseContext()
	 * @brief Destructor - cleans up the message instance
	 */
	~BaseContext();

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
	Status GetTemplateId(const char * &templateId) const;

	/** @fn Reset(const char * templateId)
	 * @brief Replaces the BaseContext's current templateId to the parameter
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

	/** @fn FullNamePath(std::string &fullPathName) const
	 * @brief Generates a std::string from the namePath list, concatenates
	 * then into the dot deliminated field name format
	 *
	 * @param fullPathName - the reference to the std::string object is
	 * set to the string generated from the name list
	 *
	 * @return status - result of operation
	 */
	Status FullNamePath(std::string &fullPathName) const;

	/** @fn AddVisited()
	 * @brief Adds the field currently represented in the namePath list
	 * to the visited list.
	 *
	 * @return status - result of operation
	 */
	Status AddVisited();

	/** @fn Visited(const char * name) const
	 * @brief Determines if the passed const char * is contained in the 
	 * visited list
	 *
	 * @param name - name is checked against the elements of visited to
	 * see if it exists in the list
	 *
	 * @return status - result of operation
	 */
	bool Visited(const char * name) const;

	/** @fn IncrementDepth(std::string fieldName)
	 * @brief Adds the fieldName parameter to the namePath list to keep track
	 * of the current depth in the FieldDescriptor list
	 *
	 * @param fieldName - the current FieldDescriptor name
	 *
	 * @return status - result of operation
	 */
	Status IncrementDepth(std::string fieldName);

	/** @fn DecrementDepth()
	 * @brief Removes the last entry from the namePath list to keep track
	 * of the current depth in the FieldDescriptor list
	 *
	 * @return status - result of operation
	 */
	Status DecrementDepth();

	/** @fn AddStatus(const std::string &namePathString, int errorLevel, const std::string &description)
	 * @brief Adds a ContextStatus with the fields set set to the passed in
	 * parameters
	 *
	 * @param namePathString - std::string name of the field in which the error
	 * occurred
	 * @param errorLevel - int errorLevel representing the errorCode
	 * @param description - std::string brief sentence describing the error
	 *
	 * @return status - result of operation
	 */
	Status AddStatus(const std::string &namePathString, int errorLevel, const std::string &description);

	/** @fn ValidateBoolean(bool validateOnApply)
	 * @brief Updates the bool that dictates if validation will occur when 
	 * calling Apply on a Message
	 *
	 * @param validateOnApply - the bool value to replace the old validateOnApply
	 *
	 * @return status - result of operation
	 */
	Status ValidateBoolean(bool validateOnApply);

	/** @fn GetValidate() const
	 * @brief Returns the value of the validateOnApply bool
	 *
	 * @return bool - if validateOnApply is true or false
	 */
	bool GetValidate() const;

	/** @fn GetStatusCount()
	 * @brief Returns the number of statuses stored in the BaseContext
	 *
	 * @return int - total number of statuses
	 */
	size_t GetStatusCount() const;

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
	BaseContext &operator=(const BaseContext &);
	BaseContext(const BaseContext &);

	std::string templateId;
	std::vector<ContextStatus> statuses;
	std::list<std::string> visited;
	
	std::list<std::string> namePath;
	bool validateOnApply;

	friend class BaseTester;
};

} //end internal

} //end mist

} //end gmsec

#endif
