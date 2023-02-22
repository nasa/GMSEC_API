# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

JAVA_11_CLASS_DIR = classes11

SOURCE	= src/*.java 

JDK_OPTS = -Xlint:unchecked -Xlint:deprecation --release 8


all: java11

clean:
	$(RM) -r $(JAVA_11_CLASS_DIR)

java11: $(JAVA_11_CLASS_DIR) $(SOURCE)
	'$(JDK_HOME)/bin/javac' $(JDK_OPTS) -cp '$(GMSEC_API_HOME)/bin/gmsecapi.jar' -d $(JAVA_11_CLASS_DIR) $(SOURCE)

$(JAVA_11_CLASS_DIR):
	mkdir -p $@
