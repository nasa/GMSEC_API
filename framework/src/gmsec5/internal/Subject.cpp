/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Subject.cpp
 *
 * @brief Utility functions for validating subjects.
 */

#include <gmsec5/internal/Subject.h>
#include <gmsec5/internal/InternalMessage.h>
#include <gmsec5/internal/StringUtil.h>

#include <gmsec5/util/Log.h>

using namespace gmsec::api5;
using namespace gmsec::api5::util;

class CharValidater
{
public:
	CharValidater(bool allowLower)
	{
		for (int i = 0; i < 10; ++i)
			valid['0' + i] = true;

		for (int i = 0; i < 26; ++i)
			valid['A' + i] = true;

		valid[index('-')] = true;
		valid[index('_')] = true;

		if (allowLower)
			for (int i = 0; i < 26; ++i)
				valid['a' + i] = true;
	}

	// this quiets some warnings about using chars as array indices
	static int index(char c) {
		return (unsigned char) c;
	}

	bool isValid (char c) {
		return valid[index(c)];
	}

private:
	bool valid[256];
};


static CharValidater VALIDATE_STRICT(false);
static CharValidater VALIDATE_LENIENT(true);


std::string Subject::isValid(const std::string& subject, bool lenient)
{
	if (subject.find("..") != std::string::npos)
		return " -- Subject has '..' (is it missing an element?)";
	std::vector<std::string> elements = StringUtil::split(subject, '.');
	if (elements.size() <= 0)
		return " -- Subject cannot be empty";
	return areValidElements(elements, NULL, false, lenient);
}


std::string Subject::isValid(const std::string& subject, const internal::InternalMessage* msg, bool lenient)
{
	if (subject.find("..") != std::string::npos)
		return " -- Subject has '..' (is it missing an element?)";
	std::vector<std::string> elements = StringUtil::split(subject, '.');
	if (elements.size() <= 0)
		return " -- Subject cannot be empty";
	return areValidElements(elements, msg, false, lenient);
}


std::string Subject::isValidSubscription(const std::string& subject, bool lenient)
{
	if (subject.find("..") != std::string::npos)
		return " -- Subject has '..' (is it missing an element?)";
	if (subject.empty())
		return " -- Subject cannot be empty";
	std::vector<std::string> elements;
	if (!getElements(subject, elements))
		return " -- Subject cannot be empty";
	return areValidElements(elements, NULL, true, lenient);
}


bool Subject::getElements(const std::string& subject, std::vector<std::string>& elements)
{
	if (subject.empty())
		return false;

	size_t i = 0;

	while (i != std::string::npos)
	{
		size_t p = subject.find('.', i);
		if (p == std::string::npos)
		{
			std::string element = subject.substr(i);
			elements.push_back(element);
			i = p;
		}
		else
		{
			std::string element = subject.substr(i, p - i);
			elements.push_back(element);
			i = p + 1;
		}
	}

	return elements.size() > 0;
}


std::string Subject::areValidElements(const std::vector<std::string>& elements, const internal::InternalMessage* msg, bool subscription, bool lenient)
{
	//test elements for validating the contents of the subject
	SubjectElementList subjectElements;
	std::string err;
	if (msg && msg->getTemplate())
	{
		subjectElements = msg->getTemplate()->getSubjectElements();

		//if subject has more elements than its corresponding template, then extra undefined elements exist.
		if (elements.size() > subjectElements.size())
		{
			for (size_t i = subjectElements.size(); i < elements.size(); ++i)
			{
				err.append("\n\tUser-defined subject element value \"").append(elements[i].c_str()).append("\" is not allowed");
			}
		}
	}

	for (size_t i = 0; i < subjectElements.size(); ++i)
	{
		std::string element = (i < elements.size() ? elements[i] : "!!!"); //!!! value indicates further defined elements, but subject has ended

		std::string fieldName = subjectElements[i].second;
		std::string fieldValue;

		//determine if subject element is required/optional and remove character flag
		bool required = true;
		if (!fieldName.empty() && fieldName.find("!") != std::string::npos)
		{
			required = false;
			fieldName = fieldName.substr(1);
		}

		if (msg)
		{
			//element can have multiple associated fields, split into sub elements and look for values
			std::vector<std::string> subElements = StringUtil::split(fieldName, ",");

			for (size_t j = 0; j < subElements.size(); ++j)
			{
				if (msg->hasField(subElements[j].c_str()))
				{
					fieldValue = msg->getStringValue(subElements[j].c_str());
					break;
				}
			}
			
			if (fieldValue.empty() && required && !fieldName.empty())
			{
				err.append("\n\tSubject element ").append(subjectElements[i].first).append(" missing; requires field(s): ").append(fieldName.c_str());
			}
		}

		std::string result = isValidElement(element, fieldValue, required, subscription, i == (elements.size() - 1), lenient);
		if (!result.empty())
		{
			err.append("\n\tSubject element \"").append(subjectElements[i].first).append("\" is invalid: ").append(result);
		}
	}

	if (!msg)
	{
		for (size_t i = 0; i < elements.size(); ++i)
		{
			std::string result = isValidElement(elements[i], "", false, subscription, i == (elements.size() - 1), lenient);
			if (!result.empty())
			{
				err.append("\n\tSubject element \"").append(elements[i]).append("\" is invalid: ").append(result);
			}
		}
	}

	return err;
}


