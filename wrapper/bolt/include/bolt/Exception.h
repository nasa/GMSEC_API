
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Exception_h
#define bolt_Exception_h


namespace bolt {


class Exception : public std::exception
{
public:

	Exception(const char *s) : text(s) { }
	Exception(const string &s) : text(s) { }

	virtual ~Exception() throw() { }

	string getText() const {
		return text;
	}

private:

	string text;
};


class IOException : public Exception
{
public:

	IOException(const string &s) : Exception(s) { }

	virtual ~IOException() throw() { }
};


} // namespace bolt


#endif /* bolt_Exception_h */
