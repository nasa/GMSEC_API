
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Property_h
#define bolt_Property_h


#include <bolt/InputBuffer.h>
#include <bolt/OutputBuffer.h>


namespace bolt {


class Property
{
public:

	enum EncMode
	{
		ENC_NULL = -1,
		ENC_1,
		ENC_2,
		ENC_3,
		ENC_END
	};

	enum Type
	{
		TYPE_UNKNOWN	= 0,

		TYPE_STRING	= 1,

		TYPE_BLOB	= 2,

		TYPE_I32	= 3,
		TYPE_F64	= 4,
		TYPE_FLAG	= 5,

		TYPE_ID		= 21,
		TYPE_TOPIC	= 22,
		TYPE_CORR_ID	= 23,
		TYPE_REPLY_TO	= 24,
		TYPE_SELECTOR	= 25,

		TYPE_COMPRESS	= 27,

		TYPE_END
	};

	virtual ~Property();

	Property *clone() const;

	string getName() const {
		return name;
	}

	Type getType() const {
		return type;
	}

	bool isNamed() const {
		return named;
	}

	bool put(OutputBuffer &out) const;
	std::ostream &put(std::ostream &out) const;

	virtual Property *_clone() const = 0;
	virtual int dataSize() const = 0;

	virtual string asString() const = 0;
	virtual bool asFlag() const = 0;
	virtual i32 asInteger() const = 0;
	virtual f64 asReal() const = 0;


	static Property *createString(const string &key, const string &value);
	static Property *createFlag(const string &key, bool value);
	// static Property *createBlob(const string &key, const ByteBuffer &value);
	static Property *createInteger(const string &key, i32 value);
	static Property *createReal(const string &key, f64 value);

	// special properties
	static Property *createID(const string &value);
	static Property *createTopic(const string &value);
	static Property *createCorrID(const string &value);
	static Property *createReplyTo(const string &value);
	static Property *createSelector(const string &value);


	static Property * get(InputBuffer &in);

	i32 getEncoding(int &prefix) const;


protected:

	Property(const string &name);

	virtual bool _put(OutputBuffer &out) const = 0;
	virtual std::ostream &_put(std::ostream &out) const = 0;

	virtual bool _get(InputBuffer &out) = 0;


private:

	void setType(Type id) {
		type = id;
	}

	void setNamed(bool in) {
		named = in;
	}

	i32 getEncoding(EncMode &etype, int &prefix) const;

	string name;
	Type type;
	bool named;
};


} // namespace bolt


inline std::ostream &operator<<(std::ostream &os, const bolt::Property &property)
{
	return property.put(os);
}


#endif /* bolt_Property_h */

