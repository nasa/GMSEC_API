
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <iostream>
#include <bolt/Encoder.h>
#include <bolt/Buffer.h>
#include <bolt/Exception.h>
#include <bolt/Input.h>
#include <bolt/Output.h>


using namespace std;
using namespace bolt;




void testOutput(ByteBuffer &buffer)
{
	OutputBuffer out(buffer);

	out.put4(2 * 256 + 3);
	cout << "after put4(2*256 + 3):\n\t" << buffer << endl;

	out.put1(13);
	cout << "after put1(13):\n\t" << buffer << endl;

	out.put2(8);
	cout << "after put2(8):\n\t" << buffer << endl;

	char abc[] = { 'a', 'b', 'c' };
	out.put(abc, sizeof(abc));
	cout << "after put(abc):\n\t" << buffer << endl;

	for (int i = 0; i < 8; ++i) {
		out.put2(i);
		cout << "after put2(i):\n\t" << buffer << endl;
	}
}


int main (int argc, char **argv)
{
	char raw[16] = { 0 };
	ByteBuffer buffer;
	buffer.set(raw, sizeof(raw), false);
	cout << "raw buffer:\n\t" << buffer << endl;

	try {
		testOutput(buffer);
	}
	catch (Exception &e) {
		cout << "caught " << e.getText() << endl;
	}


	InputBuffer in(buffer);
	i32 i = in.get4();
	cout << "get4 => " << i << endl;

	i = in.get1();
	cout << "get1 => " << i << endl;

	i = in.get2();
	cout << "get2 => " << i << endl;

	u8 xyz[3];
	in.get(xyz, sizeof(xyz));
	cout << "get(xyz) => " << xyz[0] << xyz[1] << xyz[2] << endl;

	return 0;
}

