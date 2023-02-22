#include "TestCase.h"


const char* const GOOD_CONFIG_FILE       = "good_config_file.xml";
const char* const GOOD_CONFIG_FILE_SAVED = "good_config_file_saved.xml";

const char* const XML =
	"<DEFINITIONS>\n" \
        "    <SUBSCRIPTION NAME=\"sub1\" PATTERN=\"GMSEC.&gt;\">\n" \
            "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.HB.+\"/>\n" \
            "        <EXCLUDE PATTERN=\"GMSEC.*.*.MSG.LOG.+\"/>\n" \
        "    </SUBSCRIPTION>\n" \
        "    <CONFIG NAME=\"Bolt\">\n" \
            "        <PARAMETER NAME=\"mw-id\">bolt</PARAMETER>\n" \
            "        <PARAMETER NAME=\"server\">localhost</PARAMETER>\n" \
        "    </CONFIG>\n" \
        "    <MESSAGE NAME=\"Msg1\">\n" \
            "        <CONFIG NAME=\"msg_config\">\n" \
                "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-SIZE\">15</PARAMETER>\n" \
                "            <PARAMETER NAME=\"GMSEC-MSGFLD-STORE-TYPE\">tree</PARAMETER>\n" \
            "        </CONFIG>\n" \
            "        <FIELD NAME=\"MESSAGE-TYPE\" TYPE=\"STRING\">MSG</FIELD>\n" \
            "        <FIELD NAME=\"MESSAGE-SUBTYPE\" TYPE=\"STRING\">HB</FIELD>\n" \
            "        <FIELD NAME=\"MISSION-ID\" TYPE=\"STRING\">MY-MISSION</FIELD>\n" \
            "        <FIELD NAME=\"CONSTELLATION-ID\" TYPE=\"STRING\">MY-CONSTELLATION</FIELD>\n" \
        "    </MESSAGE>\n" \
    "</DEFINITIONS>";


