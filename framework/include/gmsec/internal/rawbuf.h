
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_internal_rawbuf_h
#define gmsec_internal_rawbuf_h


#include <cstdio>
#include <ostream>


namespace gmsec
{
namespace util
{

/**
	rawbuf is an output streambuf backed by a fixed length character array.
	The rawbuf can own the array or not.
*/

class rawbuf : public std::streambuf
{
private:

	char *buffer;
	size_t allocated;
	bool owned;

public:

	rawbuf(char *p, size_t size, bool transfer = false)
			: buffer(p), allocated(size), owned(transfer)
	{
		setp(buffer, buffer + allocated);
	}

	rawbuf(size_t size)
			: buffer(new char[size]), allocated(size), owned(true)
	{
		setp(buffer, buffer + allocated);
	}

	~rawbuf()
	{
		if (owned)
			delete[] buffer;
		buffer = 0;
	}

	char *release()
	{
		owned = false;
		return buffer;
	}

protected:

	int overflow(int /* c */)
	{
		buffer[allocated-1] = 0;
		return EOF;
	}

};


template <size_t SPACE> class Rawbuf
{
public:

	Rawbuf() :
		buffer(&space[0], SPACE),
		out(&buffer) {
	}
	const char *raw() const {
		return &space[0];
	}
	std::streambuf *getbuf() {
		return &buffer;
	}
	std::ostream &stream() {
		return out;
	}
	const char *str() {
		out << std::ends;
		return &space[0];
	}

private:

	char space[SPACE];
	rawbuf buffer;
	std::ostream out;

};


} // namespace util
} // namespace gmsec

#endif /* gmsec_internal_rawbuf_h */
