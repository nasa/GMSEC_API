
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
 * @file Options.h
 */

#ifndef bolt_Options_h
#define bolt_Options_h


#include <bolt/Meta.h>


namespace bolt {

class Options
{
public:
	Options();
	Options(const Options &opt);
	~Options();

	Options& operator=(const Options& other);

	bool add(Property *p);
	Property *get(const string &key) const;

	string getString(const string &key, const char *p) const;
	i32 getI32(const string &key, i32 x) const;
	bool getFlag(const string &key, bool x) const;

	std::ostream &put(std::ostream &os, const char * prefix="") const;

private:

	typedef std::map<string, Property*>::iterator Iterator;
	typedef std::map<string, Property*>::const_iterator ConstIterator;

	string getString(const string &key) const;

	std::map<string, Property*> properties;
};


namespace opt {

const char SERVER[] = "SERVER";
const char PORT[] = "PORT";
const char GDEBUG[] = "DEBUG";
const char MULTI_SERVER[] = "MULTI_SERVER";
const char REPLY_TO[] = "REPLY_TO";
const char REPLY_EXPOSE[] = "REPLY_EXPOSE";
const char MANAGE_TIMEOUT_ms[] = "MANAGE_TIMEOUT_ms";
const char CONNECT_TIMEOUT_ms[] = "CONNECT_TIMEOUT_ms";
const char NAGLE[] = "NAGLE";
const char TCP_BUF_SIZE[] = "TCP_BUF_SIZE";

} // namespace opt


} // namespace bolt


#endif /* bolt_Options_h */

