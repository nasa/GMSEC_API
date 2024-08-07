rem Copyright 2007-2024 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

@echo off
rem There does not appear to be a way within SWIG to add code to the C++ wrapper code generated by SWIG for the SWIG directors
rem Due to the fact that SWIG does not seem to add the CALL_TYPE macro to the director classes it generates, we must do so manually
perl -i.bak -pe "s/onMessage/CALL_TYPE onMessage/g" interfaces\libgmsec_perl_wrap.h
perl -i.bak -pe "s/onReply/CALL_TYPE onReply/g" interfaces\libgmsec_perl_wrap.h
perl -i.bak -pe "s/validateMessage/CALL_TYPE validateMessage/g" interfaces\libgmsec_perl_wrap.h
perl -i.bak -pe "s/onEvent/ CALL_TYPE onEvent/g" interfaces\libgmsec_perl_wrap.h
@echo on
