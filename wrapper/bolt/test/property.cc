
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <iostream>
#include <bolt/rawbuf.h>
#include <bolt/Property.h>

using namespace std;
using namespace bolt;



void test(Property * p)
{
	cout << "and " << *p << endl;
	delete p;
}


int main (int argc, char **argv)
{
	Rawbuf<32> other;
	other.stream() << "x ";
	for (int i = 0; i < argc; ++i)
		other.stream() << argv[i];
	cout << "other |" << other.str() << "|\n";

	test(Property::createString("Ralph", "is a good boy"));

	test(Property::createInteger("EXAMPLE-I32", 32));
	test(Property::createReal("EXAMPLE-F64", -23.221e234));
	test(Property::createID("ID-1-3-2323"));
	test(Property::createTopic("GMSEC.TEST.>"));

	return 0;
}

