# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)


TESTS =	\
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
        T017_TimeUtil.exe \
        T018_Log.exe \
        T019_StringUtil.exe \
        T020_Atomics.exe \
        T021_CountDownLatch.exe \
        T030_StressConnection.exe \
        T031_MultipleConnections.exe \
        T032_MultipleSubscriptions.exe \
        T033_SubjectMatching.exe \
        T034_SizeRestrictions.exe \
        T035_TrackingFields.exe \
        T036_MessageBinning.exe \
        T037_HeartbeatTrackingFields.exe \
        T038_ConnectionEndpoint.exe \
        T039_DependentFields.exe \
        T040_FieldArrays.exe \
        T041_MsgOptimization.exe \
        T042_PerformanceLogger.exe \
        T043_MultiResponse.exe \
        T100_C2NSS_Publisher.exe \
        T100_C2NSS_Subscriber.exe \
        T101_LargeMessage.exe \
        T102_AsyncPublish.exe \
        T103_UserAuthActiveMQ.exe \
        T104_ActiveMQ_SSL.exe \
        T105_ActiveMQ_DurableMsgs.exe \
        T106_ArtemisFQQN.exe \
        T107_ArtemisFQQN.exe \
        T108_ArtemisFQQN.exe \


INTERNAL  = -I../../../framework/include -I../../../framework/support/include

CXXFLAGS += -g $(INTERNAL)

BIN_DIR   = ../bin

EXE_TESTS = $(patsubst %.exe,$(BIN_DIR)/%.exe,$(TESTS))

.PHONY: all clean


all: make_bin $(EXE_TESTS)


make_bin:
	@ mkdir -p $(BIN_DIR)


clean:
	$(RM) *.gcno *.gcda
	$(RM) $(EXE_TESTS)


$(BIN_DIR)/%.exe : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

