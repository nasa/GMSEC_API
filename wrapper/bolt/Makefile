# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


 
GMSEC_HOME	= ../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


TARGET		= libgmsec_bolt.$(SHLIB_EXT)

LOCAL_INCS		= -I./include

CXXFLAGS	+= $(LOCAL_INCS)

# bolt objects are written to a separate subdirectory to avoid name collision
BOBJDIR		= ./src
WOBJDIR     = ./gmsec

OBJECTS	= \
	$(WOBJDIR)/gmsec_bolt.o \
	$(WOBJDIR)/BoltConnection.o \
	$(BOBJDIR)/ByteBuffer.o \
	$(BOBJDIR)/Connection.o \
	$(BOBJDIR)/Decoder.o \
	$(BOBJDIR)/Encoder.o \
	$(BOBJDIR)/Header.o \
	$(BOBJDIR)/InputBuffer.o \
	$(BOBJDIR)/Manager.o \
	$(BOBJDIR)/Meta.o \
	$(BOBJDIR)/Options.o \
	$(BOBJDIR)/OutputBuffer.o \
	$(BOBJDIR)/Packet.o \
	$(BOBJDIR)/Property.o \
	$(BOBJDIR)/Reader.o \
	$(BOBJDIR)/Result.o \
	$(BOBJDIR)/Shared.o \
	$(BOBJDIR)/TCPSocket.o \
	$(BOBJDIR)/Test.o \
	$(BOBJDIR)/util.o

default: library server install

clean:
	$(RM) $(OBJECTS)
	$(RM) $(BOBJDIR)/*.gcno $(BOBJDIR)/*.gcda
	$(RM) $(WOBJDIR)/*.gcno $(WOBJDIR)/*.gcda
	$(RM) $(BINDIR)/$(TARGET)
	$(RM) $(BINDIR)/bolt.jar
	$(RM) bolt.jar
	$(RM) -r classes/*

library: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@

server: classes
	$(JAVA_BIN)/javac -d classes $(JAVA_OPTS) java/bolt/*.java
	$(JAVA_BIN)/jar cvfm bolt.jar java/MANIFEST -C classes .

install: $(BINDIR)/bolt.jar

$(BINDIR)/bolt.jar: bolt.jar
	cp bolt.jar $@

classes:
	mkdir classes

$(BOBJDIR)/%.o: ./src/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(WOBJDIR)/%.o: ./gmsec/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

