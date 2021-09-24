/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_MATHUTIL_H
#define GMSEC_API_INTERNAL_MATHUTIL_H

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{
namespace internal
{

class MathUtil
{
public:
	static bool floatEquals(GMSEC_F32 f1, GMSEC_F32 f2);

	static bool doubleEquals(GMSEC_F64 d1, GMSEC_F64 d2);

	static GMSEC_F32 extractF32(const char* hexstr);

	static GMSEC_F64 extractF64(const char* hexstr);

	static double getInfinity();

	static float getInfinityf();

	static double getNaN();

	static float getNaNf();

	static const char* realStringConstant(double x);

	static float stringRealConstant(const char* p);
};


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
