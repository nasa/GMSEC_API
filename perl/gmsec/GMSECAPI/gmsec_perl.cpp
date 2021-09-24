
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/*----------------------------------------------------------------------------
 * gmsec_perl.cpp
 *
 * Description: Perl support library for the GMSEC API
 *
 *--------------------------------------------------------------------------*/


// GMSEC API and Perl support library
#include "gmsec_perl.h"
#include <gmsec/util/Mutex.h>

using namespace gmsec::util;


static PerlInterpreter * DISPATCH_INTERPRETER;


PerlCallback::PerlCallback(SV *object)
	:
	cbPerlObject(object),
	fullMethodName(0)
{
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	setMethod("GMSECAPI::Callback", "OnMessage");

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


PerlCallback::PerlCallback(SV *object, const char *package, const char *method)
	:	
	cbPerlObject(object),
	fullMethodName(0)
{
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	setMethod(package, method);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


PerlCallback::~PerlCallback()
{
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	delete[] fullMethodName;
	SvREFCNT_dec(cbPerlObject);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void PerlCallback::setMethod(const char * package, const char * method)
{
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	// length of <package>::<method> (and terminating null)
	size_t len = strlen(package) + 2 + strlen(method) + 1;
	fullMethodName = new char[len];
	sprintf(fullMethodName, "%s::%s", package, method);
	SvREFCNT_inc(cbPerlObject);

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


static Mutex &getDispatchMutex()
{
	static Mutex dispatchMutex;
	return dispatchMutex;
}


void PerlCallback::OnMessage(Connection *conn, Message *msg)
{

	// only allow one callback into the dispatch interpreter at a time...
	AutoMutex autoMutex(getDispatchMutex());

	if (!DISPATCH_INTERPRETER)
	{
		LOG_WARNING << "PerlCallback.OnMessage: no context for dispatching callbacks- call SetDispatchContext or StartAutoDispatch";
		return;
	}

	PERL_SET_CONTEXT(DISPATCH_INTERPRETER);

	dSP;

	HV *pHash;
	SV *pPointer;
	HV *pStash;
	SV *connPerlObject;
	SV *msgPerlObject;
	IV pointer;

	ENTER;
	SAVETMPS;
        dXCPT;

        GMSEC_XCPT_TRY
        {
           XCPT_TRY_START
           { 

	      // create the Perl Connection object
	      pHash = newHV();

	      pointer = (IV) conn;
	      pPointer = newSVuv(pointer);
	      hv_store(pHash, "pointer", 7, pPointer, 0);

	      connPerlObject = sv_2mortal(newRV_inc(sv_2mortal((SV *) pHash)));
	      pStash = gv_stashpv("GMSECAPI::Connection", 1);
	      sv_bless(connPerlObject, pStash);

	      // create the Perl Message object
	      pHash = newHV();

	      pointer = (IV) msg;
	      pPointer = newSVuv(pointer);
	      hv_store(pHash, "pointer", 7, pPointer, 0);

	      msgPerlObject = sv_2mortal(newRV_inc(sv_2mortal((SV *) pHash)));
	      pStash = gv_stashpv("GMSECAPI::Message", 1);
	      sv_bless(msgPerlObject, pStash);

	      // push arguments onto the stack
	      PUSHMARK(SP);
	      XPUSHs(cbPerlObject);
	      XPUSHs(connPerlObject);
	      XPUSHs(msgPerlObject);
	      PUTBACK;

	      // call the method
	      call_method(fullMethodName, G_DISCARD);

           }XCPT_TRY_END
        }
        GMSEC_XCPT_CATCH

	SPAGAIN;

	FREETMPS;
	LEAVE;

}



void setDispatchInterpreter(PerlInterpreter *in)
{
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {
	AutoMutex hold(getDispatchMutex());

	DISPATCH_INTERPRETER = in;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void getObject(SV *perlObject, Connection *&conn)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "pointer", 7, 0);
	pointer = SvIV(*hvPointer);
	conn = (Connection *) pointer;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void getObject(SV *perlObject, ConfigFile *&cfg_file)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "pointer", 7, 0);
	pointer = SvIV(*hvPointer);
	cfg_file = (ConfigFile *) pointer;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void getObject(SV *perlObject, Config *&cfg)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "pointer", 7, 0);
	pointer = SvIV(*hvPointer);
	cfg = (Config *) pointer;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void getObject(SV *perlObject, Field *&fld)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "pointer", 7, 0);
	pointer = SvIV(*hvPointer);
	fld = (Field *) pointer;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void getObject(SV *perlObject, Message *&msg)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "pointer", 7, 0);
	pointer = SvIV(*hvPointer);
	msg = (Message *) pointer;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}


void getObject(SV *perlObject, Status *&stat)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "pointer", 7, 0);
	pointer = SvIV(*hvPointer);
	stat = (Status *) pointer;

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}

void getStatus(SV *perlObject, Status *&stat)
{
   HV *self;
   SV **hvPointer;
   IV pointer;
   dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	stat = 0;

	self = (HV *) SvRV(perlObject);

	// obtain object from stored pointer
	hvPointer = hv_fetch(self, "status", 6, 1);

	if (SvIOK(*hvPointer))
	{
		pointer = SvIV(*hvPointer);
		stat = (Status *) pointer;
	}

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}

void storeStatus(SV *perlObject, Status result)
{
   HV *self;
   SV *stPointer;
   IV pointer;
   Status *status = NULL;
    dXCPT;

   GMSEC_XCPT_TRY
   {
      XCPT_TRY_START
      {

	getStatus(perlObject, status);

	if (status == NULL)
	{
		self = (HV *) SvRV(perlObject);

		// store the result pointer as an object property
		pointer = (IV) new Status(result);
		stPointer = newSVuv(pointer);
		hv_store(self, "status", 6, stPointer, 0);
	}
	else
	{
		(*status) = result;
	}

      }XCPT_TRY_END
   }
   GMSEC_XCPT_CATCH
}

