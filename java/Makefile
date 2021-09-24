# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GMSEC_HOME = ..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


TARGET    = libgmsec_jni.$(JSHLIB_EXT)

CXXFLAGS += $(JAVA_INCS)
LDFLAGS  += $(STL_PORT)

# Java commands
#
JAVAC     = $(JAVA_BIN)/javac
JAR       = $(JAVA_BIN)/jar
RMIC      = $(JAVA_BIN)/rmic
JAVA      = $(JAVA_BIN)/java
JAVADOC   = $(JAVA_BIN)/javadoc

JAVA_OPTS += -Xlint:unchecked


# Java stuff
#
JNI_SOURCES_3x  = gov/nasa/gsfc/gmsecapi/jni/*.java

JAVA_SOURCES_3x = gov/nasa/gsfc/gmsecapi/*.java \
                  gov/nasa/gsfc/gmsecapi/internal/*.java \
                  gov/nasa/gsfc/gmsecapi/mist/*.java \
                  gov/nasa/gsfc/gmsecapi/util/*.java \
                  $(JNI_SOURCES_3x)

JNI_SOURCES_4x  = gov/nasa/gsfc/gmsec/api/jni/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/field/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/mist/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/mist/message/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/util/*.java

JAVA_SOURCES_4x = gov/nasa/gsfc/gmsec/api/*.java \
                  gov/nasa/gsfc/gmsec/api/field/*.java \
                  gov/nasa/gsfc/gmsec/api/mist/*.java \
                  gov/nasa/gsfc/gmsec/api/mist/message/*.java \
                  gov/nasa/gsfc/gmsec/api/util/*.java \
                  $(JNI_SOURCES_4x)

JAVADOC_3x      = gov.nasa.gsfc.gmsecapi \
                  gov.nasa.gsfc.gmsecapi.mist

JAVADOC_4x      = gov.nasa.gsfc.gmsec.api \
                  gov.nasa.gsfc.gmsec.api.field \
                  gov.nasa.gsfc.gmsec.api.mist  \
                  gov.nasa.gsfc.gmsec.api.mist.message \
                  gov.nasa.gsfc.gmsec.api.util


JAVA_OUTPUT_DIR = classes
JAVA_CLASS_PATH = $(JAVA_OUTPUT_DIR)
JAVADOC_DIR     = docs


# C++ stuff
#
SRCDIR_3x  = ./src

OBJECTS_3x = \
             $(SRCDIR_3x)/gmsecJNI_Cache.o \
             $(SRCDIR_3x)/gmsecJNI_Jenv.o \
             $(SRCDIR_3x)/gmsecJNI_Callbacks.o \
             $(SRCDIR_3x)/gmsecJNI_Config.o \
             $(SRCDIR_3x)/gmsecJNI_ConfigFile.o \
             $(SRCDIR_3x)/gmsecJNI_Connection.o \
             $(SRCDIR_3x)/gmsecJNI_ConnectionFactory.o \
             $(SRCDIR_3x)/gmsecJNI_ConnectionManager.o \
             $(SRCDIR_3x)/gmsecJNI_Field.o \
             $(SRCDIR_3x)/gmsecJNI_Init.o \
             $(SRCDIR_3x)/gmsecJNI_Log.o \
             $(SRCDIR_3x)/gmsecJNI_Message.o \
             $(SRCDIR_3x)/gmsecJNI_Status.o


SRCDIR_4x  = ./src_4x

OBJECTS_4x = \
             $(SRCDIR_4x)/gmsecJNI_Cache.o \
             $(SRCDIR_4x)/gmsecJNI_Jenv.o \
             $(SRCDIR_4x)/gmsecJNI_BinaryField.o \
             $(SRCDIR_4x)/gmsecJNI_BooleanField.o \
             $(SRCDIR_4x)/gmsecJNI_Callback.o \
             $(SRCDIR_4x)/gmsecJNI_CharField.o \
             $(SRCDIR_4x)/gmsecJNI_Config.o \
             $(SRCDIR_4x)/gmsecJNI_ConfigFile.o \
             $(SRCDIR_4x)/gmsecJNI_ConfigFileIterator.o \
             $(SRCDIR_4x)/gmsecJNI_Connection.o \
             $(SRCDIR_4x)/gmsecJNI_ConnectionManager.o \
             $(SRCDIR_4x)/gmsecJNI_ConnectionManagerCallback.o \
             $(SRCDIR_4x)/gmsecJNI_ConnectionManagerEventCallback.o \
             $(SRCDIR_4x)/gmsecJNI_ConnectionManagerReplyCallback.o \
             $(SRCDIR_4x)/gmsecJNI_CustomSpecification.o \
             $(SRCDIR_4x)/gmsecJNI_EventCallback.o \
             $(SRCDIR_4x)/gmsecJNI_F32Field.o \
             $(SRCDIR_4x)/gmsecJNI_F64Field.o \
             $(SRCDIR_4x)/gmsecJNI_Field.o \
             $(SRCDIR_4x)/gmsecJNI_I16Field.o \
             $(SRCDIR_4x)/gmsecJNI_I32Field.o \
             $(SRCDIR_4x)/gmsecJNI_I64Field.o \
             $(SRCDIR_4x)/gmsecJNI_I8Field.o \
             $(SRCDIR_4x)/gmsecJNI_HeartbeatGenerator.o \
             $(SRCDIR_4x)/gmsecJNI_Init.o \
             $(SRCDIR_4x)/gmsecJNI_Log.o \
             $(SRCDIR_4x)/gmsecJNI_LogHandler.o \
             $(SRCDIR_4x)/gmsecJNI_Message.o \
             $(SRCDIR_4x)/gmsecJNI_MessageFieldIterator.o \
             $(SRCDIR_4x)/gmsecJNI_MessageValidator.o \
             $(SRCDIR_4x)/gmsecJNI_Status.o \
             $(SRCDIR_4x)/gmsecJNI_StringField.o \
             $(SRCDIR_4x)/gmsecJNI_U16Field.o \
             $(SRCDIR_4x)/gmsecJNI_U32Field.o \
             $(SRCDIR_4x)/gmsecJNI_U64Field.o \
             $(SRCDIR_4x)/gmsecJNI_U8Field.o \
             $(SRCDIR_4x)/gmsecJNI_Device.o \
             $(SRCDIR_4x)/gmsecJNI_DeviceIterator.o \
             $(SRCDIR_4x)/gmsecJNI_DeviceMessage.o \
             $(SRCDIR_4x)/gmsecJNI_DeviceParam.o \
             $(SRCDIR_4x)/gmsecJNI_MistMessage.o \
             $(SRCDIR_4x)/gmsecJNI_Mnemonic.o \
             $(SRCDIR_4x)/gmsecJNI_MnemonicIterator.o \
             $(SRCDIR_4x)/gmsecJNI_MnemonicMessage.o \
             $(SRCDIR_4x)/gmsecJNI_MnemonicSample.o \
             $(SRCDIR_4x)/gmsecJNI_ProductFile.o \
             $(SRCDIR_4x)/gmsecJNI_ProductFileIterator.o \
             $(SRCDIR_4x)/gmsecJNI_ProductFileMessage.o \
             $(SRCDIR_4x)/gmsecJNI_ReplyCallback.o \
             $(SRCDIR_4x)/gmsecJNI_ServiceParam.o \
             $(SRCDIR_4x)/gmsecJNI_SchemaIDIterator.o \
             $(SRCDIR_4x)/gmsecJNI_Specification.o \
             $(SRCDIR_4x)/gmsecJNI_TimeSpec.o \
             $(SRCDIR_4x)/gmsecJNI_TimeUtil.o \




.PHONY: all message version clean library


# targets
all: check-env message version classes javac_3x javac_4x jar library gen-doc


check-env:
ifndef JDK_HOME
	$(error JDK_HOME is not defined; GMSEC API Java binding will not be built)
endif


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Java binding of the GMSEC API"
	@echo "#"
	@echo "###########################################################"


version:
	-@ echo -n "Using JDK version: "; $(JAVAC) -version
	-@ echo


classes:
	mkdir -p $(JAVA_OUTPUT_DIR)


javac_3x: $(JAVA_SOURCES_3x)
	$(JAVAC) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $(JAVA_OPTS) $^
	$(JAVAC) -h $(SRCDIR_3x) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $(JAVA_OPTS) $(JNI_SOURCES_3x)


javac_4x: $(JAVA_SOURCES_4x)
	$(JAVAC) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $(JAVA_OPTS) $^
	$(JAVAC) -h $(SRCDIR_4x) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $(JAVA_OPTS) $(JNI_SOURCES_4x)


jar:
	$(JAR) cf $(BINDIR)/gmsecapi.jar -C $(JAVA_OUTPUT_DIR) gov


clean:
	$(RM) $(SRCDIR_3x)/gov_nasa_gsfc_gmsecapi_jni_gmsecJNI.h
	$(RM) $(SRCDIR_4x)/gov_nasa_gsfc_gmsec_api_jni_gmsecJNI.h
	$(RM) -r $(JAVA_OUTPUT_DIR)/gov
	$(RM) $(OBJECTS_3x) $(OBJECTS_4x)
	$(RM) $(SRCDIR_3x)/*.gcno $(SRCDIR_3x)/*.gcda $(SRCDIR_4x)/*.gcno $(SRCDIR_4x)/*.gcda
	$(RM) $(BINDIR)/$(TARGET)
	$(RM) -r $(JAVADOC_DIR)


library: $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS_3x) $(OBJECTS_4x)
	$(DLINK) $^ $(LDFLAGS) $(JLINK_FLAGS) -o $@


$(SRCDIR_3x)/%.o: $(SRCDIR_3x)/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@


$(SRCDIR_4x)/%.o: $(SRCDIR_4x)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


gen-doc:
	-$(JAVADOC) -public -quiet -d $(JAVADOC_DIR) --no-module-directories -sourcepath . $(JAVADOC_4x)
	$(JAR) cf $(BINDIR)/gmsecapi-javadoc.jar $(JAVADOC_DIR)
	$(JAR) uf $(BINDIR)/gmsecapi.jar $(JAVADOC_DIR)
