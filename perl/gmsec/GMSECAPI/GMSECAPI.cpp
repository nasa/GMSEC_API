
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * GMSECAPI.cpp
 *
 * Description: GMSEC API constants
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI(CV *cv, CPerlObj *);
#endif


LINKAGE XS(boot_GMSECAPI)
{
   SV *sv;
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

#ifndef PERL_USE_SAFE_PUTENV
	PL_use_safe_putenv = TRUE;
#endif

	//
	// from gmsec_defs.h
	//

	sv = get_sv("GMSECAPI::GMSEC_CONNECTION_ICSSWB", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_CONNECTION_ICSSWB);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_CONNECTION_RENDEZVOUS", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_CONNECTION_RENDEZVOUS);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_CONNECTION_SMARTSOCKETS", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_CONNECTION_SMARTSOCKETS);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_UNSET", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_UNSET);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_CHAR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_CHAR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_BOOL", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_BOOL);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_SHORT", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_SHORT);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_USHORT", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_USHORT);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_LONG", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_LONG);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_ULONG", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_ULONG);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_I64", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_I64);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_FLOAT", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_FLOAT);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_DOUBLE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_DOUBLE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_STRING", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_STRING);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_BIN", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_BIN);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_I8", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_I8);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_U8", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_U8);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_I16", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_I16);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_U16", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_U16);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_I32", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_I32);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_U32", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_U32);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_F32", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_F32);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TYPE_F64", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TYPE_F64);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TRUE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TRUE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_FALSE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_FALSE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_MSG_UNSET", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_MSG_UNSET);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_MSG_PUBLISH", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_MSG_PUBLISH);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_MSG_REQUEST", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_MSG_REQUEST);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_MSG_REPLY", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_MSG_REPLY);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_NO_WAIT", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_NO_WAIT);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_WAIT_FOREVER", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_WAIT_FOREVER);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::NULL", TRUE | 0x2);
	sv_setiv(sv, (IV) 0);
	SvREADONLY_on(sv);

	//
	// from gmsec_errors.h
	//

	sv = get_sv("GMSECAPI::GMSEC_STATUS_NO_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_NO_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_FACTORY_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_FACTORY_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_CONNECTION_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_CONNECTION_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_CONFIG_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_CONFIG_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_MIDDLEWARE_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_MIDDLEWARE_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_MESSAGE_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_MESSAGE_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_FIELD_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_FIELD_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_CALLBACK_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_CALLBACK_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_STATUS_CALLBACKLKP_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_STATUS_CALLBACKLKP_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_LIBRARY_LOAD_FAILURE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_LIBRARY_LOAD_FAILURE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_CONNECTION_TYPE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_CONNECTION_TYPE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_AUTODISPATCH_FAILURE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_AUTODISPATCH_FAILURE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_CONNECTION", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_CONNECTION);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_FEATURE_NOT_SUPPORTED", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_FEATURE_NOT_SUPPORTED);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_CONFIG_VALUE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_CONFIG_VALUE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_CONFIG_END_REACHED", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_CONFIG_END_REACHED);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_MESSAGE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_MESSAGE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_UNKNOWN_MSG_TYPE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_UNKNOWN_MSG_TYPE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_FIELDS_END_REACHED", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_FIELDS_END_REACHED);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_FIELD_TYPE_MISMATCH", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_FIELD_TYPE_MISMATCH);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_UNKNOWN_FIELD_TYPE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_UNKNOWN_FIELD_TYPE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_CALLBACK", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_CALLBACK);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_REQDISPATCH_FAILURE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_REQDISPATCH_FAILURE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_MSGCONVERT_FAILURE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_MSGCONVERT_FAILURE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_NO_MESSAGE_AVAILABLE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_NO_MESSAGE_AVAILABLE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TIMEOUT_OCCURRED", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TIMEOUT_OCCURRED);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_TRACKING_FAILURE", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_TRACKING_FAILURE);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_UNUSED_CONFIG_ITEM", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_UNUSED_CONFIG_ITEM);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_FIELD", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_FIELD);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_XML_PARSE_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_XML_PARSE_ERROR);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_CONFIG", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_CONFIG);
	SvREADONLY_on(sv);

	sv = get_sv("GMSECAPI::GMSEC_INVALID_FIELD_NAME", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_INVALID_FIELD_NAME);
	SvREADONLY_on(sv);


	sv = get_sv("GMSECAPI::GMSEC_OTHER_ERROR", TRUE | 0x2);
	sv_setiv(sv, (IV) GMSEC_OTHER_ERROR);
	SvREADONLY_on(sv);


	ST(0) = &PL_sv_yes;

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN(1);
}

