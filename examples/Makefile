# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# GMSEC Examples Makefile
# Generates the example executables

GMSEC_API_HOME=..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)


all: 
	- $(MAKE) -C c
	- $(MAKE) -C cpp
	- $(MAKE) -C csharp
	- $(MAKE) -C java

clean:
	- $(MAKE) -C c $@
	- $(MAKE) -C cpp $@
	- $(MAKE) -C csharp $@
	- $(MAKE) -C java $@
