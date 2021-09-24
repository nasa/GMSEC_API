# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








# GMSEC Examples Makefile
# Generates the example executables

GMSEC_HOME=..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


all: 
	cd ./cpp && $(MAKE)
	cd ./c && $(MAKE)

clean:
	cd ./cpp && $(MAKE) clean
	cd ./c && $(MAKE) clean

