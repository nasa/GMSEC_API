
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <iostream>
#include <bolt/Meta.h>
#include <bolt/Property.h>
#include <bolt/Log.h>

using namespace std;
using namespace bolt;



int main (int argc, char **argv)
{
	Log::SetDefaultStream(&cout);

	Meta meta;

	meta.add(Property::createString("E-NAME", "This is a string value"));
	meta.add(Property::createID("1-3-3-2323"));


	int size = meta.determineSize();
	cout << "Meta size is " << size << endl;

	ByteBuffer buffer;
	buffer.resize(size);

	OutputBuffer out(buffer);
	meta.put(out);

	cout << "Meta encoded is\n" << buffer << endl;

	Meta parsed;
	bool ok = parsed.parse(buffer);
	cout << "Meta.parse => " << ok << "\n";
	parsed.put(cout);
	cout << endl;

	return 0;
}

