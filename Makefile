 
# Copyright 2007-2017 United States Government as represented by the
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

RELEASE    = ../release
GMSEC_HOME = .

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

.PHONY: check_support build library perl_api env_val api_docs mw_wrapper all_examples clean install tools


default: check_support build library mw_wrappers all_examples java_api perl_api env_val tools


check_support:
	@ if [ -d ../SUPPORT ]; then \
		echo "Found SUPPORT directory..." ; \
	fi


build:
	CXX=$(CXX) perl buildscript.pl	


library:
	$(MAKE) -C framework


java_api:
	$(MAKE) -C java


perl_api:
	cd ./perl/gmsec; \
		PERL_CC=$(PERL_CC) perl -Iextra Makefile.PL PREFIX=../../bin ; \
		$(MAKE) ; $(MAKE) install
# Note: Perl binding for API 4.X is currently only supported on Linux and Mac platforms.
ifeq ($(findstring linux,$(GMSEC_PLATFORM)), linux)
	$(MAKE) -C perl/gmsec4
endif
ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
	$(MAKE) -C perl/gmsec4
endif


env_val:
	mkdir -p ./bin/validator
	cp validator/env_validator.sh ./bin/validator
	cp validator/perl_ver.pl ./bin/validator
	cp validator/*.env ./bin/validator


tools:
	$(MAKE) -C tools


api_docs:
	$(MAKE) -C doxygen


mw_wrappers:
	$(MAKE) -C wrapper


all_examples:
	$(MAKE) -C examples INTERNAL=1


clean: clean-gcov
	$(RM) -r bin/amqp
	$(RM) -r bin/lib
	$(RM) -r bin/validator
	$(RM) bin/*
	@ for dir in framework examples wrapper java doxygen tools; do \
		$(MAKE) -C $$dir $@ ; \
	done
	- @ [ -f perl/gmsec/Makefile ] && $(MAKE) -C perl/gmsec veryclean
	- @ $(MAKE) -C perl/gmsec4 clean
	$(RM) c.ver os.ver java.ver

clean-gcov:
	@ for dir in framework examples wrapper; do \
		echo $(RM) -r $$dir/*.gcno $$dir/*.gcda ; \
	done


install:
	if [ -d $(RELEASE) ] ; then rm -rf $(RELEASE) ; fi
	mkdir -p $(RELEASE)/GMSEC_API
	tar cvf - *.txt bin config templates examples -C framework include \
		--exclude='.svn' --exclude=vendor \
		--exclude=secure --exclude=internal \
		| tar xvf - -C $(RELEASE)/GMSEC_API
	(cd $(RELEASE); tar zcvf GMSEC_API.tgz GMSEC_API)

install_docs:
	$(MAKE) install -C doxygen

endif

