
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_OutputBuffer_h
#define bolt_OutputBuffer_h


#include <bolt/ByteBuffer.h>


namespace bolt {


class Encoder;



class OutputBuffer
{
public:
	OutputBuffer(ByteBuffer &buffer);
	OutputBuffer(u8 *p, i32 count);
	~OutputBuffer();

	bool isValid() const;

	i32 size() const {
		return count;
	}
	i32 available() const {
		return i32(start - current + count);
	}

	u8 *at() const {
		return current;
	}

	bool put1(i32 i);
	bool put2(i32 i);
	bool put3(i32 i);
	bool put4(i32 i);
	bool put(const u8 *s, int localCount);
	bool put(const string &s) {
		return put((u8*) s.c_str(), s.size());
	}
	bool putr8(f64 x);


private:

	// prevent accidental copying and assignment
	OutputBuffer(const OutputBuffer &other);
	void operator=(const OutputBuffer &other);

	Encoder &encoder;

	i32 count;
	u8 *start;
	u8 *current;
};

} // namespace bolt


#endif /* bolt_OutputBuffer_h */
