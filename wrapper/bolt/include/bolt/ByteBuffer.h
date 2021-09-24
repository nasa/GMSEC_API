
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_ByteBuffer_h
#define bolt_ByteBuffer_h


#include <bolt/Types.h>

#include <ostream>


namespace bolt {


class ByteBuffer
{
public:
	ByteBuffer();
	ByteBuffer(u8 *p, i32 count, bool ownership);
	ByteBuffer(const ByteBuffer &other);
	~ByteBuffer();

	void resize(i32 size);

	void set(u8 *p, i32 count, bool ownership);
	void copy(const u8 *p, i32 count);

	void clear();

	i32 size() const {
		return _size;
	}

	u8 *raw() {
		return _buffer;
	}

	const u8 *raw() const {
		return _buffer;
	}


	std::ostream &put(std::ostream &os, const char *prefix = "") const;

private:

	// prevent accidental assignment
	void operator=(const ByteBuffer &other);

	bool _owned;
	u8 *_buffer;
	i32 _size;
};


} // namespace bolt


inline std::ostream &operator<<(std::ostream &os, const bolt::ByteBuffer &buffer)
{
	return buffer.put(os);
}


#endif /* bolt_ByteBuffer_h */
