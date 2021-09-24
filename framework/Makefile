# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GMSEC_HOME = ..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET	= libGMSECAPI.$(SHLIB_EXT)

OBJDIR	= ./src/gmsec
OBJDIR4	= ./src/gmsec4
SPTDIR  = ./support/src


CORE_OBJ    = \
	$(OBJDIR)/Config.o \
	$(OBJDIR)/ConfigFile.o \
	$(OBJDIR)/ConfigFileIterator.o \
	$(OBJDIR)/Connection.o \
	$(OBJDIR)/ConnectionFactory.o \
	$(OBJDIR)/Field.o \
	$(OBJDIR)/FieldIterator.o \
	$(OBJDIR)/Message.o \
	$(OBJDIR)/Status.o \
	$(OBJDIR)/mist/ConnectionManager.o \


INTERNAL_OBJ	= \
	$(OBJDIR)/internal/API4_Adapter.o \
	$(OBJDIR)/internal/AbstractFieldIterator.o \
	$(OBJDIR)/internal/BaseConfig.o \
	$(OBJDIR)/internal/BaseConfigFile.o \
	$(OBJDIR)/internal/BaseField.o \
	$(OBJDIR)/internal/BaseStatus.o \
	$(OBJDIR)/internal/DynamicFactory.o \
	$(OBJDIR)/internal/GarbageCollector.o \
	$(OBJDIR)/internal/InternalAtomics.o \
	$(OBJDIR)/internal/InternalConnection.o \
	$(OBJDIR)/internal/InternalMessage.o \
	$(OBJDIR)/internal/MessageBuddy.o \
	$(OBJDIR)/internal/MessageFieldMap.o \
	$(OBJDIR)/internal/SimpleConfigFileIterator.o \
	$(OBJDIR)/internal/SimpleFieldIterator.o \
	$(OBJDIR)/internal/StatusException.o \
	$(OBJDIR)/internal/Subject.o \
	$(OBJDIR)/internal/TrackingDetails.o \
	$(OBJDIR)/internal/UniqueFilter.o \
	$(OBJDIR)/internal/Value.o \
	$(OBJDIR)/internal/fileutil.o \
	$(OBJDIR)/internal/mathutil.o \
	$(OBJDIR)/internal/strutil.o \
	$(OBJDIR)/internal/mist/AbstractFieldDescriptor.o \
	$(OBJDIR)/internal/mist/AbstractBaseFieldDescriptor.o \
	$(OBJDIR)/internal/mist/BaseCompoundFieldDescriptor.o \
	$(OBJDIR)/internal/mist/BaseConnectionManager.o \
	$(OBJDIR)/internal/mist/BaseContext.o \
	$(OBJDIR)/internal/mist/BaseMessageTemplate.o \
	$(OBJDIR)/internal/mist/BaseSpecification.o \
	$(OBJDIR)/internal/mist/CompoundFieldDescriptor.o \
	$(OBJDIR)/internal/mist/Context.o \
	$(OBJDIR)/internal/mist/ConnMgrCallbacks.o \
	$(OBJDIR)/internal/mist/ConnMgrCallbackCache.o \
	$(OBJDIR)/internal/mist/DescriptorList.o \
	$(OBJDIR)/internal/mist/HeartbeatService.o \
	$(OBJDIR)/internal/mist/MessageTemplate.o \
	$(OBJDIR)/internal/mist/Specification.o



COBJDIR=$(OBJDIR)/c
CAPI_OBJ	= \
	$(COBJDIR)/config_c.o \
	$(COBJDIR)/configfile_c.o \
	$(COBJDIR)/connection_c.o \
	$(COBJDIR)/connection_factory_c.o \
	$(COBJDIR)/field_c.o \
	$(COBJDIR)/message_c.o \
	$(COBJDIR)/status_c.o \
	$(COBJDIR)/config.o \
	$(COBJDIR)/configfile.o \
	$(COBJDIR)/connection.o \
	$(COBJDIR)/connection_factory.o \
	$(COBJDIR)/field.o \
	$(COBJDIR)/message.o \
	$(COBJDIR)/status.o


