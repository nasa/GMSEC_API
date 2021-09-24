# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

 
GMSEC_HOME	     = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

CLIENT_TARGET    = libgmsec_mb.$(SHLIB_EXT)

SERVER_TARGET    = MBServer

LOCAL_INCS       = -I./include
LIBS             = 

CXXFLAGS        += $(LOCAL_INCS)
LDFLAGS         += $(STL_PORT)

OBJDIR           = ./src

CLIENT_OBJECTS   = $(OBJDIR)/gmsec_mb.o \
                   $(OBJDIR)/MBConnection.o \
                   $(OBJDIR)/MBReaderThread.o \
                   $(OBJDIR)/MBWire.o \
                   $(OBJDIR)/TCPSocket.o \
                   $(OBJDIR)/TCPSocketClient.o \
                   $(OBJDIR)/TCPSocketClientArray.o \
                   $(OBJDIR)/TCPSocketClientReconnector.o


SERVER_OBJECTS   = $(OBJDIR)/ConnectionMgr.o \
                   $(OBJDIR)/ConnectionWriter.o \
                   $(OBJDIR)/ConnMgrCallback.o \
                   $(OBJDIR)/MBFastMessage.o \
                   $(OBJDIR)/MBService.o \
                   $(OBJDIR)/MBWire.o \
                   $(OBJDIR)/MBResourceData.o \
                   $(OBJDIR)/MBServer.o \
                   $(OBJDIR)/TCPSocket.o \
                   $(OBJDIR)/TCPSocketServer.o


.PHONY: default client server clean


default : client server

client   : $(BINDIR)/$(CLIENT_TARGET)

server   : $(BINDIR)/$(SERVER_TARGET)


$(BINDIR)/$(CLIENT_TARGET) : $(CLIENT_OBJECTS)
	$(DLINK) $^ $(LDFLAGS) $(LIBS) -o $@ 
	@ echo
	@ echo "****"
	@ echo "**** Done building $(CLIENT_TARGET)"
	@ echo "****"
	@ echo
	@ echo


$(BINDIR)/$(SERVER_TARGET) : $(SERVER_OBJECTS)
	$(LINK) $^ $(LDFLAGS) $(LIBS) -o $@
	@ echo
	@ echo "****"
	@ echo "**** Done building $(SERVER_TARGET)"
	@ echo "****"
	@ echo
	@ echo


$(OBJDIR)/%.o: ./src/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


clean:
	$(RM) $(CLIENT_OBJECTS)
	$(RM) $(SERVER_OBJECTS)
	$(RM) $(OBJDIR)/$(SERVER_TARGET).o
	$(RM) $(OBJDIR)/*gcno $(OBJDIR)/*.gcda
	$(RM) $(BINDIR)/$(CLIENT_TARGET)
	$(RM) $(BINDIR)/$(SERVER_TARGET)
