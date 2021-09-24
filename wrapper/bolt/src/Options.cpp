
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Options.h>
#include <bolt/Log.h>


namespace bolt {


using namespace std;



Options::Options()
{
}


Options::Options(const Options &opt)
{
	for (ConstIterator i = opt.properties.begin(); i != opt.properties.end(); ++i)
		add(i->second->clone());
}


Options::~Options()
{
	for (Iterator i = properties.begin(); i != properties.end(); ++i)
		delete i->second;

	properties.clear();
}


Options& Options::operator=(const Options& other)
{
	if (this != &other)
	{
		for (ConstIterator i = other.properties.begin(); i != other.properties.end(); ++i)
			add(i->second->clone());
	}
	return *this;
}


bool Options::add(Property *property)
{
	bool changed = false;

	Iterator i = properties.find(property->getName());

	if (i != properties.end())
	{
		changed = true;
		delete i->second;
	}

	properties[property->getName()] = property;

	return changed;
}


Property *Options::get(const string &name) const
{
	ConstIterator i = properties.find(name);

	Property *property = 0;

	if (i != properties.end())
		property = i->second;

	return property;
}


ostream &Options::put(ostream &os, const char *prefix) const
{
	os << prefix << "Options[" << this << "] count=" << properties.size() << '\n';
	for (ConstIterator i = properties.begin(); i != properties.end(); ++i)
	{
		os << prefix;
		i->second->put(os);
	}
	os << "--- end ---\n";
	return os;
}



string Options::getString(const string &key, const char *p) const
{
	ConstIterator i = properties.find(key);
	if (i != properties.end())
		return i->second->asString();
	if (p)
		return p;
	static string empty;
	return empty;
}


i32 Options::getI32(const string &key, i32 x) const
{
	ConstIterator i = properties.find(key);
	if (i != properties.end())
		return i->second->asInteger();
	return x;
}


bool Options::getFlag(const string &key, bool x) const
{
	ConstIterator i = properties.find(key);
	if (i != properties.end())
		return i->second->asFlag();
	return x;
}


} // namespace bolt