CORE4_OBJ	= \
	$(OBJDIR4)/Config.o \
	$(OBJDIR4)/ConfigFile.o \
	$(OBJDIR4)/ConfigFileIterator.o \
	$(OBJDIR4)/Connection.o \
	$(OBJDIR4)/Exception.o \
	$(OBJDIR4)/HeartbeatGenerator.o \
	$(OBJDIR4)/Message.o \
	$(OBJDIR4)/MessageFieldIterator.o \
	$(OBJDIR4)/Status.o \
	$(OBJDIR4)/SubscriptionInfo.o \
	$(OBJDIR4)/field/BinaryField.o \
	$(OBJDIR4)/field/BooleanField.o \
	$(OBJDIR4)/field/CharField.o \
	$(OBJDIR4)/field/F32Field.o \
	$(OBJDIR4)/field/F64Field.o \
	$(OBJDIR4)/field/Field.o \
	$(OBJDIR4)/field/I16Field.o \
	$(OBJDIR4)/field/I32Field.o \
	$(OBJDIR4)/field/I64Field.o \
	$(OBJDIR4)/field/I8Field.o \
	$(OBJDIR4)/field/StringField.o \
	$(OBJDIR4)/field/U8Field.o \
	$(OBJDIR4)/field/U16Field.o \
	$(OBJDIR4)/field/U32Field.o \
	$(OBJDIR4)/field/U64Field.o \
	$(OBJDIR4)/mist/ConnectionManager.o \
	$(OBJDIR4)/mist/Device.o \
	$(OBJDIR4)/mist/DeviceIterator.o \
	$(OBJDIR4)/mist/DeviceMessage.o \
	$(OBJDIR4)/mist/DeviceParam.o \
	$(OBJDIR4)/mist/FieldSpecification.o \
	$(OBJDIR4)/mist/MessageSpecification.o \
	$(OBJDIR4)/mist/Mnemonic.o \
	$(OBJDIR4)/mist/MnemonicIterator.o \
	$(OBJDIR4)/mist/MnemonicMessage.o \
	$(OBJDIR4)/mist/MnemonicSample.o \
	$(OBJDIR4)/mist/ProductFile.o \
	$(OBJDIR4)/mist/ProductFileIterator.o \
	$(OBJDIR4)/mist/ProductFileMessage.o \
	$(OBJDIR4)/mist/SchemaIDIterator.o \
	$(OBJDIR4)/mist/ServiceParam.o \
	$(OBJDIR4)/mist/Specification.o \
	$(OBJDIR4)/mist/SubscriptionInfo.o \
	$(OBJDIR4)/mist/message/DeviceMessage.o \
	$(OBJDIR4)/mist/message/MistMessage.o \
	$(OBJDIR4)/mist/message/MnemonicMessage.o \
	$(OBJDIR4)/mist/message/ProductFileMessage.o 


