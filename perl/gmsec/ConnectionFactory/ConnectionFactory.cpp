
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * ConnectionFactory.cpp
 *
 * Description: Class for creating and destroying connections
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__ConnectionFactory(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__ConnectionFactory(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__ConnectionFactory(CV *cv, CPerlObj *);
#endif

void storeFactoryStatus(Status result)
{
// get the last result pointer from the perl environment

   IV pointer;
   Status *status = NULL;
   SV* perlStatPtr = NULL;

   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 perlStatPtr = get_sv("GMSECAPI::ConnectionFactory::statusPointer", TRUE);
	 if (SvIOK(perlStatPtr))
	 {
	    status = (Status *) SvIV(perlStatPtr);

	    if (status == NULL)
	    {
		// store the result pointer as an object property
		pointer = (IV) new Status(result);
		sv_setiv(perlStatPtr, pointer);
	    }
	    else
	    {
		*status = result;
	    }
	 }
	 else
	 {
	    pointer = (IV) new Status(result);
	    sv_setiv(perlStatPtr, pointer);
	 }

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}



GMSEC_XS_STATIC XS(_PGA_ConnectionFactory_Create)
{
	HV *connHash;
	SV *connPointer;
	HV *connStash;
	SV *connPerlObject;
	IV pointer;
	GMSEC_CONNECTION_TYPE connectionType;
	Config *cfg = (Config *) 0;
	Connection *conn = (Connection *) 0;
	Status result;
	dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1 && items != 2)
	 {
		result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConnectionFactory] Usage: $conn ="
		           " Create($connType, $cfg); OR $conn = Create($cfg);");
		// store the result pointer as an object property
		storeFactoryStatus(result);
		XSRETURN_UNDEF;
	 }

	 if (items == 1)
	 {
		if (!sv_isa((SV*) ST(0), "GMSECAPI::Config"))
		{
			result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_OTHER_ERROR,
			           "[GMSECAPI::ConnectionFactory] [Create] first"
			           " argument is not a blessed Config object");
			// store the result pointer as an object property
			storeFactoryStatus(result);
			XSRETURN_UNDEF;
		}

		getObject(ST(0), cfg);

		result = ConnectionFactory::Create(cfg, conn);
	 }
	 else
	 {
		connectionType = SvUV(ST(0));

		if (!sv_isa((SV*) ST(1), "GMSECAPI::Config"))
		{
			result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_OTHER_ERROR,
			           "[GMSECAPI::ConnectionFactory] [Create] secondi"
			           " argument is not a blessed Config object");
			// store the result pointer as an object property
			storeFactoryStatus(result);
			XSRETURN_UNDEF;
		}

		getObject(ST(1), cfg);

		result = ConnectionFactory::Create(connectionType, cfg, conn);
	 }

	 //ConnectionFactory::Destroy(conn);
	 storeFactoryStatus(result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (!result.isError())
   {
	//return the Connection object
	connHash = newHV();

	pointer = (IV) conn;
	connPointer = newSVuv(pointer);
	hv_store(connHash, "pointer", 7, connPointer, 0);

	connPerlObject = sv_2mortal(newRV_inc(sv_2mortal((SV *) connHash)));
	connStash = gv_stashpv("GMSECAPI::Connection", 1);
	sv_bless(connPerlObject, connStash);

	ST(0) = connPerlObject;
	XSRETURN(1);

   }
   else
   {
	XSRETURN_UNDEF;
   }
}


GMSEC_XS_STATIC XS(_PGA_ConnectionFactory_Destroy)
{
   Status * objStatus = NULL;
   Connection *conn = (Connection *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConnectionFactory] Usage: Destroy($conn);");
		// store the result pointer as an object property
		storeFactoryStatus(result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), conn);
	 //Status * objStatus = NULL;
	 getStatus(ST(0), objStatus);
	 //if (objStatus) delete objStatus;

	 result = ConnectionFactory::Destroy(conn);

	 // store the result pointer as an object property
	 storeFactoryStatus(result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (objStatus) delete objStatus;
 
   if (result.GetCode() == 0)
   {
	XSRETURN_YES;
   }
   else
   {
	XSRETURN_NO;
   }
}



GMSEC_XS_STATIC XS(_PGA_ConnectionFactory_GetAPIVersion)
{
   //Connection *conn = (Connection *) 0;
   const char *version = NULL;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 0)
	 {
		result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConnectionFactory] Usage: $version = "
		           "GMSECAPI::ConnectionFactory::GetAPIVersion();");
		// store the result pointer as an object property
		storeFactoryStatus(result);
		XSRETURN_UNDEF;
	 }

	 version = ConnectionFactory::GetAPIVersion();

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (version != NULL)
   {
	XSRETURN_PV(version);
   }
   else
   {
	XSRETURN_UNDEF;
   }
}

LINKAGE XS(boot_GMSECAPI__ConnectionFactory)
{
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	 newXS("GMSECAPI::ConnectionFactory::Create", _PGA_ConnectionFactory_Create, __FILE__);
	 newXS("GMSECAPI::ConnectionFactory::Destroy", _PGA_ConnectionFactory_Destroy, __FILE__);
	 newXS("GMSECAPI::ConnectionFactory::GetAPIVersion", _PGA_ConnectionFactory_GetAPIVersion, __FILE__);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}
