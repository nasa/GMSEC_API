/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_MATHUTIL_H
#define GMSEC_API5_INTERNAL_MATHUTIL_H

#include <gmsec5_defs.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API MathUtil
{
public:
	static bool CALL_TYPE floatEquals(GMSEC_F32 f1, GMSEC_F32 f2);

	static bool CALL_TYPE doubleEquals(GMSEC_F64 d1, GMSEC_F64 d2);

	static GMSEC_F32 CALL_TYPE extractF32(const char* hexstr);

	static GMSEC_F64 CALL_TYPE extractF64(const char* hexstr);

	static double CALL_TYPE getInfinity();

	static float CALL_TYPE getInfinityf();

	static double CALL_TYPE getNaN();

	static float CALL_TYPE getNaNf();

	static const char* CALL_TYPE realStringConstant(double x);

	static float CALL_TYPE stringRealConstant(const char* p);
};


} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
