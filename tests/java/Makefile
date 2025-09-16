# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_API_HOME = ../..

JAVA_CLASS_DIR = classes

SOURCE	= src/*.java 

JDK_OPTS = -Xlint:unchecked -Xlint:deprecation --release 8


all: $(JAVA_CLASS_DIR) $(SOURCE)
	'$(JDK_HOME)/bin/javac' $(JDK_OPTS) -cp '$(GMSEC_API_HOME)/bin/gmsecapi.jar' -d $(JAVA_CLASS_DIR) $(SOURCE)


$(JAVA_CLASS_DIR):
	mkdir -p $@


clean:
	$(RM) -r $(JAVA_CLASS_DIR)
