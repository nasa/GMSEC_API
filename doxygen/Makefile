# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

DOX_CFG_FILES       = gmsec_c_dox       \
                      gmsec_cpp_dox     \
                      gmsec_java_dox    \
                      gmsec_python3_dox \
                      gmsec_csharp_dox  \


GMSEC_API_HOME      = ..
DOCS                = $(GMSEC_API_HOME)/../GMSEC_API_DOCS-5.x/docs

PERL                = ../perl/gmsec5
PERL_INTERFACES     = $(PERL)/interfaces
PERL_DOCS           = manual/perl

PERL_API_CLASSES    = Config ConfigFile ConfigFileIterator Connection Errors GmsecException \
                      HeartbeatGenerator Message MessageFactory MessageFieldIterator ResourceGenerator \
                      Specification Status SubscriptionInfo \
                      field/Field field/BinaryField field/BooleanField field/CharField \
                      field/F32Field field/F64Field field/I16Field field/I32Field field/I64Field \
                      field/I8Field field/StringField field/U16Field field/U32Field \
                      field/U64Field field/U8Field \
                      util/Condition util/Log util/Mutex util/TimeUtil \


all: doxygen perl_doc py3_doc ruby_doc


doxygen:
	for cfgfile in $(DOX_CFG_FILES); do \
		doxygen $$cfgfile.cfg 1> $$cfgfile.out 2> $$cfgfile.err; \
		sed -i -r "s/Release [0-9]+.[0-9]+(.[0-9]+)?/Release $(shell cat ../version.inf)/g" $$cfgfile.md; \
	done


perl_doc:
	mkdir -p $(PERL_DOCS)
	mkdir -p $(PERL_DOCS)/util
	mkdir -p $(PERL_DOCS)/field
	cp gmsec_logo.png $(PERL_DOCS)
	pod2html --infile=$(PERL_INTERFACES)/libgmsec_perl.i --outfile=$(PERL_DOCS)/index.html --htmlroot=. --title="GMSEC Perl API"
	for class in $(PERL_API_CLASSES); do \
		pod2html --infile=$(PERL_INTERFACES)/$$class.i --outfile=$(PERL_DOCS)/$$class.html --htmlroot=. --title=$$class ; \
	done


py3_doc:
	@ ./gen_py3_doc.sh


ruby_doc:
	@ ./gen_ruby_doc.sh


clean:
	$(RM) -r manual
	$(RM) *log.txt *.err *.out
	$(RM) pod2htmd.tmp pod2htmi.tmp
	$(RM) libgmsec_python3.html