INTERNAL4_OBJ	= \
	$(OBJDIR4)/internal/ActiveSubscriptions.o \
	$(OBJDIR4)/internal/AsyncPublisher.o \
	$(OBJDIR4)/internal/CallbackAdapter.o \
	$(OBJDIR4)/internal/CallbackLookup.o \
	$(OBJDIR4)/internal/ConnectionFactory.o \
	$(OBJDIR4)/internal/Dispatcher.o \
	$(OBJDIR4)/internal/DynamicFactory.o \
	$(OBJDIR4)/internal/Encoder.o \
	$(OBJDIR4)/internal/ExclusionFilter.o \
	$(OBJDIR4)/internal/FileUtil.o \
	$(OBJDIR4)/internal/InternalAtomics.o \
	$(OBJDIR4)/internal/InternalAutoDispatcher.o \
	$(OBJDIR4)/internal/InternalConfig.o \
	$(OBJDIR4)/internal/InternalConfigFile.o \
	$(OBJDIR4)/internal/InternalConfigFileIterator.o \
	$(OBJDIR4)/internal/InternalConnection.o \
	$(OBJDIR4)/internal/InternalException.o \
	$(OBJDIR4)/internal/InternalHeartbeatGenerator.o \
	$(OBJDIR4)/internal/InternalMessage.o \
	$(OBJDIR4)/internal/InternalMessageFieldIterator.o \
	$(OBJDIR4)/internal/InternalStatus.o \
	$(OBJDIR4)/internal/InternalSubscriptionInfo.o \
	$(OBJDIR4)/internal/MathUtil.o \
	$(OBJDIR4)/internal/MessageAggregationToolkit.o \
	$(OBJDIR4)/internal/Middleware.o \
	$(OBJDIR4)/internal/MsgFieldMap.o \
	$(OBJDIR4)/internal/RequestThread.o \
	$(OBJDIR4)/internal/StringUtil.o \
	$(OBJDIR4)/internal/Subject.o \
	$(OBJDIR4)/internal/SubscriptionDetails.o \
	$(OBJDIR4)/internal/SystemUtil.o \
	$(OBJDIR4)/internal/TicketMutex.o \
	$(OBJDIR4)/internal/TrackingDetails.o \
	$(OBJDIR4)/internal/UniqueFilter.o \
	$(OBJDIR4)/internal/Value.o \
	$(OBJDIR4)/internal/field/InternalBinaryField.o \
	$(OBJDIR4)/internal/field/InternalBooleanField.o \
	$(OBJDIR4)/internal/field/InternalCharField.o \
	$(OBJDIR4)/internal/field/InternalF32Field.o \
	$(OBJDIR4)/internal/field/InternalF64Field.o \
	$(OBJDIR4)/internal/field/InternalField.o \
	$(OBJDIR4)/internal/field/InternalI16Field.o \
	$(OBJDIR4)/internal/field/InternalI32Field.o \
	$(OBJDIR4)/internal/field/InternalI64Field.o \
	$(OBJDIR4)/internal/field/InternalI8Field.o \
	$(OBJDIR4)/internal/field/InternalStringField.o \
	$(OBJDIR4)/internal/field/InternalU8Field.o \
	$(OBJDIR4)/internal/field/InternalU16Field.o \
	$(OBJDIR4)/internal/field/InternalU32Field.o \
	$(OBJDIR4)/internal/field/InternalU64Field.o \
	$(OBJDIR4)/internal/mist/ConnMgrCallbacks.o \
	$(OBJDIR4)/internal/mist/ConnMgrCallbackCache.o \
	$(OBJDIR4)/internal/mist/CustomMessageValidator.o \
	$(OBJDIR4)/internal/mist/FieldTemplate.o \
	$(OBJDIR4)/internal/mist/FieldTemplateDependency.o \
	$(OBJDIR4)/internal/mist/HeartbeatService.o \
	$(OBJDIR4)/internal/mist/InternalConnectionManager.o \
	$(OBJDIR4)/internal/mist/InternalDevice.o \
	$(OBJDIR4)/internal/mist/InternalDeviceIterator.o \
	$(OBJDIR4)/internal/mist/InternalDeviceParam.o \
	$(OBJDIR4)/internal/mist/InternalFieldSpecification.o \
	$(OBJDIR4)/internal/mist/InternalMessageSpecification.o \
	$(OBJDIR4)/internal/mist/InternalMnemonic.o \
	$(OBJDIR4)/internal/mist/InternalMnemonicIterator.o \
	$(OBJDIR4)/internal/mist/InternalMnemonicSample.o \
	$(OBJDIR4)/internal/mist/InternalProductFile.o \
	$(OBJDIR4)/internal/mist/InternalProductFileIterator.o \
	$(OBJDIR4)/internal/mist/InternalSchemaIDIterator.o \
	$(OBJDIR4)/internal/mist/InternalServiceParam.o \
	$(OBJDIR4)/internal/mist/InternalSpecification.o \
	$(OBJDIR4)/internal/mist/InternalSubscriptionInfo.o \
	$(OBJDIR4)/internal/mist/MessagePopulator.o \
	$(OBJDIR4)/internal/mist/MessageSubclassHelper.o \
	$(OBJDIR4)/internal/mist/MessageTemplate.o \
	$(OBJDIR4)/internal/mist/MistCallbackAdapter.o \
	$(OBJDIR4)/internal/mist/ResourceInfoGenerator.o \
	$(OBJDIR4)/internal/mist/ResourceService.o \
	$(OBJDIR4)/internal/mist/SchemaTemplate.o \
	$(OBJDIR4)/internal/mist/message/InternalDeviceMessage.o \
	$(OBJDIR4)/internal/mist/message/InternalMistMessage.o \
	$(OBJDIR4)/internal/mist/message/InternalMnemonicMessage.o \
	$(OBJDIR4)/internal/mist/message/InternalProductFileMessage.o \
	$(OBJDIR4)/internal/performance/PerformanceLogger.o \
	$(OBJDIR4)/internal/util/InternalCondition.o \
	$(OBJDIR4)/internal/util/InternalLog.o \
	$(OBJDIR4)/internal/util/InternalLoggerStream.o \
	$(OBJDIR4)/internal/util/InternalLogStream.o \
	$(OBJDIR4)/internal/util/InternalMutex.o


