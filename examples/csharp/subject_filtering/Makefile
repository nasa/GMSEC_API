# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


default all: make_pub make_sub

make_pub:
	$(MAKE) -f Makefile.publish

make_sub:
	$(MAKE) -f Makefile.subscribe

clean:
	$(MAKE) -f Makefile.publish $@
	$(MAKE) -f Makefile.subscribe $@
