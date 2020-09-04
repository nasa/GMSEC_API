# Copyright 2007-2016 United States Government as represented by the
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
JAVAH     = $(JAVA_BIN)/javah
RMIC      = $(JAVA_BIN)/rmic
JAVA      = $(JAVA_BIN)/java
JAVADOC   = $(JAVA_BIN)/javadoc


# Java stuff
#
JAVA_SOURCES_3x = gov/nasa/gsfc/gmsecapi/*.java \
                  gov/nasa/gsfc/gmsecapi/internal/*.java \
                  gov/nasa/gsfc/gmsecapi/mist/*.java \
                  gov/nasa/gsfc/gmsecapi/jni/*.java \
                  gov/nasa/gsfc/gmsecapi/util/*.java

JAVA_SOURCES_4x = gov/nasa/gsfc/gmsec/api/*.java \
                  gov/nasa/gsfc/gmsec/api/field/*.java \
                  gov/nasa/gsfc/gmsec/api/mist/*.java \
                  gov/nasa/gsfc/gmsec/api/mist/message/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/field/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/mist/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/mist/message/*.java \
                  gov/nasa/gsfc/gmsec/api/jni/util/*.java \
                  gov/nasa/gsfc/gmsec/api/util/*.java


JAVA_OUTPUT_DIR = classes
JAVA_CLASS_PATH = $(JAVA_OUTPUT_DIR)


# C++ stuff
#
OBJDIR_3x  = ./src

OBJECTS_3x = \
             $(OBJDIR_3x)/gmsecJNI_Cache.o \
             $(OBJDIR_3x)/gmsecJNI_Jenv.o \
             $(OBJDIR_3x)/gmsecJNI_Callbacks.o \
             $(OBJDIR_3x)/gmsecJNI_Config.o \
             $(OBJDIR_3x)/gmsecJNI_ConfigFile.o \
             $(OBJDIR_3x)/gmsecJNI_Connection.o \
             $(OBJDIR_3x)/gmsecJNI_ConnectionFactory.o \
             $(OBJDIR_3x)/gmsecJNI_ConnectionManager.o \
             $(OBJDIR_3x)/gmsecJNI_Field.o \
             $(OBJDIR_3x)/gmsecJNI_Init.o \
             $(OBJDIR_3x)/gmsecJNI_Log.o \
             $(OBJDIR_3x)/gmsecJNI_Message.o \
             $(OBJDIR_3x)/gmsecJNI_Status.o


OBJDIR_4x  = ./src_4x

OBJECTS_4x = \
             $(OBJDIR_4x)/gmsecJNI_Cache.o \
             $(OBJDIR_4x)/gmsecJNI_Jenv.o \
             $(OBJDIR_4x)/gmsecJNI_BinaryField.o \
             $(OBJDIR_4x)/gmsecJNI_BooleanField.o \
             $(OBJDIR_4x)/gmsecJNI_Callback.o \
             $(OBJDIR_4x)/gmsecJNI_CharField.o \
             $(OBJDIR_4x)/gmsecJNI_Config.o \
             $(OBJDIR_4x)/gmsecJNI_ConfigFile.o \
             $(OBJDIR_4x)/gmsecJNI_ConfigFileIterator.o \
             $(OBJDIR_4x)/gmsecJNI_Connection.o \
             $(OBJDIR_4x)/gmsecJNI_ConnectionManager.o \
             $(OBJDIR_4x)/gmsecJNI_ConnectionManagerCallback.o \
             $(OBJDIR_4x)/gmsecJNI_ConnectionManagerEventCallback.o \
             $(OBJDIR_4x)/gmsecJNI_ConnectionManagerReplyCallback.o \
             $(OBJDIR_4x)/gmsecJNI_EventCallback.o \
             $(OBJDIR_4x)/gmsecJNI_F32Field.o \
             $(OBJDIR_4x)/gmsecJNI_F64Field.o \
             $(OBJDIR_4x)/gmsecJNI_Field.o \
             $(OBJDIR_4x)/gmsecJNI_I16Field.o \
             $(OBJDIR_4x)/gmsecJNI_I32Field.o \
             $(OBJDIR_4x)/gmsecJNI_I64Field.o \
             $(OBJDIR_4x)/gmsecJNI_I8Field.o \
             $(OBJDIR_4x)/gmsecJNI_Init.o \
             $(OBJDIR_4x)/gmsecJNI_Log.o \
             $(OBJDIR_4x)/gmsecJNI_LogHandler.o \
             $(OBJDIR_4x)/gmsecJNI_Message.o \
             $(OBJDIR_4x)/gmsecJNI_MessageFieldIterator.o \
             $(OBJDIR_4x)/gmsecJNI_Status.o \
             $(OBJDIR_4x)/gmsecJNI_StringField.o \
             $(OBJDIR_4x)/gmsecJNI_U16Field.o \
             $(OBJDIR_4x)/gmsecJNI_U32Field.o \
             $(OBJDIR_4x)/gmsecJNI_U64Field.o \
             $(OBJDIR_4x)/gmsecJNI_U8Field.o \
             $(OBJDIR_4x)/gmsecJNI_Device.o \
             $(OBJDIR_4x)/gmsecJNI_DeviceIterator.o \
             $(OBJDIR_4x)/gmsecJNI_DeviceMessage.o \
             $(OBJDIR_4x)/gmsecJNI_DeviceParam.o \
             $(OBJDIR_4x)/gmsecJNI_MistMessage.o \
             $(OBJDIR_4x)/gmsecJNI_Mnemonic.o \
             $(OBJDIR_4x)/gmsecJNI_MnemonicIterator.o \
             $(OBJDIR_4x)/gmsecJNI_MnemonicMessage.o \
             $(OBJDIR_4x)/gmsecJNI_MnemonicSample.o \
             $(OBJDIR_4x)/gmsecJNI_ProductFile.o \
             $(OBJDIR_4x)/gmsecJNI_ProductFileIterator.o \
             $(OBJDIR_4x)/gmsecJNI_ProductFileMessage.o \
             $(OBJDIR_4x)/gmsecJNI_ReplyCallback.o \
             $(OBJDIR_4x)/gmsecJNI_ServiceParam.o \
             $(OBJDIR_4x)/gmsecJNI_SchemaIDIterator.o \
             $(OBJDIR_4x)/gmsecJNI_Specification.o \
             $(OBJDIR_4x)/gmsecJNI_TimeSpec.o \
             $(OBJDIR_4x)/gmsecJNI_TimeUtil.o \




.PHONY: all version clean library


# targets
all: version classes javac_3x javac_4x jar jni library


version:
	-@ $(JAVAC) -version


classes:
	mkdir -p $(JAVA_OUTPUT_DIR)


javac_3x: $(JAVA_SOURCES_3x)
	$(JAVAC) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $^


javac_4x: $(JAVA_SOURCES_4x)
	$(JAVAC) -d $(JAVA_OUTPUT_DIR) -classpath $(JAVA_CLASS_PATH) $^


jar:
	$(JAR) cvf ../bin/gmsecapi.jar -C $(JAVA_OUTPUT_DIR) gov


clean:
	$(RM) src/gmsecJNI.h src_4x/gmsecJNI.h
	$(RM) -r $(JAVA_OUTPUT_DIR)/gov
	$(RM) $(OBJECTS_3x) $(OBJECTS_4x)
	$(RM) $(OBJDIR_3x)/*.gcno $(OBJDIR_3x)/*.gcda $(OBJDIR_4x)/*.gcno $(OBJDIR_4x)/*.gcda
	$(RM) $(BINDIR)/$(TARGET)
	$(RM) -r docs


jni:
	$(RM) $(OBJDIR_3x)/gmsecJNI.h
	$(JAVAH) -classpath $(JAVA_CLASS_PATH) -o $(OBJDIR_3x)/gmsecJNI.h gov.nasa.gsfc.gmsecapi.jni.gmsecJNI
	$(RM) $(OBJDIR_4x)/gmsecJNI.h
	$(JAVAH) -classpath $(JAVA_CLASS_PATH) -o $(OBJDIR_4x)/gmsecJNI.h gov.nasa.gsfc.gmsec.api.jni.gmsecJNI


library: $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS_3x) $(OBJECTS_4x)
	$(DLINK) $^ $(LDFLAGS) $(JLINK_FLAGS) -o $@


$(OBJDIR_3x)/%.o: $(OBJDIR_3x)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


$(OBJDIR_4x)/%.o: $(OBJDIR_4x)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


doc:
	$(JAVADOC) -d docs -classpath $(JAVA_CLASS_PATH) $(JAVA_SOURCES_3x) $(JAVA_SOURCES_4x)
