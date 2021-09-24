
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#ifndef gmsec_internal_mathutil_h
#define gmsec_internal_mathutil_h

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>

#include <cstddef>	// for size_t

namespace gmsec
{
namespace util
{


const float SIMPLE = 1.5;

/**
* \fn getInfinity ()
* \brief Get double precision infinity value.
*/
double getInfinity();

/**
* \fn getInfinityf ()
* \brief Get single precision infinity value.
*/
float getInfinityf();

/**
* \fn getNaN ()
* \brief Get double precision NaN value.
*/
double getNaN();

/**
* \fn getNaNf ()
* \brief Get single precision NaN value.
*/
float getNaNf();


/**
* \fn realStringConstant (double x)
* \brief Returns the special string representation of \x if
*	there is one or 0 otherwise.
* \param x The value to determine the special string constant of.
*/
const char *realStringConstant(double x);

/**
* \fn float stringRealConstant (const char *p)
* \brief Returns the float value corresponding to \p if it is NAN
* or INF, or FP_TYPE_NORMAL if it is not a special string.
* \param p The string to test for special values.
*/
float stringRealConstant(const char *p);

/**
* \fn GMSEC_I64 stringToI64(const char *s)
* \brief converts a string to a 64-bit integer.
* \param s The string to convert.
* \return The converted value.
*/
GMSEC_API GMSEC_I64 CALL_TYPE stringToI64(const char *s);

/** \fn char * I64toString(GMSEC_I64 val, char *dest, size_t destSize)
* \brief converts a 64-bit integer to a string.  This buffer must be
* long enough to hold a string representation of 2^64 (24 or more characters).
* \param val The value to convert.
* \param dest The buffer which will hold the converted string.
* \return The converted string.
*/
GMSEC_API char * CALL_TYPE I64toString(GMSEC_I64 val, char *dest, std::size_t destSize);

} // namespace internal
} // namespace gmsec


#endif /* gmsec_internal_mathutil_h */
