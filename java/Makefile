# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GMSEC_API_HOME = ..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)


TARGET    = libgmsec_jni.$(JSHLIB_EXT)

CXXFLAGS += $(JAVA_INCS)
LDFLAGS  += $(STL_PORT) $(JAVA_LIBS)

# Java commands
#
JAVAC     = $(JAVA_BIN)/javac
JAR       = $(JAVA_BIN)/jar
RMIC      = $(JAVA_BIN)/rmic
JAVA      = $(JAVA_BIN)/java
JAVADOC   = $(JAVA_BIN)/javadoc

JAR_FILE         = $(BINDIR)/gmsecapi.jar
JAVA_DOC_FILE    = $(BINDIR)/gmsecapi-javadoc.jar

JAVA_OPTS       += -Xlint:unchecked
JAVA_OUTPUT_DIR  = classes
JAVA_CLASS_PATH  = $(JAVA_OUTPUT_DIR)
JAVADOC_DIR      = docs


# Java stuff
#
JNI_SOURCES  = gov/nasa/gsfc/gmsec/api5/jni/*.java \
               gov/nasa/gsfc/gmsec/api5/jni/field/*.java \
               gov/nasa/gsfc/gmsec/api5/jni/util/*.java

JAVA_SOURCES = gov/nasa/gsfc/gmsec/api5/*.java \
               gov/nasa/gsfc/gmsec/api5/field/*.java \
               gov/nasa/gsfc/gmsec/api5/util/*.java \
                $(JNI_SOURCES)

JAVA_DOCS    = gov.nasa.gsfc.gmsec.api5 \
               gov.nasa.gsfc.gmsec.api5.field \
               gov.nasa.gsfc.gmsec.api5.util


# C++ stuff
#
SRCDIR  = ./src

OBJECTS = \
          $(SRCDIR)/gmsecJNI_Cache.o \
          $(SRCDIR)/gmsecJNI_Jenv.o \
          $(SRCDIR)/gmsecJNI_BinaryField.o \
          $(SRCDIR)/gmsecJNI_BooleanField.o \
          $(SRCDIR)/gmsecJNI_Callback.o \
          $(SRCDIR)/gmsecJNI_CharField.o \
          $(SRCDIR)/gmsecJNI_Config.o \
          $(SRCDIR)/gmsecJNI_ConfigFile.o \
          $(SRCDIR)/gmsecJNI_ConfigFileIterator.o \
          $(SRCDIR)/gmsecJNI_Connection.o \
          $(SRCDIR)/gmsecJNI_EventCallback.o \
          $(SRCDIR)/gmsecJNI_F32Field.o \
          $(SRCDIR)/gmsecJNI_F64Field.o \
          $(SRCDIR)/gmsecJNI_Field.o \
          $(SRCDIR)/gmsecJNI_I16Field.o \
          $(SRCDIR)/gmsecJNI_I32Field.o \
          $(SRCDIR)/gmsecJNI_I64Field.o \
          $(SRCDIR)/gmsecJNI_I8Field.o \
          $(SRCDIR)/gmsecJNI_HeartbeatGenerator.o \
          $(SRCDIR)/gmsecJNI_Init.o \
          $(SRCDIR)/gmsecJNI_Log.o \
          $(SRCDIR)/gmsecJNI_LogHandler.o \
          $(SRCDIR)/gmsecJNI_Message.o \
          $(SRCDIR)/gmsecJNI_MessageFactory.o \
          $(SRCDIR)/gmsecJNI_MessageFieldIterator.o \
          $(SRCDIR)/gmsecJNI_MessageValidator.o \
          $(SRCDIR)/gmsecJNI_ResourceGenerator.o \
          $(SRCDIR)/gmsecJNI_Status.o \
          $(SRCDIR)/gmsecJNI_StringField.o \
          $(SRCDIR)/gmsecJNI_U16Field.o \
          $(SRCDIR)/gmsecJNI_U32Field.o \
          $(SRCDIR)/gmsecJNI_U64Field.o \
          $(SRCDIR)/gmsecJNI_U8Field.o \
          $(SRCDIR)/gmsecJNI_ReplyCallback.o \
          $(SRCDIR)/gmsecJNI_SchemaIDIterator.o \
          $(SRCDIR)/gmsecJNI_Specification.o \
          $(SRCDIR)/gmsecJNI_TimeSpec.o \
          $(SRCDIR)/gmsecJNI_TimeUtil.o \


.PHONY: all message version clean library


# targets
all: check-env message version classes javac jar library gen-doc


check-env:
ifndef JDK_HOME
	$(error JDK_HOME is not defined; GMSEC API Java binding will not be built)
endif


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Java binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"


version:
	-@ echo -n "Using JDK version: "; $(JAVAC) -version
	-@ echo


classes:
	mkdir -p $(JAVA_OUTPUT_DIR)


javac: $(JAVA_SOURCES)
	$(JAVAC) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $(JAVA_OPTS) $^
	$(JAVAC) -h $(SRCDIR) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $(JAVA_OPTS) $(JNI_SOURCES)


jar:
	$(JAR) cf $(JAR_FILE) -C $(JAVA_OUTPUT_DIR) gov


clean:
	$(RM) $(SRCDIR)/gov_nasa_gsfc_gmsec_api5_jni_gmsecJNI.h
	$(RM) -r $(JAVA_OUTPUT_DIR)/gov
	$(RM) $(OBJECTS)
	$(RM) $(SRCDIR)/*.gcno $(SRCDIR)/*.gcda
	$(RM) $(JAR_FILE) $(JAVA_DOC_FILE)
	$(RM) -r $(JAVADOC_DIR)


library: $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $^ $(LDFLAGS) $(JLINK_FLAGS) -o $@


$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


gen-doc:
	$(JAVADOC) -public -quiet -d $(JAVADOC_DIR) --no-module-directories -sourcepath . $(JAVA_DOCS)
	$(JAR) cf $(JAVA_DOC_FILE) $(JAVADOC_DIR)
	$(JAR) uf $(JAR_FILE) $(JAVADOC_DIR)
