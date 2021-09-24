
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <iostream>
#include <bolt/rawbuf.h>


using namespace std;
using namespace bolt;




int main (int argc, char **argv)
{
	Rawbuf<32> other;
	other.stream() << "x ";
	for (int i = 0; i < argc; ++i)
		other.stream() << argv[i];
	cout << "other |" << other.str() << "|\n";

	return 0;
}

