# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



# GMSEC unit tests Makefile
# Generates the unit test executables

ifndef GMSEC_API_HOME
done:
	@ echo "Error: GMSEC_API_HOME is not defined."
	@ echo
	@ false

else

all: 
	- $(MAKE) -C cpp/src
	- $(MAKE) -C c/src
	- $(MAKE) -C csharp/src
	- $(MAKE) -C java

clean:
	- $(MAKE) -C cpp/src $@
	- $(MAKE) -C c/src $@
	- $(MAKE) -C csharp/src $@
	- $(MAKE) -C java $@

endif
