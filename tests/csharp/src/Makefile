# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_API_HOME=../../..

ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
MCS = /usr/local/bin/mcs
else
MCS = /usr/bin/mcs
endif

CS_ASSEMBLY = $(GMSEC_API_HOME)/bin/gmsec_csharp.dll

TEST_CASE   = TestCase.cs

TESTS       = \
              T001_Status.exe \
              T002_Config.exe \
              T003_Fields.exe \
              T004_GmsecException.exe \
              T005_Message.exe \
              T006_MessageFieldIterator.exe \
              T007_MessageFactory.exe \
              T008_ConfigFile.exe \
              T009_ConfigFileIterator.exe \
              T010_Connection.exe \
              T011_HeartbeatGenerator.exe \
              T012_Specification.exe \
              T013_SchemaIDIterator.exe \
              T014_MessageSpecification.exe \
              T015_FieldSpecification.exe \
              T016_ResourceGenerator.exe \
              T043_MultiResponse.exe \

BIN_DIR    = ../bin

EXE_TESTS  = $(patsubst %.exe,$(BIN_DIR)/%.exe,$(TESTS))

.PHONY: all clean


all: make_bin $(EXE_TESTS)


make_bin:
	@ mkdir -p $(BIN_DIR)


clean:
	$(RM) $(EXE_TESTS)


$(BIN_DIR)/%.exe: %.cs
	$(MCS) -r:$(CS_ASSEMBLY) -out:$@ $< $(TEST_CASE)

