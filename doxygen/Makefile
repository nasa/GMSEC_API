# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

DOX_CFG_FILES       = gmsec_c_3x_dox       \
                      gmsec_c_4x_dox       \
                      gmsec_cpp_3x_dox     \
                      gmsec_cpp_4x_dox     \
                      gmsec_java_3x_dox    \
                      gmsec_java_4x_dox    \
                      gmsec_python3_4x_dox \
                      gmsec_csharp_4x_dox  \


GMSEC_HOME          = ..
DOCS                = $(GMSEC_HOME)/../GMSEC_API_DOCS/docs

PERL_3X             = ../perl/gmsec
PERL_3X_LIB         = $(PERL_3X)/lib
PERL_3X_DOCS        = manual/perl_3x

PERL_4X             = ../perl/gmsec4
PERL_4X_INTERFACES  = $(PERL_4X)/interfaces
PERL_4X_DOCS        = manual/perl_4x

PERL_3X_API_CLASSES = Callback Config ConfigFile Connection ConnectionFactory Field Message Status Util

PERL_4X_API_CLASSES = Config ConfigFile ConfigFileIterator Connection Errors Exception \
                      HeartbeatGenerator Message MessageFieldIterator Status SubscriptionInfo \
                      field/Field field/BinaryField field/BooleanField field/CharField \
                      field/F32Field field/F64Field field/I16Field field/I32Field field/I64Field \
                      field/I8Field field/StringField field/U16Field field/U32Field \
                      field/U64Field field/U8Field \
                      util/Condition util/Log util/Mutex util/TimeUtil \
                      mist/ConnectionManager mist/Device mist/DeviceIterator mist/DeviceParam \
                      mist/FieldSpecification mist/MessageSpecification mist/mist_defs \
                      mist/Mnemonic mist/MnemonicIterator mist/MnemonicSample mist/ProductFile \
                      mist/ProductFileIterator mist/SchemaIDIterator mist/ServiceParam \
                      mist/Specification mist/SubscriptionInfo \
                      mist/message/DeviceMessage mist/message/MistMessage \
                      mist/message/MnemonicMessage mist/message/ProductFileMessage


all: ruby_doc py3_doc doxygen perl_4x_docs perl_3x_docs


ruby_doc:
	@ ./gen_ruby_doc.sh


py3_doc:
	@ ./gen_py3_doc.sh


doxygen:
	for cfgfile in $(DOX_CFG_FILES); do \
		doxygen $$cfgfile.cfg 1> $$cfgfile.out 2> $$cfgfile.err; \
		sed -i -r "s/Release [0-9]+.[0-9]+(.[0-9]+)?/Release $(shell cat ../version.inf)/g" $$cfgfile.html; \
	done


perl_3x_docs:
	mkdir -p $(PERL_3X_DOCS)/GMSECAPI
	pod2html --infile=$(PERL_3X_LIB)/GMSECAPI.pm --outfile=$(PERL_3X_DOCS)/index.html --htmlroot=..
	cp -p $(PERL_3X_DOCS)/index.html $(PERL_3X_DOCS)/GMSECAPI.html
	for class in $(PERL_3X_API_CLASSES); do \
		pod2html --infile=$(PERL_3X_LIB)/GMSECAPI/$$class.pm --outfile=$(PERL_3X_DOCS)/GMSECAPI/$$class.html --htmlroot=../.. --podroot=$(PERL_3X_LIB) --podpath=.. ; \
	done

perl_4x_docs:
	mkdir -p $(PERL_4X_DOCS)
	mkdir -p $(PERL_4X_DOCS)/util
	mkdir -p $(PERL_4X_DOCS)/field
	mkdir -p $(PERL_4X_DOCS)/mist/message
	cp gmsec_logo.png $(PERL_4X_DOCS)
	pod2html --infile=$(PERL_4X_INTERFACES)/libgmsec_perl.i --outfile=$(PERL_4X_DOCS)/index.html --htmlroot=. --title="GMSEC Perl API"
	for class in $(PERL_4X_API_CLASSES); do \
		pod2html --infile=$(PERL_4X_INTERFACES)/$$class.i --outfile=$(PERL_4X_DOCS)/$$class.html --htmlroot=. --title=$$class ; \
	done


install:
	mkdir -p $(DOCS)/manual
	cp -r $(GMSEC_HOME)/doxygen/manual/* $(DOCS)/manual
	cp  $(GMSEC_HOME)/doxygen/gmsec_logo.png $(DOCS)


clean:
	$(RM) -r manual
	$(RM) *log.txt *.err *.out
	$(RM) pod2htmd.tmp pod2htmi.tmp
	$(RM) libgmsec_python3.html
