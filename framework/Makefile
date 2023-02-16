# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GMSEC_API_HOME = ..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

TARGET	= libgmsec_api.$(SHLIB_EXT)

OBJDIR5	= ./src/gmsec5
SPTDIR  = ./support/src


CORE5_OBJ = \
	$(OBJDIR5)/Config.o \
	$(OBJDIR5)/ConfigFile.o \
	$(OBJDIR5)/ConfigFileIterator.o \
	$(OBJDIR5)/Connection.o \
	$(OBJDIR5)/GmsecException.o \
	$(OBJDIR5)/FieldSpecification.o \
	$(OBJDIR5)/HeartbeatGenerator.o \
	$(OBJDIR5)/Message.o \
	$(OBJDIR5)/MessageFactory.o \
	$(OBJDIR5)/MessageFieldIterator.o \
	$(OBJDIR5)/MessageSpecification.o \
	$(OBJDIR5)/ResourceGenerator.o \
	$(OBJDIR5)/SchemaIDIterator.o \
	$(OBJDIR5)/Specification.o \
	$(OBJDIR5)/Status.o \
	$(OBJDIR5)/SubscriptionInfo.o \
	$(OBJDIR5)/field/BinaryField.o \
	$(OBJDIR5)/field/BooleanField.o \
	$(OBJDIR5)/field/CharField.o \
	$(OBJDIR5)/field/F32Field.o \
	$(OBJDIR5)/field/F64Field.o \
	$(OBJDIR5)/field/Field.o \
	$(OBJDIR5)/field/I16Field.o \
	$(OBJDIR5)/field/I32Field.o \
	$(OBJDIR5)/field/I64Field.o \
	$(OBJDIR5)/field/I8Field.o \
	$(OBJDIR5)/field/StringField.o \
	$(OBJDIR5)/field/U16Field.o \
	$(OBJDIR5)/field/U32Field.o \
	$(OBJDIR5)/field/U64Field.o \
	$(OBJDIR5)/field/U8Field.o \


INTERNAL5_OBJ = \
	$(OBJDIR5)/internal/ActiveSubscriptions.o \
	$(OBJDIR5)/internal/AsyncPublisher.o \
	$(OBJDIR5)/internal/AutoDispatcher.o \
	$(OBJDIR5)/internal/CallbackAdapter.o \
	$(OBJDIR5)/internal/CallbackLookup.o \
	$(OBJDIR5)/internal/ComplianceValidator.o \
	$(OBJDIR5)/internal/ConnectionCallback.o \
	$(OBJDIR5)/internal/ConnectionCallbackCache.o \
	$(OBJDIR5)/internal/ConnectionFactory.o \
	$(OBJDIR5)/internal/CustomMessageValidator.o \
	$(OBJDIR5)/internal/Dispatcher.o \
	$(OBJDIR5)/internal/DynamicFactory.o \
	$(OBJDIR5)/internal/Encoder.o \
	$(OBJDIR5)/internal/ExclusionFilter.o \
	$(OBJDIR5)/internal/FieldTemplate.o \
	$(OBJDIR5)/internal/FieldTemplateDependency.o \
	$(OBJDIR5)/internal/FileUtil.o \
	$(OBJDIR5)/internal/InternalAtomics.o \
	$(OBJDIR5)/internal/InternalConfig.o \
	$(OBJDIR5)/internal/InternalConfigFile.o \
	$(OBJDIR5)/internal/InternalConfigFileIterator.o \
	$(OBJDIR5)/internal/InternalConnection.o \
	$(OBJDIR5)/internal/InternalFieldSpecification.o \
	$(OBJDIR5)/internal/InternalGmsecException.o \
	$(OBJDIR5)/internal/InternalHeartbeatGenerator.o \
	$(OBJDIR5)/internal/InternalMessage.o \
	$(OBJDIR5)/internal/InternalMessageFactory.o \
	$(OBJDIR5)/internal/InternalMessageFieldIterator.o \
	$(OBJDIR5)/internal/InternalMessageSpecification.o \
	$(OBJDIR5)/internal/InternalResourceGenerator.o \
	$(OBJDIR5)/internal/InternalSchemaIDIterator.o \
	$(OBJDIR5)/internal/InternalSpecification.o \
	$(OBJDIR5)/internal/InternalStatus.o \
	$(OBJDIR5)/internal/InternalSubscriptionInfo.o \
	$(OBJDIR5)/internal/PerformanceLogger.o \
	$(OBJDIR5)/internal/MathUtil.o \
	$(OBJDIR5)/internal/MessageAggregationToolkit.o \
	$(OBJDIR5)/internal/MessageTemplate.o \
	$(OBJDIR5)/internal/Middleware.o \
	$(OBJDIR5)/internal/MsgFieldMap.o \
	$(OBJDIR5)/internal/RequestThread.o \
	$(OBJDIR5)/internal/ResourceInfoCollector.o \
	$(OBJDIR5)/internal/SchemaTemplate.o \
	$(OBJDIR5)/internal/StringUtil.o \
	$(OBJDIR5)/internal/Subject.o \
	$(OBJDIR5)/internal/SystemUtil.o \
	$(OBJDIR5)/internal/TicketMutex.o \
	$(OBJDIR5)/internal/TrackingDetails.o \
	$(OBJDIR5)/internal/UniqueFilter.o \
	$(OBJDIR5)/internal/Value.o \
	$(OBJDIR5)/internal/field/InternalBinaryField.o \
	$(OBJDIR5)/internal/field/InternalBooleanField.o \
	$(OBJDIR5)/internal/field/InternalCharField.o \
	$(OBJDIR5)/internal/field/InternalF32Field.o \
	$(OBJDIR5)/internal/field/InternalF64Field.o \
	$(OBJDIR5)/internal/field/InternalField.o \
	$(OBJDIR5)/internal/field/InternalI16Field.o \
	$(OBJDIR5)/internal/field/InternalI32Field.o \
	$(OBJDIR5)/internal/field/InternalI64Field.o \
	$(OBJDIR5)/internal/field/InternalI8Field.o \
	$(OBJDIR5)/internal/field/InternalStringField.o \
	$(OBJDIR5)/internal/field/InternalU16Field.o \
	$(OBJDIR5)/internal/field/InternalU32Field.o \
	$(OBJDIR5)/internal/field/InternalU64Field.o \
	$(OBJDIR5)/internal/field/InternalU8Field.o \
	$(OBJDIR5)/internal/util/InternalCondition.o \
	$(OBJDIR5)/internal/util/InternalLog.o \
	$(OBJDIR5)/internal/util/InternalLoggerStream.o \
	$(OBJDIR5)/internal/util/InternalLogStream.o \
	$(OBJDIR5)/internal/util/InternalMutex.o \


