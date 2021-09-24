
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Header_h
#define bolt_Header_h


#include <bolt/Types.h>


namespace bolt {

class ByteBuffer;
class InputBuffer;
class OutputBuffer;


class Header
{
public:

	static const int SIZE = 8;
	static const int SYNC = 0xb5;

	static const int F_META  = 0x01;
	static const int F_BODY	 = 0x02;
	static const int F_WARN	 = 0x10;
	static const int F_ERROR = 0x20;


	Header(PacketType type);
	Header(const Header &other);
	~Header();

	Header& operator=(const Header& other);

	static Header *parse(const ByteBuffer &buffer);

	/* puts
		size	4 bytes
		type	1 byte
		SYNC	1 byte
		flags	1 byte
		0	1 byte
	*/
	bool put(OutputBuffer &buffer, i32 size, int flags) const;

	PacketType getType() const {
		return type;
	}

	i32 getSize() const {
		return size;
	}

	int getFlags() const {
		return flags;
	}

	bool hasMeta() const {
		return (flags & F_META) != 0;
	}

	bool hasBody() const {
		return (flags & F_BODY) != 0;
	}

	bool hasProblems() const {
		return (flags & (F_WARN|F_ERROR)) != 0;
	}

	std::ostream &put(std::ostream &os, const char *prefix = "") const;


private:

	bool parseAux(InputBuffer &buffer);


	i32 size;		// 4 bytes
	PacketType type;
	int flags;
};

} // namespace bolt


#endif /* bolt_Header_h */
