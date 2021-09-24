
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Result.h>

#include <sstream>

namespace bolt {


Result::Result()
{
}


Result::Result(const Result &other)
	:
	shared(other.shared)
{
}


Result::Result(int code, const string &text)
{
	set(code, text);
}


bool Result::isValid() const
{
	if (!shared.get())
		return true;
	return shared->code == 0;
}


bool Result::isError() const
{
	if (!shared.get())
		return false;
	return shared->code != 0;
}


int Result::getCode() const
{
	if (!shared.get())
		return 0;
	return shared->code;
}


string Result::getText() const
{
	if (!shared.get()) {
		static string ok("ok");
		return ok;
	}
	return shared->text;
}


void Result::set(int i, const string &s)
{
	if (i || s.size())
	{
		if (!shared.get())
			shared.reset(new Imp(i, s));
	}
	else
		shared.reset();
}


void Result::clear()
{
	shared.reset();
}


Result &Result::operator=(const Result &other)
{
	if (this == &other)
		return *this;

	shared = other.shared;
	return *this;
}


std::ostream &Result::put(std::ostream &os) const
{
	os << "Result[" << getCode() << ',' << getText() << ']';
	return os;
}

gmsec::api::util::LogStream &Result::put(gmsec::api::util::LogStream &os) const
{
	os << "Result[" << getCode() << ',' << getText().c_str() << ']';
	return os;
}

} // namespace bolt

