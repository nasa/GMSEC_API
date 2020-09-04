
/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * ConfigFile.cpp
 *
 * Description: Class for managing GMSEC config files
 *
 *--------------------------------------------------------------------------*/

// GMSEC API and Perl support library
#include "gmsec_perl.h"


#ifndef PERL_OBJECT
#ifndef MULTIPLICITY
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__ConfigFile(CV* cv);
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__ConfigFile(pTHXo_ CV* cv);
#endif
#else
LINKAGE GMSEC_XS_EXPORT(void) boot_GMSECAPI__ConfigFile(CV *cv, CPerlObj *);
#endif


GMSEC_XS_STATIC XS(_PGA_ConfigFile_Create)
{
   //SV *stPerlVar;
   HV *self;
   SV *hvPointer;
   IV pointer;
   ConfigFile *cfg_file = (ConfigFile *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	 if (items != 2)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] Usage: ConfigFile(<config file path>);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::ConfigFile"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 cfg_file = new ConfigFile(SvPV_nolen(ST(1)));

	 // store object pointer
	 self = (HV *) SvRV(ST(0));
	 pointer = (IV) cfg_file;
	 hvPointer = newSVuv(pointer);
	 hv_store(self, "pointer", 7, hvPointer, 0);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH


   XSRETURN_YES;
}


GMSEC_XS_STATIC XS(_PGA_ConfigFile_Load)
{
   ConfigFile *cfg_file = (ConfigFile *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 1)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] Usage: $cfg_file->Load();");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::ConfigFile"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] [Load] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg_file);
	 result = cfg_file->Load();
	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
      XSRETURN_YES;
   }
   else
   {
      XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_ConfigFile_LookupConfig)
{
   ConfigFile *cfg_file = (ConfigFile *) 0;
   const char *name;
   Config *cfg = (Config *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	 if (items != 3)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] Usage: $cfg_file->LookupConfig($name, $cfg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::ConfigFile"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [LookupConfig] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(2), "GMSECAPI::Config"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [LookupConfig] third argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg_file);
	 name = SvPV_nolen(ST(1));
	 getObject(ST(2), cfg);

	 result = cfg_file->LookupConfig(name, cfg);
	 storeStatus(ST(0), result);

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
      XSRETURN_YES;
   }
   else
   {
      XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_ConfigFile_LookupMessage)
{
   ConfigFile *cfg_file = (ConfigFile *) 0;
   const char *name;
   Message *msg = (Message *) 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 3)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] Usage: $cfg_file->LookupMessage($name, $msg);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::ConfigFile"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [LookupMessage] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(2), "GMSECAPI::Message"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [LookupConfig] third argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg_file);
	 name = SvPV_nolen(ST(1));
	 getObject(ST(2), msg);

	 result = cfg_file->LookupMessage(name, msg);
	 storeStatus(ST(0), result);

       }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0)
   {
      XSRETURN_YES;
   }
   else
   {
      XSRETURN_NO;
   }
}


GMSEC_XS_STATIC XS(_PGA_ConfigFile_LookupSubscription)
{
   ConfigFile *cfg_file = (ConfigFile *) 0;
   const char *name = 0;
   const char *subscr = 0;
   Status result;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 if (items != 3)
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::ConfigFile] Usage: $cfg_file->LookupSubscription($name, $subscr);");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::ConfigFile"))
	 {
		result.Set(GMSEC_STATUS_CONFIG_ERROR, GMSEC_OTHER_ERROR,
		           "[GMSECAPI::Config] [LookupSubscription] first argument is not a blessed object");
		storeStatus(ST(0), result);
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfg_file);
	 name = SvPV_nolen(ST(1));

	 result = cfg_file->LookupSubscription(name, subscr);
	 storeStatus(ST(0), result);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (result.GetCode() == 0 && subscr != NULL)
   {
      sv_setpv(ST(2), subscr);
      XSRETURN_YES;
   }
   else
   {
      sv_setpv(ST(2), "");
      XSRETURN_NO;
   }

}



GMSEC_XS_STATIC XS(_PGA_ConfigFile_DESTROY)
{
   Status * objStatus = NULL;
   ConfigFile *cfgf = (ConfigFile *) 0;
   dXSARGS;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 // warn("[GMSECAPI::ConfigFile] $obj->DESTROY();");
	 if (items != 1)
	 {
		warn("[GMSECAPI::ConfigFile] Usage: $cfgfile->DESTROY();");
		XSRETURN_UNDEF;
	 }

	 if (!sv_isa((SV*) ST(0), "GMSECAPI::ConfigFile"))
	 {
		warn("[GMSECAPI::ConfigFile] [DESTROY] first argument is not a blessed object");
		XSRETURN_UNDEF;
	 }

	 getObject(ST(0), cfgf);
	 getStatus(ST(0), objStatus);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   if (objStatus) delete objStatus;

   if (cfgf) delete cfgf;
}


LINKAGE XS(boot_GMSECAPI__ConfigFile)
{
   dXSARGS;
   dXCPT;

   USE_ITEMS;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	 newXS("GMSECAPI::ConfigFile::ConfigFile",  _PGA_ConfigFile_Create, __FILE__);
	 newXS("GMSECAPI::ConfigFile::Load", _PGA_ConfigFile_Load, __FILE__);
	 newXS("GMSECAPI::ConfigFile::LookupMessage", _PGA_ConfigFile_LookupMessage, __FILE__);
	 newXS("GMSECAPI::ConfigFile::LookupConfig", _PGA_ConfigFile_LookupConfig, __FILE__);
	 newXS("GMSECAPI::ConfigFile::LookupSubscription", _PGA_ConfigFile_LookupSubscription, __FILE__);
	 newXS("GMSECAPI::ConfigFile::DESTROY", _PGA_ConfigFile_DESTROY, __FILE__);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH

   XSRETURN_YES;
}
