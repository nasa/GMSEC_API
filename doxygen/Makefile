# Copyright 2007-2015 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

DOX_CFG_FILES = gmsec_c_dox gmsec_c_4x_dox gmsec_cpp_dox gmsec_cpp_4x_dox gmsec_java_dox gmsec_java_4x_dox

GMSEC_HOME    = ..
DOCS          = $(GMSEC_HOME)/../GMSEC_API_DOCS/docs

GM_PERL       = ../perl
GM_PERL_LIB   = $(GM_PERL)/lib
GM_PERL_DOCS  = manual/perl

GM_PERL_API_CLASSES = Callback Config ConfigFile Connection ConnectionFactory Field Message Status Util


all: doxygen perldocs


doxygen:
	for cfgfile in $(DOX_CFG_FILES); do \
		doxygen $$cfgfile.cfg 1> $$cfgfile.out 2> $$cfgfile.err; \
		sed -i -r "s/Release [0-9]+.[0-9]+(.[0-9]+)?/Release $(shell cat ../version.inf)/g" $$cfgfile.html; \
	done


perldocs:
	mkdir -p $(GM_PERL_DOCS)/GMSECAPI
	pod2html --infile=$(GM_PERL_LIB)/GMSECAPI.pm --outfile=$(GM_PERL_DOCS)/GMSECAPI.html --htmlroot=..
	for class in $(GM_PERL_API_CLASSES); do \
		pod2html --infile=$(GM_PERL_LIB)/GMSECAPI/$$class.pm --outfile=$(GM_PERL_DOCS)/GMSECAPI/$$class.html --htmlroot=../.. --podroot=$(GM_PERL_LIB) --podpath=.. ; \
	done


install:
	if [ ! -d $(DOCS) ] ; then mkdir $(DOCS) ; fi
	cp -r $(GMSEC_HOME)/doxygen/manual/* $(DOCS)


clean:
	$(RM) -r manual
	$(RM) log.txt *.err *.out
	$(RM) pod2htmd.tmp pod2htmi.tmp
