
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_InputBuffer_h
#define bolt_InputBuffer_h


#include <bolt/Types.h>


namespace bolt {


class ByteBuffer;
class Decoder;


class InputBuffer
{
public:
	InputBuffer(const ByteBuffer &buffer);
	InputBuffer(const u8 *data, i32 len);
	~InputBuffer();

	bool isValid() const;

	i32 size() const {
		return count;
	}
	i32 available() const {
		return start - current + count;
	}

	const u8 *at() const {
		return current;
	}

	void advance(i32 n);

	i32 get1();
	i32 get2();
	i32 get3();
	i32 get4();
	bool get(u8 *s, int localCount);
	f64 getr8();


private:

	// prevent accidental copying and assignment
	InputBuffer(const InputBuffer &other);
	void operator=(const InputBuffer &other);

	Decoder &decoder;

	i32 count;
	const u8 *start;
	const u8 *current;
};


} // namespace bolt



#endif /* bolt_InputBuffer_h */
