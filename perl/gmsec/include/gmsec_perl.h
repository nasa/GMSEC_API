
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * gmsec_perl.h
 *
 * Description: Perl support library for the GMSEC API
 *
 *--------------------------------------------------------------------------*/

// GMSEC API
#include <gmsec_cpp.h>
#include <iostream>

using namespace std;

#include "EXTERN.h"
#include "perl.h"
#define NO_XSLOCKS
#include "XSUB.h"
#ifdef WIN32
#define GMSEC_XS_EXPORT(x) __declspec(dllexport) x
#define GMSEC_XS_STATIC
#else
#define GMSEC_XS_EXPORT(x) x
#define GMSEC_XS_STATIC
#endif


#ifdef __cplusplus
#define LINKAGE extern "C"
#else
#define LINKAGE
#endif

#ifdef WIN32
#ifdef GMSEC_VC6
#define FPRINTF fprintf
#else
#define FPRINTF fprintf_s
#endif /* GMSEC_VC6 */
#else
#define FPRINTF fprintf
#endif

//----------------------------------------------------------
// Define these macros here because depending on the version
// of XSUB.h, they may not be available.
//----------------------------------------------------------
#ifndef dXCPT
#define dXCPT             dJMPENV; int rEtV = 0
#endif

#ifndef XCPT_TRY_START
#define XCPT_TRY_START 	JMPENV_PUSH(rEtV); if (rEtV == 0)
#endif

#ifndef XCPT_TRY_END
#define XCPT_TRY_END 	JMPENV_POP;
#endif

#ifndef XCPT_CATCH
#define XCPT_CATCH	if (rEtV != 0)
#endif

#ifndef XCPT_RETHROW
#define XCPT_RETHROW      JMPENV_JUMP(rEtV)
#endif

// --- End of XSUB.h macro definitions.

#define GMSEC_XCPT_TRY try

#define GMSEC_XCPT_CATCH \
    catch (bad_alloc& e) \
    { \
        FPRINTF(stderr, "%s:%d: encountered bad_alloc %s\n", __FILE__, __LINE__, e.what()); \
        XCPT_CATCH \
        { \
           XCPT_RETHROW; \
        } \
    } \
    catch (exception& e) \
    { \
        FPRINTF(stderr, "%s:%d: encountered exception %s\n", __FILE__, __LINE__, e.what()); \
        XCPT_CATCH \
        { \
           XCPT_RETHROW; \
        } \
    } \
    catch (...) \
    { \
        FPRINTF(stderr, "%s:%d: encountered unknown exception\n", __FILE__, __LINE__); \
        XCPT_CATCH \
        { \
           XCPT_RETHROW; \
        } \
    }


#define USE_ITEMS if (items % 1000 != items) return;


using namespace gmsec;


// class used for executing a message callback in perl
class PerlCallback : public Callback
{
private:
	// not implemented
	PerlCallback(const PerlCallback &);
	PerlCallback &operator=(const PerlCallback &);

	SV *cbPerlObject;
	char *fullMethodName;


	void setMethod(const char *package, const char *method);

public:
	PerlCallback(SV *object);
	PerlCallback(SV *object, const char *package, const char *method);

	~PerlCallback();

	virtual void CALL_TYPE OnMessage(Connection *conn, Message *msg);
};


// functions used to retrieve a GMSEC class object from a perl object
void getObject(SV *perlObject, Connection *&conn);
void getObject(SV *perlObject, Config *&cfg);
void getObject(SV *perlObject, Field *&fld);
void getObject(SV *perlObject, Message *&msg);
void getObject(SV *perlObject, Status *&stat);
void getObject(SV *perlObject, ConfigFile *&cfg_file);

// function used to retrieve the status pointer from the perl property
void getStatus(SV *perlObject, Status *&stat);
// function used to store a status pointer as a perl object property
void storeStatus(SV *perlObject, Status result);

void setDispatchInterpreter(PerlInterpreter *interpreter);