COBJDIR4=$(OBJDIR4)/c
CAPI4_OBJ	= \
	$(COBJDIR4)/config.o \
	$(COBJDIR4)/config_file.o \
	$(COBJDIR4)/config_file_iterator.o \
	$(COBJDIR4)/connection.o \
	$(COBJDIR4)/heartbeat_generator.o \
	$(COBJDIR4)/message.o \
	$(COBJDIR4)/message_field_iterator.o \
	$(COBJDIR4)/status.o \
	$(COBJDIR4)/subscription_info.o \
	$(COBJDIR4)/field/binary_field.o \
	$(COBJDIR4)/field/boolean_field.o \
	$(COBJDIR4)/field/char_field.o \
	$(COBJDIR4)/field/i16_field.o \
	$(COBJDIR4)/field/i32_field.o \
	$(COBJDIR4)/field/i64_field.o \
	$(COBJDIR4)/field/i8_field.o \
	$(COBJDIR4)/field/f32_field.o \
	$(COBJDIR4)/field/f64_field.o \
	$(COBJDIR4)/field/field.o \
	$(COBJDIR4)/field/string_field.o \
	$(COBJDIR4)/field/u16_field.o \
	$(COBJDIR4)/field/u32_field.o \
	$(COBJDIR4)/field/u64_field.o \
	$(COBJDIR4)/field/u8_field.o \
	$(COBJDIR4)/mist/connection_manager.o \
	$(COBJDIR4)/mist/device.o \
	$(COBJDIR4)/mist/device_iterator.o \
	$(COBJDIR4)/mist/device_param.o \
	$(COBJDIR4)/mist/mnemonic.o \
	$(COBJDIR4)/mist/mnemonic_iterator.o \
	$(COBJDIR4)/mist/mnemonic_sample.o \
	$(COBJDIR4)/mist/product_file.o \
	$(COBJDIR4)/mist/product_file_iterator.o \
	$(COBJDIR4)/mist/schema_id_iterator.o \
	$(COBJDIR4)/mist/service_param.o \
	$(COBJDIR4)/mist/specification.o \
	$(COBJDIR4)/mist/message/device_message.o \
	$(COBJDIR4)/mist/message/mist_message.o \
	$(COBJDIR4)/mist/message/mnemonic_message.o \
	$(COBJDIR4)/mist/message/product_file_message.o \
	$(COBJDIR4)/util/log.o \
	$(COBJDIR4)/util/timeutil.o \


UTIL_OBJ	= \
	$(OBJDIR)/util/Atomics.o \
	$(OBJDIR)/util/Condition.o \
	$(OBJDIR)/util/CountDownLatch.o \
	$(OBJDIR)/util/Encoder.o \
	$(OBJDIR)/util/Log.o \
	$(OBJDIR)/util/log_c.o \
	$(OBJDIR)/secure/MersenneTwister.o \
	$(OBJDIR)/util/Mutex.o \
	$(OBJDIR)/util/Thread.o \
	$(OBJDIR)/secure/Random.o \
	$(OBJDIR)/util/String.o \
	$(OBJDIR)/util/StdThread.o \
	$(OBJDIR)/util/sysutil.o \
	$(OBJDIR)/util/timeutil.o \
	$(OBJDIR)/util/timeutil_c.o


