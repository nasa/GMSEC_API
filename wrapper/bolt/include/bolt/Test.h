
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Test_h
#define bolt_Test_h


#include <bolt/Result.h>

#include <typeinfo>


#define LOG_INT(i) LOG((LogLevel) i)


namespace bolt {


class Shared;


class Test
{
public:
	Test();

	virtual ~Test() { }

	virtual Code apply() = 0;

	string info();

	void describe(const string &s);

	bool isPrelocked() const {
		return prelocked;
	}

	void setPrelocked(bool flag) {
		prelocked = flag;
	}

	void setVerbosity(int v) {
		verbosity = v;
	}

	int getVerbosity() const {
		return verbosity;
	}


private:

	string description;
	bool prelocked;
	int verbosity;
};


class TestConnected : public Test
{
public:
	TestConnected(Shared *s);

	virtual Code apply();

private:
	Shared *shared;
};



class TestAcknowledged : public Test
{
public:
	TestAcknowledged(Shared *s, const std::string &id);

	virtual Code apply();

private:
	Shared *shared;
	std::string id;
};



namespace util {

Result await(Test &test, Condition &cv, i32 timeout_ms);

} // namespace util


} // namespace bolt


#endif /* bolt_Test_h */

