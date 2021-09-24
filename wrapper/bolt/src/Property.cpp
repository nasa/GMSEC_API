/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <bolt/Property.h>
#include <bolt/Log.h>
#include <bolt/Exception.h>
#include <bolt/util.h>

#include <gmsec4/internal/Rawbuf.h>
#include <gmsec4/internal/StringUtil.h>

#include <sstream>


namespace bolt {

using std::ostream;


class StringProperty : public Property
{
public:
	StringProperty(const string &name, const string &v)
		: Property(name), value(v) { }
	virtual ~StringProperty() { }

	StringProperty *_clone() const {
		return new StringProperty(getName(), value);
	}

	string asString() const {
		return value;
	}
	bool asFlag() const {
		if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value.c_str(), "false"))
			return false;
		if (value == "0")
			return false;
		if (gmsec::api::util::StringUtil::stringEqualsIgnoreCase(value.c_str(), "no"))
			return false;
		return true;
	}
	i32 asInteger() const {
		i32 tmp = 0;
		if (gmsec::api::util::StringUtil::stringParseI32(value.c_str(), tmp))
			return tmp;
		GMSEC_WARNING << "StringProperty.asInteger: unable to convert";
		return 0;
	}
	f64 asReal() const {
		GMSEC_F64 tmp = 0;
		if (gmsec::api::util::StringUtil::stringParseF64(value.c_str(), tmp))
			return tmp;
		GMSEC_WARNING << "StringProperty.asReal: unable to convert";
		return 0;
	}
	int dataSize() const {
		return value.length();
	}
	bool _put(OutputBuffer &buffer) const {
		return buffer.put(value);
	}
	bool _get(InputBuffer &buffer)  {
		if (buffer.available())
			value = string((char*) buffer.at(), buffer.available());
		return true;
	}
	ostream &_put(ostream &out) const {
		out << value;
		return out;
	}
private:
	string value;
};


class IntegerProperty : public Property
{
public:
	IntegerProperty(const string &name, i32 v)
		: Property(name), value(v) { }
	virtual ~IntegerProperty() { }

	IntegerProperty *_clone() const {
		return new IntegerProperty(getName(), value);
	}

	string asString() const {
		gmsec::api::util::Rawbuf<32> buffer;
		buffer.stream() << value;
		return buffer.str();
	}
	bool asFlag() const {
		return value != 0;
	}
	i32 asInteger() const {
		return value;
	}
	f64 asReal() const {
		return value;
	}
	int dataSize() const {
		return 4;
	}
	bool _put(OutputBuffer &buffer) const {
		return buffer.put4(value);
	}
	bool _get(InputBuffer &buffer) {
		value = buffer.get4();
		return true;
	}
	ostream &_put(ostream &out) const {
		out << value;
		return out;
	}
private:
	i32 value;
};


class RealProperty : public Property
{
public:
	RealProperty(const string &name, f64 v)
		: Property(name), value(v) { }
	virtual ~RealProperty() { }

	RealProperty *_clone() const {
		return new RealProperty(getName(), value);
	}

	string asString() const {
		gmsec::api::util::Rawbuf<64> buffer;
		buffer.stream() << value;
		return buffer.str();
	}
	bool asFlag() const {
		return value != 0;
	}
	i32 asInteger() const {
		i32 tmp = i32(value);
		if (tmp != value)
		{
			GMSEC_WARNING << "RealProperty.asInteger: truncated " << value;
		}
		return tmp;
	}
	f64 asReal() const {
		return value;
	}
	int dataSize() const {
		return 8;
	}
	bool _put(OutputBuffer &buffer) const {
		return buffer.putr8(value);
	}
	bool _get(InputBuffer &buffer) {
		value = buffer.getr8();
		return true;
	}
	ostream &_put(ostream &out) const {
		out << value;
		return out;
	}
private:
	f64 value;
};


class FlagProperty : public Property
{
public:
	FlagProperty(const string &name, bool v)
		: Property(name), value(v) { }
	virtual ~FlagProperty() { }

	FlagProperty *_clone() const {
		return new FlagProperty(getName(), value);
	}

	string asString() const {
		static string s_true("true");
		static string s_false("false");
		if (value)
			return s_true;
		return s_false;
	}
	bool asFlag() const {
		return value;
	}
	i32 asInteger() const {
		i32 tmp = value ? 1 : 0;
		return tmp;
	}
	f64 asReal() const {
		f64 tmp = value ? 1 : 0;
		return tmp;
	}
	int dataSize() const {
		return 1;
	}
	bool _put(OutputBuffer &buffer) const {
		i32 tmp = value ? 1 : 0;
		return buffer.put1(tmp);
	}
	bool _get(InputBuffer &buffer) {
		i32 tmp = buffer.get1();
		value = tmp != 0;
		return true;
	}
	ostream &_put(ostream &out) const {
		out << value;
		return out;
	}
private:
	bool value;
};


Property::Property(const string &n)
	:
	name(n),
	type(TYPE_UNKNOWN),
	named(true)
{
}


Property::~Property()
{
}



Property *Property::clone() const
{
	Property *p = _clone();
	p->setType(getType());
	p->setNamed(isNamed());
	return p;
}


Property *Property::createID(const string &id)
{
	Property *p = new StringProperty("ID", id);
	p->setType(TYPE_ID);
	p->setNamed(false);
	return p;
}


Property *Property::createTopic(const string &topic)
{
	Property *p = new StringProperty("TOPIC", topic);
	p->setType(TYPE_TOPIC);
	p->setNamed(false);
	return p;
}


Property *Property::createCorrID(const string &id)
{
	Property *p = new StringProperty("CORR_ID", id);
	p->setType(TYPE_CORR_ID);
	p->setNamed(false);
	return p;
}


