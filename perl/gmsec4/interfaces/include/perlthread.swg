/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* -----------------------------------------------------------------------------
 * perlthread.swg
 *
 * This file contains a static variable definition and macros for the use of
 * the PerlInterpreter which is used to make a function call from a thread
 * running external to the main Perl execution thread. (e.g. C++ -> Perl Code
 * for Automated Callbacks)
 * ----------------------------------------------------------------------------- */


%{
static PerlInterpreter* orig_perl = NULL;

/*
 * Use the perlguts macro "dTHX" to obtain the PerlInterpreter of the thread
 * invoking the function where this macro is used
 */
#define GMSEC_SWIG_INIT_PERL_CONTEXT \
dTHX; \
orig_perl = my_perl;

/*
 * Use the perlguts macro "PERL_SET_CONTEXT" to assign the PerlInterpreter
 * held in orig_perl for use in the function call where this macro is used
 */
#define GMSEC_SWIG_SET_PERL_CONTEXT \
if (orig_perl) \
{ \
	PERL_SET_CONTEXT(orig_perl); \
} \
else \
{ \
	GMSEC_ERROR << "Unable to call onMessage for callback; Perl Context was not set in Connection::create"; \
	return; \
}
%}