UTIL4_OBJ	= \
	$(OBJDIR4)/util/Atomics.o \
	$(OBJDIR4)/util/Condition.o \
	$(OBJDIR4)/util/CountDownLatch.o \
	$(OBJDIR4)/util/Log.o \
	$(OBJDIR4)/util/LoggerStream.o \
	$(OBJDIR4)/util/LogStream.o \
	$(OBJDIR4)/util/Mutex.o \
	$(OBJDIR4)/util/StdThread.o \
	$(OBJDIR4)/util/TimeUtil.o


SECURE_OBJ	= \
	$(OBJDIR)/secure/Access.o \
	$(OBJDIR)/secure/Cipher.o \
	$(OBJDIR)/secure/Policy.o \
	$(OBJDIR)/secure/Signer.o


SECURE4_OBJ	= \
	$(OBJDIR4)/secure/Access.o \
	$(OBJDIR4)/secure/Cipher.o \
	$(OBJDIR4)/secure/MersenneTwister.o \
	$(OBJDIR4)/secure/Policy.o \
	$(OBJDIR4)/secure/Random.o \
	$(OBJDIR4)/secure/Signer.o


SUPPORT_OBJ	= \
	./support/src/miniz.o \
	./support/src/tinyxml2.o \
	./support/src/jsoncpp.o


OBJECTS  = $(CORE_OBJ)  $(INTERNAL_OBJ)  $(UTIL_OBJ)  $(CAPI_OBJ)  $(SECURE_OBJ)
OBJECTS += $(CORE4_OBJ) $(INTERNAL4_OBJ) $(UTIL4_OBJ) $(CAPI4_OBJ) $(SECURE4_OBJ)
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

library: $(COBJDIR) $(COBJDIR4) $(BINDIR)/$(TARGET)

clean:
	if [ "$(OBJDIR)" = "" ] ; then echo "empty OBJDIR- check GMSEC_PLATFORM"; false; fi
	find $(OBJDIR)  \( -name '*.o' -o -name '*.gcno' -o -name '*.gcda' \) -exec rm {} \;
	find $(OBJDIR4) \( -name '*.o' -o -name '*.gcno' -o -name '*.gcda' \) -exec rm {} \;
	find $(SPTDIR)  \( -name '*.o' -o -name '*.gcno' -o -name '*.gcda' \) -exec rm {} \;
	$(RM) $(BINDIR)/$(TARGET)


# API 3.x
#
$(COBJDIR):
	mkdir -p $(COBJDIR)

$(OBJDIR)/%.o: $(OBJDIR)/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(COBJDIR)/%.o: $(OBJDIR)/c/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(OBJDIR)/util/%.o: ./src/gmsec/util/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(OBJDIR)/internal/%.o: $(OBJDIR)/internal/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(OBJDIR)/internal/performance/%.o: ./src/gmsec/internal/performance/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(OBJDIR)/secure/%.o: ./src/gmsec/secure/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: $(OBJDIR)/mist/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: ./src/gmsec/internal/mist/%.cpp
	$(CXX) -c -w $(API_CXXFLAGS) $< -o $@


# API 4.x
#
$(COBJDIR4):
	mkdir -p $(COBJDIR4)

$(OBJDIR4)/%.o: $(OBJDIR4)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(OBJDIR4)/util/%.o: ./src/gmsec4/util/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(OBJDIR4)/secure/%.o: ./src/gmsec4/secure/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(COBJDIR4)/%.o: $(OBJDIR4)/c/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(OBJDIR4)/internal/%.o: $(OBJDIR4)/internal/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@



# 3rdParty support
#
./support/src/%.o: ./support/src/%.cpp
	$(CXX) -c $(API_CXXFLAGS) -w $< -o $@


# GMSEC API Shared-object library
#
$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(API_LDFLAGS) -o $@