void test_configFileCreate()
{
	GMSEC_INFO("Test configFileCreate()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(status);
	testCheckBool("Status is bad", statusHasError(status) == GMSEC_FALSE);
	testCheckBool("ConfigFile is NULL", cfgFile != NULL);
	testCheckBool("Expected IsLoaded to report false", configFileIsLoaded(cfgFile, NULL) == GMSEC_FALSE);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileCreateUsingConfig()
{
	GMSEC_INFO("Test configFileCreateUsingConfig()");

	GMSEC_Status status = statusCreate();

	// Nominal test(s)
	{
		GMSEC_Config     config  = configCreate();
		GMSEC_ConfigFile cfgFile = configFileCreateUsingConfig(config, status);
		char             file[OUT_STR_LEN];

		testCheckBool("Status is bad", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("ConfigFile is NULL", cfgFile != NULL);
		testCheckBool("Expected IsLoaded to report false", configFileIsLoaded(cfgFile, NULL) == GMSEC_FALSE);

		testGetDataFile(GOOD_CONFIG_FILE, file);

		configFileLoad(cfgFile, file, status);
		testCheckBool("Expected to load good config file", statusHasError(status) == GMSEC_FALSE);

		GMSEC_Message msg = configFileLookupMessage(cfgFile, "Msg1", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Expected C2MS specification", messageGetSchemaLevel(msg, NULL) == GMSEC_SCHEMA_LEVEL_0);

		messageDestroy(&msg);
		configFileDestroy(&cfgFile);
		configDestroy(&config);
	}
	{
		GMSEC_Config     config  = configCreateUsingData("gmsec-schema-level=2", KEY_VALUE_DATA, NULL);
		GMSEC_ConfigFile cfgFile = configFileCreateUsingConfig(config, status);
		char             file[OUT_STR_LEN];

		testCheckBool("Status is bad", statusHasError(status) == GMSEC_FALSE);
		testCheckBool("ConfigFile is NULL", cfgFile != NULL);
		testCheckBool("Expected IsLoaded to report false", configFileIsLoaded(cfgFile, NULL) == GMSEC_FALSE);

		testGetDataFile(GOOD_CONFIG_FILE, file);

		configFileLoad(cfgFile, file, status);
		testCheckBool("Expected to load good config file", statusHasError(status) == GMSEC_FALSE);

		GMSEC_Message msg = configFileLookupMessage(cfgFile, "Msg1", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Expected GMSEC specification", messageGetSchemaLevel(msg, NULL) == GMSEC_SCHEMA_LEVEL_2);

		messageDestroy(&msg);
		configFileDestroy(&cfgFile);
		configDestroy(&config);
	}

	// Off-nominal test(s)
	{
		// Config is NULL
		GMSEC_ConfigFile cfgFile = configFileCreateUsingConfig(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected cfgFile to be NULL", cfgFile == NULL);
	}
	{
		// Cannot create MessageFactory for ConfigFile using bogus specification
		GMSEC_Config     config  = configCreateUsingData("gmsec-specification-version=201500", KEY_VALUE_DATA, NULL);
		GMSEC_ConfigFile cfgFile = configFileCreateUsingConfig(config, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected cfgFile to be NULL", cfgFile == NULL);
	}

	statusDestroy(&status);
}


void test_configFileDestroy()
{
	GMSEC_INFO("Test configFileDestroy()");

	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	configFileDestroy(&cfgFile);
	testCheckBool("ConfigFile should be NULL", cfgFile == NULL);
}


void test_configFileLoad()
{
	GMSEC_INFO("Test configFileLoad()");

	GMSEC_Status     status   = statusCreate();
	GMSEC_ConfigFile cfgFile  = configFileCreate(NULL);
	char             file[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE, file);

	// Nominal test
	configFileLoad(cfgFile, file, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Expected IsLoaded to report true", configFileIsLoaded(cfgFile, NULL) == GMSEC_TRUE);

	// Off-nominal tests
	configFileLoad(NULL, file, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configFileLoad(cfgFile, NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Expected IsLoaded to report false", configFileIsLoaded(cfgFile, NULL) == GMSEC_FALSE);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileSave()
{
	GMSEC_INFO("Test configFileSave()");

	GMSEC_Status     status   = statusCreate();
	GMSEC_ConfigFile cfgFile  = configFileCreate(NULL);
	char             file[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE_SAVED, file);

	configFileLoad(cfgFile, file, status);

	// Nominal test
	configFileSave(cfgFile, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Off-nominal tests
	configFileSave(NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

#ifdef WIN32
	_unlink(file);
#else
	unlink(file);
#endif

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileSaveToFile()
{
	GMSEC_INFO("Test configFileSaveToFile()");

	GMSEC_Status     status   = statusCreate();
	GMSEC_ConfigFile cfgFile1 = configFileCreate(NULL);
	GMSEC_ConfigFile cfgFile2 = configFileCreate(NULL);
	char             file[OUT_STR_LEN];
	char             newFile[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE, file);
	testGetDataFile(GOOD_CONFIG_FILE_SAVED, newFile);

	configFileLoad(cfgFile1, file, status);

	// Nominal test
	configFileSaveToFile(cfgFile1, newFile, GMSEC_FALSE, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	configFileLoad(cfgFile2, newFile, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	testCheckBool("Unexpected XML content", strcompare(configFileToXML(cfgFile1, NULL), configFileToXML(cfgFile2, NULL)) == 0);

	// Off-nominal tests
	configFileSaveToFile(NULL, newFile, GMSEC_FALSE, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configFileSaveToFile(cfgFile1, NULL, GMSEC_FALSE, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

#ifdef WIN32
	_unlink(newFile);
#else
	unlink(newFile);
#endif

	configFileDestroy(&cfgFile1);
	configFileDestroy(&cfgFile2);
	statusDestroy(&status);
}


void test_configFileFromXML()
{
	GMSEC_INFO("Test configFileFromXML()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);

	// Nominal test
	{
		configFileFromXML(cfgFile, XML, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Expected IsLoaded to report true", configFileIsLoaded(cfgFile, NULL) == GMSEC_TRUE);
	}

	// Off-nominal test
	{
		// ConfigFile is NULL
		configFileFromXML(NULL, XML, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	}
	{
		// XML data is NULL
		configFileFromXML(cfgFile, NULL, status);
		testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected IsLoaded to report false", configFileIsLoaded(cfgFile, NULL) == GMSEC_FALSE);
	}

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileToXML()
{
	GMSEC_INFO("Test configFileToXML()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_ConfigFile cfgFile = configFileCreate(NULL);

		configFileFromXML(cfgFile, XML, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Unexpected XML content", strcompare(configFileToXML(cfgFile, NULL), XML) == 0);

		configFileDestroy(&cfgFile);
	}

	// Off-nominal test
	{
		// ConfigFile is NULL
		const char* xml = configFileToXML(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("XML data should be NULL", xml == NULL);
	}

	statusDestroy(&status);
}


void test_configFileIsLoaded()
{
	GMSEC_INFO("Test configFileIsLoaded()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	char             file[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE, file);

	// Nominal tests
	testCheckBool("Expected isLoaded to report GMSEC_FALSE", configFileIsLoaded(cfgFile, NULL) == GMSEC_FALSE);

	configFileLoad(cfgFile, file, NULL);
	testCheckBool("Expected isLoaded to report GMSEC_TRUE", configFileIsLoaded(cfgFile, NULL) == GMSEC_TRUE);

	// Off-nominal test
	testCheckBool("Expected isLoaded to report GMSEC_FALSE", configFileIsLoaded(NULL, status) == GMSEC_FALSE);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileAddConfig()
{
	GMSEC_INFO("Test configFileAddConfig()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	GMSEC_Config     config  = configCreateUsingData("name1=value1", KEY_VALUE_DATA, NULL);

	// Nominal test
	{
		configFileAddConfig(cfgFile, "Cfg1", config, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		GMSEC_Config tmp = configFileLookupConfig(cfgFile, "Cfg1", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Config handle is NULL", tmp != NULL);
		testCheckBool("Unexpected config value", strcompare(configGetValue(tmp, "name1", NULL), configGetValue(config, "name1", NULL)) == 0);
		configDestroy(&tmp);
	}

	// Off-nominal tests
	{
		// ConfigFile is NULL
		configFileAddConfig(NULL, "Cfg1", config, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Config is NULL
		configFileAddConfig(cfgFile, "Cfg1", NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Config name is NULL
		configFileAddConfig(cfgFile, NULL, config, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Config name is empty-string
		configFileAddConfig(cfgFile, "", config, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	configDestroy(&config);
	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileLookupConfig()
{
	GMSEC_INFO("Test configFileLookupConfig()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	char             file[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE, file);

	configFileLoad(cfgFile, file, NULL);

	// Nominal tests
	GMSEC_Config config = configFileLookupConfig(cfgFile, "Bolt", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Config handle is NULL", config != NULL);
	testCheckBool("Unexpected mw-id value", strcompare(configGetValue(config, "mw-id", NULL), "bolt") == 0);
	testCheckBool("Unexpected server value", strcompare(configGetValue(config, "server", NULL), "localhost") == 0);
	configDestroy(&config);

	config = configFileLookupConfig(cfgFile, "ActiveMQ", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Config handle is NULL", config != NULL);
	testCheckBool("Unexpected mw-id value", strcompare(configGetValue(config, "mw-id", NULL), "activemq39") == 0);
	testCheckBool("Unexpected server value", strcompare(configGetValue(config, "server", NULL), "tcp://localhost:61616") == 0);
	configDestroy(&config);

	// Off-nominal tests
	config = configFileLookupConfig(cfgFile, "bogus", status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Config handle should be NULL", config == NULL);

	config = configFileLookupConfig(NULL, "Bolt", status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Config handle should be NULL", config == NULL);

	config = configFileLookupConfig(cfgFile, NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Config handle should be NULL", config == NULL);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileRemoveConfig()
{
	GMSEC_INFO("Test configFileRemoveConfig()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	GMSEC_Config     config  = configCreateUsingData("mw-id=bolt server=localhost", KEY_VALUE_DATA, NULL);

	configFileAddConfig(cfgFile, "Bolt", config, NULL);

	// Nominal test
	{
		testCheckBool("Expected to remove existing config", configFileRemoveConfig(cfgFile, "Bolt", status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Did not expect to remove config", configFileRemoveConfig(cfgFile, "Bolt", status) == GMSEC_FALSE);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// ConfigFile is NULL
		configFileRemoveConfig(NULL, "Bolt", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Non-existent entry
		testCheckBool("Did not expect to remove config", configFileRemoveConfig(cfgFile, "bogus", status) == GMSEC_FALSE);
	}
	{
		// NULL entry name
		testCheckBool("Did not expect to remove config", configFileRemoveConfig(cfgFile, NULL, status) == GMSEC_FALSE);
	}

	configDestroy(&config);
	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileAddMessage()
{
	GMSEC_INFO("Test configFileAddMessage()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_ConfigFile     cfgFile = configFileCreate(NULL);
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);
	GMSEC_Field          field1  = stringFieldCreate("FIELD1", "ONE", GMSEC_FALSE, NULL);
	GMSEC_Field          field2  = stringFieldCreate("FIELD2", "TWO", GMSEC_FALSE, NULL);

	messageAddField(msg, field1, NULL);
	messageAddField(msg, field2, NULL);

	// Nominal test
	{
		configFileAddMessage(cfgFile, "my-msg", msg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		GMSEC_Message tmp = configFileLookupMessage(cfgFile, "my-msg", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Message is NULL", msg != NULL);
		testCheckBool("Message XML content differs", strcompare(messageToXML(msg, NULL), messageToXML(tmp, NULL)) == 0);
		messageDestroy(&tmp);
	}

	// Off-nominal test
	{
		// ConfigFile is NULL
		configFileAddMessage(NULL, "my-msg", msg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Message is NULL
		configFileAddMessage(cfgFile, "my-msg", NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Message name is NULL
		configFileAddMessage(cfgFile, NULL, msg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Message name is empty-string
		configFileAddMessage(cfgFile, "", msg, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	fieldDestroy(&field1);
	fieldDestroy(&field2);
	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileLookupMessage()
{
	GMSEC_INFO("Test configFileLookupMessage()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	char             file[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE, file);

	configFileLoad(cfgFile, file, NULL);

	// Nominal tests
	GMSEC_Message msg = configFileLookupMessage(cfgFile, "Msg1", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Message handle is NULL", msg != NULL);
	testCheckBool("Unexpected MESSAGE-TYPE", strcompare(messageGetStringValue(msg, "MESSAGE-TYPE", NULL), "MSG") == 0);
	testCheckBool("Unexpected MESSAGE-SUBTYPE", strcompare(messageGetStringValue(msg, "MESSAGE-SUBTYPE", NULL), "HB") == 0);
	messageDestroy(&msg);

	// Off-nominal tests
	msg = configFileLookupMessage(cfgFile, "bogus", status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Message handle should be NULL", msg == NULL);

	msg = configFileLookupMessage(NULL, "Msg1", status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Message handle should be NULL", msg == NULL);

	msg = configFileLookupMessage(cfgFile, NULL, status);
	testCheckBool("Expected an error", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Message handle should be NULL", msg == NULL);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileRemoveMessage()
{
	GMSEC_INFO("Test configFileRemoveMessage()");

	GMSEC_Status         status  = statusCreate();
	GMSEC_ConfigFile     cfgFile = configFileCreate(NULL);
	GMSEC_MessageFactory factory = messageFactoryCreate(NULL);
	GMSEC_Message        msg     = messageFactoryCreateMessage(factory, "HB", NULL);

	configFileAddMessage(cfgFile, "Heartbeat", msg, NULL);

	// Nominal test
	{
		testCheckBool("Expected to remove existing message", configFileRemoveMessage(cfgFile, "Heartbeat", status) == GMSEC_TRUE);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Expected removal to be false for non-existent message", configFileRemoveMessage(cfgFile, "Heartbeat", status) == GMSEC_FALSE);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		testCheckBool("Expected removal to be false for non-existent message", configFileRemoveMessage(cfgFile, "bogus", status) == GMSEC_FALSE);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal tests
	{
		// ConfigFile is NULL
		GMSEC_BOOL removed = configFileRemoveMessage(NULL, "Heartbeat", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected removed to be false", removed == GMSEC_FALSE);
	}

	messageDestroy(&msg);
	messageFactoryDestroy(&factory);
	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileAddSubscriptionEntry()
{
	GMSEC_INFO("Test configFileAddSubscriptionEntry()");

	GMSEC_Status            status  = statusCreate();
	GMSEC_ConfigFile        cfgFile = configFileCreate(NULL);
	GMSEC_SubscriptionEntry entry   = subscriptionEntryCreate("Sub1", "GMSEC.>", NULL);

	// Nominal test
	configFileAddSubscriptionEntry(cfgFile, entry, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	subscriptionEntryDestroy(&entry);
	testCheckBool("Expected entry to be NULL", entry == NULL);

	entry = configFileLookupSubscriptionEntry(cfgFile, "Sub1", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("SubscriptionEntry handle is NULL", entry != NULL);
	testCheckBool("Unexpected subscription entry name", strcompare(subscriptionEntryGetName(entry, NULL), "Sub1") == 0);
	testCheckBool("Unexpected subscription entry pattern", strcompare(subscriptionEntryGetPattern(entry, NULL), "GMSEC.>") == 0);

	// Off-nominal tests
	configFileAddSubscriptionEntry(NULL, entry, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	configFileAddSubscriptionEntry(cfgFile, NULL, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileLookupSubscriptionEntry()
{
	GMSEC_INFO("Test configFileLookupSubscriptionEntry()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);
	char             file[OUT_STR_LEN];

	testGetDataFile(GOOD_CONFIG_FILE, file);

	configFileLoad(cfgFile, file, NULL);

	// Nominal test
	GMSEC_SubscriptionEntry entry = configFileLookupSubscriptionEntry(cfgFile, "Sub1", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("SubscriptionEntry handle is NULL", entry != NULL);
	testCheckBool("Unexpected subscription entry name", strcompare(subscriptionEntryGetName(entry, NULL), "Sub1") == 0);
	testCheckBool("Unexpected subscription entry pattern", strcompare(subscriptionEntryGetPattern(entry, NULL), "GMSEC.>") == 0);

	// Off-nominal tests
	entry = configFileLookupSubscriptionEntry(cfgFile, "bogus", status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("SubscriptionEntry handle should be NULL", entry == NULL);

	entry = configFileLookupSubscriptionEntry(NULL, "Sub1", status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("SubscriptionEntry handle should be NULL", entry == NULL);

	entry = configFileLookupSubscriptionEntry(cfgFile, NULL, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("SubscriptionEntry handle should be NULL", entry == NULL);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileRemoveSubscription()
{
	GMSEC_INFO("Test configFileRemoveSubscription()");

	GMSEC_Status            status  = statusCreate();
	GMSEC_ConfigFile        cfgFile = configFileCreate(NULL);
	GMSEC_SubscriptionEntry entry   = subscriptionEntryCreate("AllMessages", "GMSEC.>", NULL);

	configFileAddSubscriptionEntry(cfgFile, entry, NULL);

	// Nominal test
	testCheckBool("Expected to remove existing subscription", configFileRemoveSubscriptionEntry(cfgFile, "AllMessages", status) == GMSEC_TRUE);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Off-nominal tests
	testCheckBool("Did not expect to remove subscription", configFileRemoveSubscriptionEntry(cfgFile, "AllMessages", status) == GMSEC_FALSE);
	testCheckBool("Did not expect to remove subscription", configFileRemoveSubscriptionEntry(cfgFile, "bogus", status) == GMSEC_FALSE);
	testCheckBool("Did not expect to remove subscription", configFileRemoveSubscriptionEntry(cfgFile, NULL, status) == GMSEC_FALSE);

	testCheckBool("Expected false to be returned", configFileRemoveSubscriptionEntry(NULL, "AllMessages", status) == GMSEC_FALSE);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);

	subscriptionEntryDestroy(&entry);
	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_configFileGetIterator()
{
	GMSEC_INFO("Test configFileGetIterator()");

	GMSEC_Status     status  = statusCreate();
	GMSEC_ConfigFile cfgFile = configFileCreate(NULL);

	// Nominal test
	GMSEC_ConfigFileIterator iter = configFileGetIterator(cfgFile, status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	testCheckBool("Iterator is NULL", iter != NULL);

	// Off-nominal test
	iter = configFileGetIterator(NULL, status);
	testCheckBool("An error was expected", statusHasError(status) == GMSEC_TRUE);
	testCheckBool("Iterator should be NULL", iter == NULL);

	configFileDestroy(&cfgFile);
	statusDestroy(&status);
}


void test_subscriptionEntryCreate()
{
	GMSEC_INFO("Test subscriptionEntryCreate()");

	GMSEC_Status status = statusCreate();

	// Nominal test
	{
		GMSEC_SubscriptionEntry entry = subscriptionEntryCreate("All-Messages", "GMSEC.>", status);

		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Entry should not be NULL", entry != NULL);
		testCheckBool("Unexpected entry name", strcompare("All-Messages", subscriptionEntryGetName(entry, NULL)) == 0);
		testCheckBool("Unexpected entry pattern", strcompare("GMSEC.>", subscriptionEntryGetPattern(entry, NULL)) == 0);

		subscriptionEntryDestroy(&entry);
	}

	// Off-nominal test(s)
	{
		// Entry name is NULL
		GMSEC_SubscriptionEntry entry = subscriptionEntryCreate(NULL, "GMSEC.>", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("1. Entry should be NULL", entry == NULL);
	}
	{
		// Entry name is empty-string
		GMSEC_SubscriptionEntry entry = subscriptionEntryCreate("", "GMSEC.>", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("2. Entry should be NULL", entry == NULL);
	}
	{
		// Entry pattern is NULL
		GMSEC_SubscriptionEntry entry = subscriptionEntryCreate("All-Messages", NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("3. Entry should be NULL", entry == NULL);
	}
	{
		// Entry pattern is empty-string
		GMSEC_SubscriptionEntry entry = subscriptionEntryCreate("All-Messages", "", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("4. Entry should be NULL", entry == NULL);
	}
	{
		// Entry pattern is malformed
		GMSEC_SubscriptionEntry entry = subscriptionEntryCreate("All-Messages", "GMSEC%BAR", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("5. Entry should be NULL", entry == NULL);
	}

	statusDestroy(&status);
}


void test_subscriptionEntryGetName()
{
	GMSEC_INFO("Test subscriptionEntryGetName()");

	GMSEC_Status status = statusCreate();

	// Nominal tests are performed elsewhere

	// Off-nominal test(s)
	{
		// SubscriptionEntry is NULL
		const char* name = subscriptionEntryGetName(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Entry name should be NULL", name == NULL);
	}

	statusDestroy(&status);
}


void test_subscriptionEntryGetPattern()
{
	GMSEC_INFO("Test subscriptionEntryGetPattern()");

	GMSEC_Status status = statusCreate();

	// Nominal tests are performed elsewhere

	// Off-nominal test(s)
	{
		// SubscriptionEntry is NULL
		const char* pattern = subscriptionEntryGetPattern(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Entry pattern should be NULL", pattern == NULL);
	}

	statusDestroy(&status);
}


void test_subscriptionEntryAddExcludedPattern()
{
	GMSEC_INFO("Test subscriptionEntryAddExcludedPattern()");

	GMSEC_Status            status = statusCreate();
	GMSEC_SubscriptionEntry entry  = subscriptionEntryCreate("All-Messages", "GMSEC.>", status);

	// Nominal test
	{
		subscriptionEntryAddExcludedPattern(entry, "GMSEC.*.*.MSG.LOG.+", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
	}

	// Off-nominal test(s)
	{
		// SubscriptionEntry is NULL
		subscriptionEntryAddExcludedPattern(NULL, "GMSEC.*.*.MSG.LOG.+", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Pattern is NULL
		subscriptionEntryAddExcludedPattern(entry, NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}
	{
		// Pattern is empty-string
		subscriptionEntryAddExcludedPattern(entry, "", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
	}

	subscriptionEntryDestroy(&entry);
	statusDestroy(&status);
}


void test_subscriptionEntryHasNextExcludedPattern()
{
	GMSEC_INFO("Test subscriptionEntryHasNextExcludedPattern()");

	GMSEC_Status            status = statusCreate();
	GMSEC_SubscriptionEntry entry  = subscriptionEntryCreate("All-Messages", "GMSEC.>", status);

	// Nominal test
	{
		subscriptionEntryAddExcludedPattern(entry, "GMSEC.*.*.MSG.LOG.+", status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

		GMSEC_BOOL hasEntry = subscriptionEntryHasNextExcludedPattern(entry, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);
		testCheckBool("Expected to have an entry", hasEntry == GMSEC_TRUE);
	}

	// Off-nominal test
	{
		// SubscriptionEntry is NULL
		GMSEC_BOOL hasEntry = subscriptionEntryHasNextExcludedPattern(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected to have an entry", hasEntry == GMSEC_FALSE);
	}

	subscriptionEntryDestroy(&entry);
	statusDestroy(&status);
}


void test_subscriptionEntryNextExcludedPattern()
{
	GMSEC_INFO("Test subscriptionEntryNextExcludedPattern()");

	GMSEC_Status            status = statusCreate();
	GMSEC_SubscriptionEntry entry  = subscriptionEntryCreate("All-Messages", "GMSEC.>", status);

	subscriptionEntryAddExcludedPattern(entry, "GMSEC.*.*.MSG.LOG.+", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	subscriptionEntryAddExcludedPattern(entry, "GMSEC.*.*.MSG.HB.+", status);
	testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

	// Nominal test
	{
		int patterns = 0;

		while (subscriptionEntryHasNextExcludedPattern(entry, NULL) == GMSEC_TRUE)
		{
			++patterns;

			const char* pattern = subscriptionEntryNextExcludedPattern(entry, status);
			testCheckBool(statusGet(status), statusHasError(status) == GMSEC_FALSE);

			if (patterns == 1)
			{
				testCheckBool("Unexpected pattern", strcompare("GMSEC.*.*.MSG.LOG.+", pattern) == 0);
			}
			else
			{
				testCheckBool("Unexpected pattern", strcompare("GMSEC.*.*.MSG.HB.+", pattern) == 0);
			}
		}

		testCheckBool("Unexpected number of patterns", patterns == 2);

		const char* pattern = subscriptionEntryNextExcludedPattern(entry, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected pattern to be NULL", pattern == NULL);
	}

	// Off-nominal test(s)
	{
		// SubscriptionEntry is NULL
		const char* pattern = subscriptionEntryNextExcludedPattern(NULL, status);
		testCheckBool(statusGet(status), statusHasError(status) == GMSEC_TRUE);
		testCheckBool("Expected pattern to be NULL", pattern == NULL);
	}
}


int test_ConfigFile()
{
	testSetDescription("Test ConfigFile");

	test_configFileCreate();
	test_configFileCreateUsingConfig();
	test_configFileDestroy();
	test_configFileLoad();
	test_configFileSave();
	test_configFileSaveToFile();
	test_configFileFromXML();
	test_configFileToXML();
	test_configFileIsLoaded();
	test_configFileAddConfig();
	test_configFileRemoveConfig();
	test_configFileLookupConfig();
	test_configFileAddMessage();
	test_configFileLookupMessage();
	test_configFileRemoveMessage();
	test_configFileAddSubscriptionEntry();
	test_configFileLookupSubscriptionEntry();
	test_configFileRemoveSubscription();
	test_configFileGetIterator();

	test_subscriptionEntryCreate();
	test_subscriptionEntryGetName();
	test_subscriptionEntryGetPattern();
	test_subscriptionEntryAddExcludedPattern();
	test_subscriptionEntryHasNextExcludedPattern();
	test_subscriptionEntryNextExcludedPattern();

	return 0;
}
TEST_DRIVER(test_ConfigFile)