COBJDIR5=$(OBJDIR5)/c
CAPI5_OBJ	= \
	$(COBJDIR5)/config.o \
	$(COBJDIR5)/config_file.o \
	$(COBJDIR5)/config_file_iterator.o \
	$(COBJDIR5)/connection.o \
	$(COBJDIR5)/heartbeat_generator.o \
	$(COBJDIR5)/message.o \
	$(COBJDIR5)/message_factory.o \
	$(COBJDIR5)/message_field_iterator.o \
	$(COBJDIR5)/resource_generator.o \
	$(COBJDIR5)/schema_id_iterator.o \
	$(COBJDIR5)/specification.o \
	$(COBJDIR5)/status.o \
	$(COBJDIR5)/subscription_info.o \
	$(COBJDIR5)/field/binary_field.o \
	$(COBJDIR5)/field/boolean_field.o \
	$(COBJDIR5)/field/char_field.o \
	$(COBJDIR5)/field/i16_field.o \
	$(COBJDIR5)/field/i32_field.o \
	$(COBJDIR5)/field/i64_field.o \
	$(COBJDIR5)/field/i8_field.o \
	$(COBJDIR5)/field/f32_field.o \
	$(COBJDIR5)/field/f64_field.o \
	$(COBJDIR5)/field/field.o \
	$(COBJDIR5)/field/string_field.o \
	$(COBJDIR5)/field/u16_field.o \
	$(COBJDIR5)/field/u32_field.o \
	$(COBJDIR5)/field/u64_field.o \
	$(COBJDIR5)/field/u8_field.o \
	$(COBJDIR5)/util/log.o \
	$(COBJDIR5)/util/timeutil.o


UTIL5_OBJ   = \
	$(OBJDIR5)/util/Atomics.o \
	$(OBJDIR5)/util/Condition.o \
	$(OBJDIR5)/util/CountDownLatch.o \
	$(OBJDIR5)/util/Log.o \
	$(OBJDIR5)/util/LoggerStream.o \
	$(OBJDIR5)/util/LogStream.o \
	$(OBJDIR5)/util/Mutex.o \
	$(OBJDIR5)/util/StdThread.o \
	$(OBJDIR5)/util/TimeUtil.o


SECURE_OBJ = \
	$(OBJDIR5)/secure/Access.o \
	$(OBJDIR5)/secure/Cipher.o \
	$(OBJDIR5)/secure/MersenneTwister.o \
	$(OBJDIR5)/secure/Policy.o \
	$(OBJDIR5)/secure/Random.o \
	$(OBJDIR5)/secure/Signer.o


SUPPORT_OBJ = \
	$(SPTDIR)/miniz.o \
	$(SPTDIR)/tinyxml2.o \
	$(SPTDIR)/jsoncpp.o


OBJECTS += $(CORE5_OBJ) $(INTERNAL5_OBJ) $(CAPI5_OBJ) $(UTIL5_OBJ) $(SECURE_OBJ)
OBJECTS += $(SUPPORT_OBJ)


default: message library


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Core API Framework"
	@echo "#"
	@echo "###########################################################"

library: $(BINDIR)/$(TARGET)


clean:
	find $(OBJDIR5) \( -name '*.o' -o -name '*.gcno' -o -name '*.gcda' \) -exec rm {} \;
	find $(SPTDIR)  \( -name '*.o' -o -name '*.gcno' -o -name '*.gcda' \) -exec rm {} \;
	$(RM) $(BINDIR)/$(TARGET)


$(COBJDIR5):
	mkdir -p $(COBJDIR5)

$(OBJDIR5)/%.o: $(OBJDIR5)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(OBJDIR5)/util/%.o: ./src/gmsec5/util/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(OBJDIR5)/secure/%.o: ./src/gmsec5/secure/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(COBJDIR5)/%.o: $(OBJDIR5)/c/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(OBJDIR5)/internal/%.o: $(OBJDIR5)/internal/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@



# 3rdParty support
#
./support/src/%.o: ./support/src/%.cpp
	$(CXX) -c $(API_CXXFLAGS) -w $< -o $@


# GMSEC API Shared-object library
#
$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(API_LDFLAGS) -o $@
