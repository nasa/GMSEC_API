 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


ifndef GMSEC_PLATFORM

%:
	@ echo "Error: GMSEC_PLATFORM is not defined."
	@ echo
	@ echo "Please refer to the 'INSTALL.txt' file for reference"
	@ echo "on how to setup your build environment."
	@ echo
	@ false

else

GMSEC_HOME      = .
RELEASE_DIR     = ../release
GMSEC_API_DEST := $(RELEASE_DIR)/GMSEC_API

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

.PHONY: check_support build library csharp_api csharp_api_4x perl_api perl_api_3x perl_api_4x python3_api python3_api_4x ruby_api env_val api_docs mw_wrapper clean install tools


default: check_support build library mw_wrappers csharp_api java_api perl_api python3_api ruby_api env_val tools patch-mac


check_support:
	@ if [ -d $$SUPPORT ]; then \
		echo "Found $$SUPPORT directory..." ; \
	fi
	mkdir -p $(BINDIR)


build:
ifneq (, $(shell which perl))
	CXX="$(CXX)" perl buildscript.pl
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  perl not found"
	@echo "#  Exiting build of the GMSEC API"
	@echo "#"
	@echo "###########################################################"
endif


library:
	$(MAKE) -C framework


csharp_api:
# Note: C# binding for API 4.x is currently only supported on Linux and macOS distros.
ifeq ($(findstring linux,$(GMSEC_PLATFORM)), linux)
	$(MAKE) csharp_api_4x
endif
ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
	$(MAKE) csharp_api_4x
endif


csharp_api_4x:
ifdef SWIG_HOME
ifneq (, $(shell which mcs))
	$(MAKE) -C csharp/gmsec4
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  mcs (from Mono Development package) not found"
	@echo "#  Skipping build of C# binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of C# binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif


java_api:
ifdef JDK_HOME
	$(MAKE) -C java
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  JDK_HOME is not defined"
	@echo "#  Skipping build of Java binding of the GMSEC API"
	@echo "#"
	@echo "###########################################################"
endif


perl_api: perl_api_3x
# Note: Perl binding for API 4.x is currently only supported on Linux and macOS distros.
ifeq ($(findstring linux,$(GMSEC_PLATFORM)), linux)
	$(MAKE) perl_api_4x
endif
ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
	$(MAKE) perl_api_4x
endif


perl_api_3x:
ifdef PERL5_LIB
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Perl 3.x binding of the GMSEC API"
	@echo "#"
	@echo "###########################################################"
	cd ./perl/gmsec; \
		PERL_CC=$(PERL_CC) perl -Iextra Makefile.PL PREFIX=../../bin ; \
		$(MAKE) ; $(MAKE) install;
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PERL5_LIB is not defined."
	@echo "#  Skipping build of Perl 3.x binding of the GMSEC API"
	@echo "#"
	@echo "###########################################################"
endif


perl_api_4x:
ifdef SWIG_HOME
ifdef PERL5_LIB
ifdef PERL5_INC
	$(MAKE) -C perl/gmsec4
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PERL5_INC is not defined, or Perl tools not available"
	@echo "#  Skipping build of Perl binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PERL5_LIB is not defined, or Perl tools not available"
	@echo "#  Skipping build of Perl binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of Perl binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif


python3_api:
# Note: Python3 binding for API 4.x is currently only supported on Linux and macOS distros.
ifeq ($(findstring linux,$(GMSEC_PLATFORM)), linux)
	$(MAKE) python3_api_4x
endif
ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
	$(MAKE) python3_api_4x
endif


python3_api_4x:
ifdef SWIG_HOME
ifdef PYTHON3_HOME
	$(MAKE) -C python3/gmsec4
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PYTHON3_HOME is not defined"
	@echo "#  Skipping build of Python3 binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of Python3 binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif


ruby_api:
# Note: Ruby binding for API 4.x is currently only supported on Linux and macOS distros.
ifeq ($(findstring linux,$(GMSEC_PLATFORM)), linux)
	$(MAKE) ruby_api_4x
endif
ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
	$(MAKE) ruby_api_4x
endif


ruby_api_4x:
ifdef SWIG_HOME
	@cd ruby/gmsec4 && $(MAKE) -f MakeRuby
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of C# binding of the GMSEC API 4.x"
	@echo "#"
	@echo "###########################################################"
endif


env_val:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Setting up GMSEC API Validator"
	@echo "#"
	@echo "###########################################################"
	mkdir -p ./bin/validator
	cp validator/env_validator.sh ./bin/validator
	cp validator/perl_ver.pl ./bin/validator
	cp validator/*.env ./bin/validator


tools:
	$(MAKE) -C tools


patch-mac:
	@$(GMSEC_HOME)/fix-loader-path.sh


api_docs:
	$(MAKE) -C doxygen


mw_wrappers:
	$(MAKE) -C wrapper


all_examples:
	$(MAKE) -C examples INTERNAL=1


clean: clean-gcov
ifneq (, $(shell which perl))
	CXX="$(CXX)" perl buildscript.pl $@
endif
	$(RM) -r $(BINDIR)/amqp
	$(RM) -r $(BINDIR)/lib
	$(RM) -r $(BINDIR)/validator
	@ for dir in framework examples wrapper java doxygen tools; do \
		$(MAKE) -C $$dir $@ ; \
	done
	- @ [ -f perl/gmsec/Makefile ] && $(MAKE) -C perl/gmsec veryclean
	- @ $(MAKE) -C csharp/gmsec4 clean
	- @ $(MAKE) -C perl/gmsec4 clean
	- @ $(MAKE) -C python3/gmsec4 clean
	- @ $(MAKE) -C ruby/gmsec4 clean
	$(RM) -r $(BINDIR)/*
	$(RM) c.ver os.ver java.ver


clean-gcov:
	@ for dir in framework examples wrapper; do \
		echo $(RM) -r $$dir/*.gcno $$dir/*.gcda ; \
	done


install:
	@ echo
	@ echo
	@ echo "###########################################################"
	@ echo "#"
	@ echo "# Installing the GMSEC API"
	@ echo "#"
	@ echo "###########################################################"
	mkdir -p $(RELEASE_DIR)
	$(RM) -r $(GMSEC_API_DEST) && mkdir $(GMSEC_API_DEST)
	find . -type d -name '\?' | xargs -n 20 rm -rf   # remove oracle_jre_usage info (if any)
	cp -a README.txt bin config templates examples framework/include $(GMSEC_API_DEST)
	$(RM) -r $(GMSEC_API_DEST)/include/gmsec/internal
	$(RM) -r $(GMSEC_API_DEST)/include/gmsec4/internal

install_docs:
	$(MAKE) install -C doxygen

endif