Property *Property::createReplyTo(const string &topic)
{
	Property *p = new StringProperty("REPLY_TO", topic);
	p->setType(TYPE_REPLY_TO);
	p->setNamed(false);
	return p;
}

Property *Property::createSelector(const string &selector)
{
	Property *p = new StringProperty("SELECTOR", selector);
	p->setType(TYPE_SELECTOR);
	p->setNamed(false);
	return p;
}


static bool checkKey(const string &key)
{
	bool ok = false;
	if (key.length() < 256)
		ok = true;
	else
		GMSEC_WARNING << "invalid property name " << key.c_str();
	return ok;
}


Property *Property::createString(const string &key, const string &value)
{
	if (!checkKey(key))
		return 0;
	Property *p = new StringProperty(key, value);
	p->setType(TYPE_STRING);
	return p;
}


Property *Property::createInteger(const string &key, i32 value)
{
	if (!checkKey(key))
		return 0;
	Property *p = new IntegerProperty(key, value);
	p->setType(TYPE_I32);
	return p;
}


Property *Property::createReal(const string &key, f64 value)
{
	if (!checkKey(key))
		return 0;
	Property *p = new RealProperty(key, value);
	p->setType(TYPE_F64);
	return p;
}


Property *Property::createFlag(const string &key, bool value)
{
	if (!checkKey(key))
		return 0;
	Property *p = new FlagProperty(key, value);
	p->setType(TYPE_FLAG);
	return p;
}


i32 Property::getEncoding(int &prefix) const
{
	EncMode emode = ENC_NULL;
	return getEncoding(emode, prefix);
}


i32 Property::getEncoding(EncMode &etype, int &prefix) const
{
	etype = ENC_NULL;

	i32 count = dataSize();

	// the prefix consists of etype|type and length.
	prefix = 0;

	++count;	// a byte for the name length
	if (isNamed())
		count += name.length();

	if (count < 256)
	{
		etype = ENC_1;
		prefix = 2;
	}
	else if (count < 256*256)
	{
		etype = ENC_2;
		prefix = 3;
	}
	else if (count < 256*256*256)
	{
		etype = ENC_3;
		prefix = 4;
	}
	else
	{
		GMSEC_WARNING << "ignoring Property " << name.c_str() << " with count=" << count;
		count = 0;
	}

	return count;
}


bool Property::put(OutputBuffer &buffer) const
{
	EncMode etype = ENC_NULL;
	int prefix = 0;
	int count = getEncoding(etype, prefix);

	if (etype != ENC_1 && etype != ENC_2 && etype != ENC_3)
	{
		GMSEC_WARNING << "Property.put: failed: name=" << name.c_str() << " etype=" << etype << " count=" << count;
		return false;
	}

	int fulltype = (etype << 6) | getType();
	buffer.put1(fulltype);
	if (etype == ENC_1)
		buffer.put1(count);
	else if (etype == ENC_2)
		buffer.put2(count);
	else if (etype == ENC_3)
		buffer.put3(count);
	else
	{
		GMSEC_ERROR << "Property.put: logic error: name=" << name.c_str() << " etype=" << etype << " count=" << count;
		return false;
	}

	if (isNamed())
	{
		buffer.put1(name.length());
		buffer.put(name);
	}
	else
		buffer.put1(0);

	bool ok = _put(buffer);
	if (!ok)
	{
		GMSEC_ERROR << "Property.put: _put failed: name=" << name.c_str() << " etype=" << etype << " count=" << count;
	}

	return ok;
}


ostream &Property::put(ostream &out) const
{
	out << "Property[name=" << getName() << ", type=" << type << ", value=";
	_put(out);
	out << "]\n";
	return out;
}


Property *Property::get(InputBuffer &buffer)
{
	int ptype = buffer.get1();
	EncMode encmode = (EncMode) (ptype >> 6);
	i32 proptype = ptype & 0x3f;

	i32 proplength = 0;

	switch (encmode)
	{
	case ENC_1:
		proplength = buffer.get1();
		break;
	case ENC_2:
		proplength = buffer.get2();
		break;
	case ENC_3:
		proplength = buffer.get3();
		break;
	default:
		{
			std::stringstream ss;
			ss << "invalid encmode=" << encmode;
			throw IOException(ss.str().c_str());
		}
	}

	InputBuffer propbuf(buffer.at(), proplength);

	buffer.advance(proplength);

	string key;
	int namelen = propbuf.get1();
	if (namelen > 0)
	{
		u8 tmp[256]; // 0 < namelen < 256
		tmp[0] = 0;
		propbuf.get(tmp, namelen);
		key = string((char*) tmp, namelen);
	}

	Property *property = 0;
	static string empty;

	switch (proptype)
	{
	case TYPE_STRING:
		property = createString(key, empty);
		break;
	case TYPE_I32:
		property = createInteger(key, 0);
		break;
	case TYPE_F64:
		property = createReal(key, 0);
		break;
	case TYPE_FLAG:
		property = createFlag(key, false);
		break;
	case TYPE_ID:
		property = createID(empty);
		break;
	case TYPE_TOPIC:
		property = createTopic(empty);
		break;
	case TYPE_CORR_ID:
		property = createCorrID(empty);
		break;
	case TYPE_REPLY_TO:
		property = createReplyTo(empty);
		break;
	case TYPE_SELECTOR:
		property = createSelector(empty);
		break;
	default:
		throw IOException("invalid proptype=" + util::toString(proptype));
	}

	if (property) {
		property->_get(propbuf);
	}

	return property;
}



} // namespace bolt

