/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Subject.cpp
 *
 *  @brief Utility functions for validating subjects.
 *
**/

#include <gmsec4/internal/Subject.h>

namespace gmsec {
namespace api {
namespace util {


using std::string;
using std::vector;


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



bool Subject::isValid (const string &subject, bool lenient)
{
	vector<string> elements;
	if (!getElements(subject, elements))
		return false;
	return areValidElements(elements, false, lenient);
}


bool Subject::isValidSubscription (const string &subject, bool lenient)
{
	vector<string> elements;
	if (!getElements(subject, elements))
		return false;
	return areValidElements(elements, true, lenient);
}


bool Subject::getElements (const string &subject, vector<string> &elements)
{
     size_t i = 0;

     while (i != string::npos)
     {
             size_t p = subject.find('.', i);
             if (p == string::npos)
             {
                     string element = subject.substr(i);
                     elements.push_back(element);
                     i = p;
             }
             else
             {
                     string element = subject.substr(i, p - i);
                     elements.push_back(element);
                     i = p + 1;
             }
     }

     return elements.size() > 0;
}


bool Subject::areValidElements (const vector<string> &elements, bool subscription, bool lenient)
{
	for (size_t i = 0; i < elements.size(); ++i)
		if (!isValidElement(elements[i], subscription, i == elements.size() - 1, lenient))
			return false;
	return true;
}


bool Subject::isValidElement (const string &element, bool subscription, bool last, bool lenient)
{
	bool valid = false;

	CharValidater &validater = lenient ? VALIDATE_LENIENT : VALIDATE_STRICT;

	if (element.length() < 1)
		valid = false;
	else if (subscription && last && (element == ">" || element == "+") )
		valid = true;
	else if (subscription && element == "*")
		valid = true;
	else
	{
		valid = true;
		for (string::const_iterator i = element.begin(); i != element.end(); ++i)
			if (!validater.isValid(*i))
				valid = false;
	}

	return valid;
}


bool Subject::doesSubjectMatchPattern(const string &subject, const string &pattern)
{
	vector<string> subelem;
	if (!getElements(subject, subelem))
		return false;

	vector<string> pattelem;
	if (!getElements(pattern, pattelem))
		return false;

	return doesSubjectMatchPattern(subelem, pattelem);
}


bool Subject::doesSubjectMatchPattern(const vector<string> &subelem, const string &pattern)
{
	vector<string> pattelem;
	if (!getElements(pattern, pattelem))
		return false;

	return doesSubjectMatchPattern(subelem, pattelem);
}


bool Subject::doesSubjectMatchPattern(const string &subject, const vector<string> &pattelem)
{
	vector<string> subelem;
	if (!getElements(subject, subelem))
		return false;

	return doesSubjectMatchPattern(subelem, pattelem);
}


bool Subject::doesSubjectMatchPattern(const vector<string> &subelem, const vector<string> &pattelem)
{
	vector<string>::const_iterator isub = subelem.begin();
	vector<string>::const_iterator psub = pattelem.begin();

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



} // namespace util
} // namespace api
} // namespace gmsec


#ifdef GMSEC_TEST_SUBJECT

#include <iostream>

using std::cout;
using std::string;


static void testSubject (string s, bool expected)
{
	bool valid = gmsec::util::Subject::isValid(s);

	cout << "test(" << s << ") => " << valid;
	if (valid != expected)
		cout << ": WRONG";
	cout << "\n";
}


static void testSubscription (string s, bool expected)
{
	bool valid = gmsec::util::Subject::isValidSubscription(s);

	cout << "test(" << s << ") => " << valid;
	if (valid != expected)
		cout << ": WRONG";
	cout << "\n";
}

static void testMatch (string topic, string pattern, bool expected)
{
	bool match = gmsec::util::Subject::doesSubjectMatchPattern(topic, pattern);
	cout << "match(" << topic << ", " << pattern << ") => " << match;

	if (match != expected)
		cout << ": WRONG";
	cout << "\n";
}


static void test ()
{
cout << "\ntesting subjects\n";
	testSubject("X", true);
	testSubject("A.B", true);
	testSubject("AB.", false);
	testSubject("A>", false);
	testSubject(">A", false);
	testSubject(">.A", false);
	testSubject("..", false);
	testSubject("*.A", false);
	testSubject("A.*.B", false);
	testSubject("A.**.B", false);
	testSubject("A.>.B", false);
	testSubject("a.>", false);
	testSubject("HELLo", false);

// from Interface Spec
	testSubject("THIS.IS.A.VALID.SUBJECT", true);
	testSubject("THIS.IS.NOT.A.VALID.SUBJECT.", false); // missing E at end
	testSubject(".THIS.IS.NOT.A.VALID.SUBJECT", false); // missing E at start
	testSubject("THIS.IS..NOT.A.VALID.SUBJECT", false); // empty element in middle
	testSubject("GMSEC.FILL.VALID.SUBJECT", true); // valid GMSEC subject
	testSubject("GMSEC.VALID.SUBJECT", true); // valid GMSEC subject
	testSubject("Gmsec.not.valid.subject", false); // lower case

cout << "testing subscriptions\n";
	testSubscription("A.B", true);
	testSubscription("AB.", false);
	testSubscription("A>", false);
	testSubscription(">A", false);
	testSubscription(">.A", false);
	testSubscription("..", false);
	testSubscription("*.A", true);
	testSubscription("A.*.B", true);
	testSubscription("A.**.B", false);
	testSubscription("A.>.B", false);
	testSubscription("a.>", false);
	testSubscription("HELLo", false);

// from Interface Spec
	testSubscription("THIS.*.VALID", true);
	testSubscription("THIS.IS*.NOT.VALID", false);

	testSubscription("THIS.IS.VALID.>", true);
	testSubscription("THIS.IS.>.NOT.VALID", false);

cout << "\ntesting matching\n";
	testMatch("X.Y.Z", "X.Y.Z", true);
	testMatch("X.Y.Z", "X.Y.Q", false);
	testMatch("X.Y", "X.Y.Z", false);
	testMatch("W.X.Y.Z", "W.X", false);
	testMatch("A.B", ">", true);
	testMatch("A.B", "*", false);
	testMatch("A.B.C", "A.>", true);
}


int main (int argc, char ** argv)
{
	test();
	return 0;
}


#endif /* GMSEC_TEST_SUBJECT */
