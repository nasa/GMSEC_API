# Define paths and variables

GMSEC_HOME  = ../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

MAJOR_VER    = 3
MINOR_VER    = 12
PATCH_VER    = 1

GMSEC_LIBROOTNAME = gmsec_opendds$(MAJOR_VER)$(MINOR_VER)
GMSEC_LIBVERSION = OpenDDS_v$(MAJOR_VER).$(MINOR_VER)

TARGET 		= lib$(GMSEC_LIBROOTNAME).$(SHLIB_EXT)

OPENDDS_CFG = ../opendds/dds_tcp_conf.ini

SRCDIR 		= ../opendds/src
INCDIR 		= ../opendds/include
OBJDIR 		= $(SRCDIR)

SUPDIR 		= $(SUPPORT)/opendds-$(MAJOR_VER).$(MINOR_VER).$(PATCH_VER)

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(GMSEC_LIBROOTNAME)
LOCAL_DEFS  += -DGMSEC_LIBVERSION=$(GMSEC_LIBVERSION)

LOCAL_INCS 	= -I$(SUPDIR)/include -I$(INCDIR) -I$(SRCDIR)

API_CXXFLAGS += $(LOCAL_DEFS) $(LOCAL_INCS)

# TODO: Figure out if we should depend on all OpenDDS libraries
LDFLAGS 	+= -L$(SUPDIR)/lib $(shell ls $(SUPDIR)/lib/*.so)

TAO_IDL 	= $(SUPDIR)/bin/tao_idl
OPENDDS_IDL = $(SUPDIR)/bin/opendds_idl

GMSEC_IDL_FILE 		= $(SRCDIR)/OpenDDSMessage.idl
OPENDDS_IDL_FILE 	= $(SRCDIR)/OpenDDSMessageTypeSupport.idl

GEN_SRCS 	= $(SRCDIR)/OpenDDSMessageC.h \
			  $(SRCDIR)/OpenDDSMessageC.cpp \
			  $(SRCDIR)/OpenDDSMessageC.inl \
			  $(SRCDIR)/OpenDDSMessageS.h \
			  $(SRCDIR)/OpenDDSMessageS.cpp \
			  $(SRCDIR)/OpenDDSMessageTypeSupportImpl.h \
			  $(SRCDIR)/OpenDDSMessageTypeSupportImpl.cpp \
			  $(SRCDIR)/OpenDDSMessageTypeSupport.idl \
			  $(SRCDIR)/OpenDDSMessageTypeSupportC.cpp \
			  $(SRCDIR)/OpenDDSMessageTypeSupportC.h \
			  $(SRCDIR)/OpenDDSMessageTypeSupportC.inl \
			  $(SRCDIR)/OpenDDSMessageTypeSupportS.h \
			  $(SRCDIR)/OpenDDSMessageTypeSupportS.cpp
			  
SRC_FILES 	= $(SRCDIR)/OpenDDSConnection.cpp \
			  $(SRCDIR)/gmsec_opendds.cpp \
			  $(SRCDIR)/OpenDDSMessageTypeSupportImpl.cpp \
			  $(SRCDIR)/OpenDDSMessageC.cpp \
			  $(SRCDIR)/OpenDDSMessageS.cpp \
			  $(SRCDIR)/OpenDDSMessageTypeSupportC.cpp \
			  $(SRCDIR)/OpenDDSMessageTypeSupportS.cpp \
			  $(SRCDIR)/OpenDDSMessageListener.cpp \
			  $(SRCDIR)/Queue.cpp

OBJ_FILES 	= $(SRC_FILES:.cpp=.o)


.PHONY: all default clean


all default: clean gen-srcs library config


gen-srcs:
	@ LD_LIBRARY_PATH=$(SUPDIR)/lib $(TAO_IDL) $(GMSEC_IDL_FILE) -o $(OBJDIR)
	@ LD_LIBRARY_PATH=$(SUPDIR)/lib $(OPENDDS_IDL) $(GMSEC_IDL_FILE) -o $(OBJDIR)
	@ LD_LIBRARY_PATH=$(SUPDIR)/lib $(TAO_IDL) -I$(SRCDIR) -I$(SUPDIR)/include $(OPENDDS_IDL_FILE) -o $(OBJDIR)


library: $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJ_FILES)
	$(DLINK) $^ $(LDFLAGS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


config:
	@mkdir -p $(BINDIR)/opendds
	@cp $(OPENDDS_CFG) $(BINDIR)/opendds


clean:
	$(RM) $(OBJ_FILES)
	$(RM) $(OBJDIR)/*.gcda $(OBJDIR)/*.gcno
	$(RM) $(GEN_SRCS)
	$(RM) $(BINDIR)/$(TARGET)
	$(RM) -r $(BINDIR)/opendds
