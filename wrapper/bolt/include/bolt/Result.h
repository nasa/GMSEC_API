
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Result_h
#define bolt_Result_h


#include <bolt/Types.h>

#include <gmsec4/util/LogStream.h>

#include <ostream>


namespace bolt {

class Result
{
public:

	Result();
	Result(const Result &result);
	Result(int code, const std::string &text);

	bool isValid() const;
	bool isError() const;

	int getCode() const;
	std::string getText() const;

	void set(int code, const std::string &text);

	void clear();

	gmsec::api::util::LogStream &put(gmsec::api::util::LogStream &os) const;
	std::ostream &put(std::ostream &os) const;

	Result &operator=(const Result &result);


private:

	struct Imp
	{
		Imp(int i, std::string s) : code(i), text(s) { }
		int code;
		std::string text;
	};

	counted_ptr<Imp> shared;
};


namespace util {
void set(bolt::Result &result, Code code);
}


} // namespace bolt

inline std::ostream &operator<<(std::ostream &os, const bolt::Result &result)
{
	return result.put(os);
}

inline gmsec::api::util::LogStream &operator<<(gmsec::api::util::LogStream &os, const bolt::Result &result)
{
	return result.put(os);
}

#endif /* bolt_Result_h */

