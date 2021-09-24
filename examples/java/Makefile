# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


JAVAC     = $(JDK_HOME)/bin/javac
CLASSPATH =../../bin/gmsecapi.jar:.


.PHONY: all clean


all: checkJDK
	$(JAVAC) -cp $(CLASSPATH) -Xlint:deprecation *.java


clean:
	$(RM) *.class


checkJDK:
	@ if [ -z $(JDK_HOME) ]; then \
		echo "JDK_HOME is not defined"; \
		exit 1; \
	fi
