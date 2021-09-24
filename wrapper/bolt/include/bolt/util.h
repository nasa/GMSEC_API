
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_util_h
#define bolt_util_h

/*
 * @file util.h
 */


#include <bolt/Types.h>
#include <bolt/Result.h>

#include <cstdlib>
#include <cstring>
#include <typeinfo>


namespace bolt {
namespace util {


void zeroBytes(void *p, size_t count);

string toString(const Result &result);

string toString(i32 x);
string toString(f64 x);

string toString(Code code);
string toString(Error error);
string toString(State state);
string toString(PacketType type);
string toString(Signal s);
string toString(ConnectState s);

string getname(const std::type_info &t);
string demangle(const char *s);


} // namespace util
} // namespace bolt


#endif /* bolt_util_h */