std::string Subject::isValidElement(const std::string& element, const std::string& fieldValue, bool required, bool subscription, bool last, bool lenient)
{
	std::string err;

	CharValidater& validater = lenient ? VALIDATE_LENIENT : VALIDATE_STRICT;

	if (element.length() < 1)
		return "\n\t\telement cannot be empty";
	else if (StringUtil::stringEquals(element.c_str(), "!!!"))
	{
		if (required && !subscription)
			return "\n\t\telement is required, but missing";
	}
	else if (subscription && last && (element == ">" || element == "+")) /*valid, do nothing*/;
	else if (subscription && element == "*") /*valid, do nothing*/;
	else
	{
		for (std::string::const_iterator i = element.begin(); i != element.end(); ++i)
		{
			if (!validater.isValid(*i))
			{
				err.append("\n\t\telement contains illegal character");
				break;
			}
		}

		if (!fieldValue.empty())
		{
			//compare value of element to value in referenced field
			//invalid if values don't match
			if (!StringUtil::stringEquals(element.c_str(), fieldValue.c_str()))
				err.append("\n\t\telement contains unexpected value \"").append(element).append("\". Expected value: ").append(fieldValue);

		}
	}

	return err;
}


bool Subject::doesSubjectMatchPattern(const std::string& subject, const std::string& pattern)
{
	std::vector<std::string> subelem;

	if (!getElements(subject, subelem))
		return false;

	std::vector<std::string> pattelem;
	if (!getElements(pattern, pattelem))
		return false;

	return doesSubjectMatchPattern(subelem, pattelem);
}


bool Subject::doesSubjectMatchPattern(const std::vector<std::string>& subelem, const std::string& pattern)
{
	std::vector<std::string> pattelem;

	if (!getElements(pattern, pattelem))
		return false;

	return doesSubjectMatchPattern(subelem, pattelem);
}


bool Subject::doesSubjectMatchPattern(const std::string& subject, const std::vector<std::string>& pattelem)
{
	std::vector<std::string> subelem;

	if (!getElements(subject, subelem))
		return false;

	return doesSubjectMatchPattern(subelem, pattelem);
}


bool Subject::doesSubjectMatchPattern(const std::vector<std::string>& subelem, const std::vector<std::string>& pattelem)
{
	std::vector<std::string>::const_iterator isub = subelem.begin();
	std::vector<std::string>::const_iterator psub = pattelem.begin();

	while (isub != subelem.end() && psub != pattelem.end())
	{
		if (*psub == ">" || *psub == "+")
		{
			// any remaining elements of *isub are ok
			return true;
		}
		else if (*psub == "*")
		{
			// any value of *isub ok for this element
		}
		else if (*psub != *isub)
		{
			// *psub and *isub mis-match in this element
			return false;
		}
		else
		{
			// ok, *psub and *isub matched here
		}

		++isub;
		++psub;

		if (isub == subelem.end() && psub != pattelem.end() )
		{
			if (*psub == "+")
			{
				// all *psub elements before + match with *isub elements 
				return true;
			}
		}
	}

	if (psub == pattelem.end() && isub == subelem.end())
	{
		// no mis-matches and reached end
		return true;
	}

	return false;
}
