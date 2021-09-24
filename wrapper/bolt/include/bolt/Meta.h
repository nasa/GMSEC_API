
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
 * @file Meta.h
 */

#ifndef bolt_Meta_h
#define bolt_Meta_h


#include <bolt/Result.h>
#include <bolt/Property.h>

#include <map>


namespace bolt {



class Meta
{
public:
	Meta();
	Meta(const Meta &meta);
	~Meta();

	Meta& operator=(const Meta& other);

	enum { MAX_ITEMS = 1000 };

	bool add(Property *p);
	Property *get(const string &key) const;


	void setTopic(const string &topic);
	string getTopic() const;

	void setID(const string &id);
	string getID() const;

	void setCorrID(const string &id);
	string getCorrID() const;

	void setReplyTo(const string &topic);
	string getReplyTo() const;


	bool parse(InputBuffer &buffer);
	Result put(OutputBuffer &buffer) const;
	std::ostream &put(std::ostream &os, const char * prefix="") const;

	i32 determineSize() const;

	typedef std::map<string, Property*>::iterator Iterator;
	typedef std::map<string, Property*>::const_iterator ConstIterator;

	ConstIterator iterator() const {
		return properties.begin();
	}
	ConstIterator end() const {
		return properties.end();
	}

private:

	bool parseAux(InputBuffer &buffer);
	string getString(const string &key) const;

	std::map<string, Property*> properties;
	mutable int encodedSize;
};

} // namespace bolt


#endif /* bolt_Meta_h */

