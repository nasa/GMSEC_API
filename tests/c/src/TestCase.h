#ifndef GMSEC5_TEST_C_H
#define GMSEC5_TEST_C_H

#include <gmsec5_c.h>

#include <stdio.h>
#include <string.h>
#include <signal.h>    // for signal()
#include <stdlib.h>    // for exit(), malloc(), and free()
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>

#ifndef WIN32
#include <unistd.h>
#else
#include <winsock2.h>
#include <process.h>
// This is not production level code; allow usage of strcat(), strcpy(), etc.
#pragma warning ( disable : 4996 )
#endif


#define OUT_STR_LEN 2048

//
// "Public" test functions
//
int          testDriver(int argc, char* argv[], int (*pf)());
void         testStaticInit(int argc, char** argv);
GMSEC_Config testGetConfig();
GMSEC_BOOL   testCheckBool(const char* what, GMSEC_BOOL flag);
GMSEC_BOOL   testCheckStatus(const char* what, const GMSEC_Status status);
GMSEC_BOOL   testRequireInt(const char* what, int flag);
GMSEC_BOOL   testRequireBool(const char* what, GMSEC_BOOL flag);
GMSEC_BOOL   testRequireStatus(const char* what, const GMSEC_Status status);
const char*  testGetSubject();
char*        testGetSubjectCharPtr(const char* tag, char* subject);
void         testStart();
void         testFinish();
void         testCrashed();
GMSEC_BOOL   testAllowMiddleware(const char* allowedMiddleware[], size_t size);
void         testExcuse(const char* s);
void         testCleanup();
void         testGetDataFile(const char* name, char* dir);
const char*  testGetDirectory();
void         testSetDescription(const char* desc);
const char*  testGetDescription();
void         stringToUpper(char* destStr, const char* srcStr);
void         stringToLower(char* destStr, const char* srcStr);

struct FieldInfo
{
	const char*     name;
	GMSEC_FieldType type;
	const char*     value;
};


struct FieldInfo testStandardFields[] =
{
    { "MISSION-ID",       GMSEC_STRING_TYPE, "MY-MISSION" },
    { "CONSTELLATION-ID", GMSEC_STRING_TYPE, "MY-CONSTELLATION" },
    { "SAT-ID-PHYSICAL",  GMSEC_STRING_TYPE, "MY-SAT-ID" },
    { "SAT-ID-LOGICAL",   GMSEC_STRING_TYPE, "MY-SAT-ID" },
    { "FACILITY",         GMSEC_STRING_TYPE, "MY-FACILITY" },
    { "COMPONENT",        GMSEC_STRING_TYPE, "MY-COMPONENT" },
    { "DOMAIN1",          GMSEC_STRING_TYPE, "MY-DOMAIN-1" },
    { "DOMAIN2",          GMSEC_STRING_TYPE, "MY-DOMAIN-2" },
};

void setStandardFields(GMSEC_MessageFactory factory);
void getStandardFields(GMSEC_Field* fields[], size_t* numFields);
void destroyStandardFields(GMSEC_Field fields[], size_t numFields);
void verifyStandardFields(GMSEC_Message msg);

//
// "Private" test functions
//
GMSEC_BOOL   _testCheck(const char* what, GMSEC_BOOL flag, GMSEC_BOOL noThrow, const char* info);
void         testSetPrefix(const char* s);
void         testMakeCompliant(const char* tag, char* compliantTag);


//
// Util functions
//
int        printToString(char* destination, const char* format, ...);
int        strcontains(const char* haystack, const char* needle);
int        strcompare(const char* s1, const char* s2);
size_t     strlength(const char* str);
void       strcopy(char* s1, const char* s2);
char*      strduplicate(const char* str, size_t length);
size_t     strfind(const char* haystack, const char* needle);
char*      substr(const char* str, size_t offset, size_t len, char* dest);
GMSEC_BOOL checkForOS(const char* os);


#define TEST_DRIVER(fname) \
int main(int argc, char* argv[]) { \
	return testDriver(argc, argv, &fname); \
}


//
// "Private" data
//
static GMSEC_Config m_config      = NULL;
static unsigned int m_nOk         = 0;
static unsigned int m_nBad        = 0;
static const char*  m_directory   = NULL;
static const char*  m_prefix      = NULL;
static const char*  m_signal      = NULL;
static const char*  m_reason      = NULL;
static const char*  m_description = NULL;


void testStaticInit(int argc, char** argv)
{
	char out_str[OUT_STR_LEN];

	int  i   = 0;

	const char* logLevel = NULL;
	const char* logFile  = NULL;

	m_config = configCreateWithArgs(argc, argv);
	m_nOk    = 0;
	m_nBad   = 0;

	testSetPrefix(argv[0]);

	for (i = 0; i < argc; i++)
	{
		const char* arg = argv[i];
		if (arg[0] == '-')
		{
			printToString(out_str, "driver argument: %s", arg);
			GMSEC_VERBOSE(out_str);

			if (strstr(arg, "-signal=") == NULL)
			{
				printToString(out_str, "ignoring %s\n", arg);
			}
			else
			{
				if (m_signal != NULL)
				{
					free((void* ) m_signal);
				}
				m_signal = strduplicate(arg+strlength("-signal="), strlength(arg+strlength("-signal=")));
			}
		}
		else
		{
			printToString(out_str, "test argument:  %s", arg);
			GMSEC_DEBUG(out_str);
		}
	}

	logLevel = configGetValue(m_config, "LOGLEVEL", NULL);
	logFile  = configGetValue(m_config, "LOGFILE", NULL);

	if (!logLevel)
	{
		configAddValue(m_config, "LOGLEVEL", "INFO", NULL);
	}

	if (!logFile)
	{
		configAddValue(m_config, "LOGFILE", "STDOUT", NULL);
	}
}


GMSEC_Config testGetConfig()
{
	return m_config;
}


/*
 * NOTE:
 * In _check the actual test argument is Logged using GMSEC_VERBOSE,
 * and this can cause the heap to run out of memory when a large
 * number of requires/checks are being performed within a single
 * test.
 */


/* testCheckBool(text, GMSEC_BOOL)
 * Log a message if flag is not true
 */
GMSEC_BOOL testCheckBool(const char* what, GMSEC_BOOL flag)
{
	return _testCheck(what, flag, GMSEC_TRUE, 0);
}


/* testCheckStatus(const char*, GMSEC_Status)
 * Log a message if status is not valid
 */
GMSEC_BOOL testCheckStatus(const char* what, const GMSEC_Status status)
{
	const char* extra = (statusGetClass(status) != GMSEC_NO_ERROR ? statusGet(status) : NULL);

	return _testCheck(what, statusGetClass(status) == GMSEC_NO_ERROR, GMSEC_TRUE, extra);
}


/* testRequireInt(const char*, int)
 * Log a message and throw if flag is not true
 */
GMSEC_BOOL testRequireInt(const char* what, int flag)
{
	return _testCheck(what, (flag != 0 ? GMSEC_TRUE : GMSEC_FALSE), GMSEC_FALSE, 0);
}


/* testRequireBool(const char*, GMSEC_BOOL)
 * Log a message and throw if flag is not true
 */
GMSEC_BOOL testRequireBool(const char* what, GMSEC_BOOL flag)
{
	return _testCheck(what, flag, GMSEC_FALSE, 0);
}


/* testRequireStatus(const char*, GMSEC_Status)
* Log a message and throw if status is not valid
*/
GMSEC_BOOL testRequireStatus(const char* what, const GMSEC_Status status)
{
	const char* extra = (statusGetClass(status) != GMSEC_NO_ERROR ? statusGet(status) : NULL);

	return _testCheck(what, statusGetClass(status) == GMSEC_NO_ERROR, GMSEC_FALSE, extra);
}


const char* testGetSubject()
{
	return m_prefix;
}


char* testGetSubjectCharPtr(const char* tag, char* subject)
{
	char compliant_tag[OUT_STR_LEN];

	testMakeCompliant(tag, compliant_tag);

	strcopy(subject, m_prefix);
	strcat(subject, ".");
	strcat(subject, compliant_tag);

	return subject;
}


char* testGetUniqueComponent(char* component)
{
	strcopy(component, m_prefix);

	char* i = strchr(component, '.');
	while (i)
	{
		*i = '-';
		i = strchr(i, '.');
	}

	return component;
}


void testStart()
{
	FILE* out_fp = NULL;

	//
	// Same as finish(), but only put out initial signal file with
	// a valid description and that's all.  Meant to be called from
	// setDescription() so presumably it will be called at the start
	// of every unit test.
	//
	char out_str[OUT_STR_LEN];
	char tmp_str[OUT_STR_LEN];

	printToString(tmp_str, "DESCRIPTION=%s\n", testGetDescription());

	if (m_signal != NULL)
	{
		if (strlength(m_signal) > 0)
		{
			printToString(out_str, "Test.start: signal=%s", m_signal);

			GMSEC_VERBOSE(out_str);

			out_fp = fopen(m_signal, "w");

			fputs(tmp_str, out_fp);

			fclose(out_fp);
		}
	}
}


void testFinish()
{
	char        log_str[OUT_STR_LEN];
	char        file_out_str[OUT_STR_LEN];
	const char* result = "FAIL";
	FILE*       out_fp = NULL;

	if (m_nBad == 0 && m_nOk > 0)
	{
		result = "PASS";
	}
	else if (m_reason != NULL)
	{
		if (strcompare(m_reason, "") != 0)
		{
			result = "EXCUSE";
		}
	}

	printToString(file_out_str,
	        "RESULT=%s\n"
	        "DESCRIPTION=%s\n"
	        "N_OK=%d\n"
	        "N_BAD=%d\n"
	        "PREFIX=%s\n",
	        result, testGetDescription(), m_nOk, m_nBad, m_prefix);

	if (strcompare(result, "EXCUSE") == 0)
	{
		strcat(file_out_str, "\nEXCUSE=");
		strcat(file_out_str, m_reason);
	}

	printToString(log_str, "summary:\n%s", file_out_str);
	GMSEC_INFO(log_str);

	if (m_signal != NULL)
	{
		if (strlength(m_signal) > 0)
		{
			printToString(log_str, "Test.finish: signal=%s", m_signal);
			GMSEC_VERBOSE(log_str);

			out_fp = fopen(m_signal, "w+");

			fputs(file_out_str, out_fp);

			fclose(out_fp);
		}
	}
}


void testCrashed()
{
	m_nBad++;
	testFinish();
	testCleanup();
	connectionShutdownAllMiddlewares();
}


GMSEC_BOOL testAllowMiddleware(const char* allowedMiddleware[], size_t size)
{
	GMSEC_BOOL  mw_allowed = GMSEC_FALSE;
	const char* mw         = configGetValue(m_config, "MW-ID", NULL);

	if (mw != NULL)
	{
		size_t i;

		for (i = 0; !mw_allowed && i < size; i++)
		{
			if (strcompare(allowedMiddleware[i], mw) == 0) // match!
			{
				mw_allowed = GMSEC_TRUE;
			}
		}

		if (!mw_allowed)
		{
			char msg[OUT_STR_LEN];

			printToString(msg, "Test is being excused for %s.", mw);

			testExcuse(msg);
		}
	}
	else
	{
		testExcuse("Test excused; mw-id not defined in configuration.");
	}

	return mw_allowed;
}


void testExcuse(const char* s)
{
	if (m_reason != NULL)
	{
		free((void* ) m_reason);
	}
	m_reason = strduplicate(s, strlength(s));
}


void testCleanup()
{
	if (m_config != NULL)
	{
		configDestroy(&m_config);
		m_config = NULL;
	}

	if (m_directory != NULL)
	{
		free((void* ) m_directory);
		m_directory = NULL;
	}

	if (m_prefix != NULL)
	{
		free((void* ) m_prefix);
		m_prefix = NULL;
	}

	if (m_signal != NULL)
	{
		free((void* ) m_signal);
		m_signal = NULL;
	}

	if (m_reason != NULL)
	{
		free((void* ) m_reason);
		m_reason = NULL;
	}

	if (m_description != NULL)
	{
		free((void* ) m_description);
		m_description = NULL;
	}
}


void testGetDataFile(const char* name, char* dir)
{
	const char* addonDir = configGetValue(m_config, "addons", NULL);

	strcopy(dir, "");

	if (addonDir != NULL)
	{
		strcopy(dir, addonDir);
	}
	else
	{
		if (m_directory != NULL)
		{
			printToString(dir, "%s%s", m_directory, "/../../c/src/addons");
		}
		else
		{
			printToString(dir, "%s", "../../GMSEC_API-5.x/tests/c/src/addons");
		}
	}

	strcat(dir, "/");
	strcat(dir, name);
}


const char* testGetDirectory()
{
	return m_directory;
}


void testSetDescription(const char* desc)
{
	if (m_description != NULL)
	{
		free((void* ) m_description);
	}
	m_description = strduplicate(desc, strlength(desc));
	testStart();
}


const char* testGetDescription()
{
	GMSEC_BOOL known = GMSEC_FALSE;
	if (m_description != NULL)
	{
		if (strcompare(m_description, "") != 0)
		{
			known = GMSEC_TRUE;
		}
	}

	return !known ? "Unknown C++ Unit Test" : m_description;
}


void stringToUpper(char* destStr, const char* srcStr)
{
	size_t index;

	for (index = 0; index < strlength(srcStr); index++)
	{
		if (srcStr[index] >= 'a' && srcStr[index] <= 'z')
		{
			destStr[index] = toupper(srcStr[index]);
		}
		else
		{
			destStr[index] = srcStr[index];
		}
	}
	destStr[index] = '\0';

}


void stringToLower(char* destStr, const char* srcStr)
{
	size_t index;

	for (index = 0; index < strlength(srcStr); index++)
	{
		if (srcStr[index] >= 'A' && srcStr[index] <= 'Z')
		{
			destStr[index] = tolower(srcStr[index]);
		}
		else
		{
			destStr[index] = srcStr[index];
		}
	}
	destStr[index] = '\0';

}


GMSEC_BOOL _testCheck(const char* what, GMSEC_BOOL flag, GMSEC_BOOL noThrow, const char* info)
{
	char out_str[OUT_STR_LEN];

	printToString(out_str, "check '%s'", what);
	GMSEC_DEBUG(out_str);

	if (flag == GMSEC_TRUE)
	{
		m_nOk++;
	}
	else
	{
		m_nBad++;

		if (what != NULL)
		{
			if (info != NULL)
			{
				printToString(out_str, "'%s' failed:  %s", what, info);
				GMSEC_WARNING(out_str);
			}
			else
			{
				printToString(out_str, "'%s' failed.", what);
				GMSEC_WARNING(out_str);
			}
		}

		if (noThrow == GMSEC_FALSE)
		{
			testFinish();
			testCleanup();
			connectionShutdownAllMiddlewares();
			exit(1);
		}
	}

	return flag;
}


void testSetPrefix(const char* s)
{
	GMSEC_BOOL path_divider_found = GMSEC_FALSE;
	GMSEC_BOOL underscore_found   = GMSEC_FALSE;
	size_t     index              = strlength(s)-1;

	char hostname_str[OUT_STR_LEN];
	char unit_test_number_str[OUT_STR_LEN];
	char out_str[OUT_STR_LEN];
	char tmp_directory[OUT_STR_LEN];

	//
	// Find the directory of the current running program path, which is expected
	// to be the input parameter "s" to this function.  Set that to m_directory.
	//

	strcopy(tmp_directory, s);

	while (!path_divider_found && index >= 0)
	{
		if (tmp_directory[index] == '/' || tmp_directory[index] == '\\')
		{
			path_divider_found = GMSEC_TRUE;
			tmp_directory[index] = '\0';
		}
		else
		{
			index--;
		}
	}

	if (m_directory != NULL)
	{
		free((void* ) m_directory);
	}
	m_directory = strduplicate(tmp_directory, strlength(tmp_directory));

	strcopy(unit_test_number_str, &(tmp_directory[index+1]));

	//
	// Find the name of the unit test, which should ordinarily be a 4-digit number.
	// Strip away the possible ".exe" at the end, if it is there.
	//
	index = strlength(unit_test_number_str)-1;
	while (!underscore_found && index >= 0)
	{
		if (unit_test_number_str[index] == '_')
		{
			underscore_found = GMSEC_TRUE;
			unit_test_number_str[index] = '\0';
		}
		else
		{
			index--;
		}
	}

	//
	// Construct a test "prefix" that can be used as a unique identifier of a test.
	// The strings that construct this prefix will be in the form of:
	// NIGHTRUN.CR4.H.`hostname`.PID`getpid`.UT_[unit_test_number]
	//

	gethostname(hostname_str, OUT_STR_LEN);

	for (index = 0; index < strlength(hostname_str); index++)
	{
		hostname_str[index] = toupper(hostname_str[index]);
	}

	printToString(out_str, "NIGHTRUN.C.%d_%s.%s",
#ifndef WIN32
		      getpid(),
#else
		      _getpid(),
#endif
	          hostname_str,
		      unit_test_number_str);

	if (m_prefix != NULL)
	{
		free((void* ) m_prefix);
	}
	m_prefix = strduplicate(out_str, strlength(out_str));

}


#define PERIOD     '.'
#define UNDERSCORE '_'
#define DASH       '-'
#define GREATER    '>'
#define WILDCARD   '*'
#define PLUS       '+'

void testMakeCompliant(const char* tag, char* compliantTag)
{
	GMSEC_BOOL compliant  = GMSEC_TRUE;

	char out_str[OUT_STR_LEN];
	size_t i;

	strcopy(compliantTag, tag);

	for (i = 0; i < strlength(compliantTag); ++i)
	{
		char ch = compliantTag[i];

		if (isalnum(ch))
		{
			if (isalpha(ch) && islower(ch))
			{
				compliant = GMSEC_FALSE;
				ch = toupper(ch);
				compliantTag[i] = ch;
			}
		}
		else
		{
			switch (ch)
			{
				case PERIOD:
				case UNDERSCORE:
				case DASH:
				case GREATER:
				case WILDCARD:
				case PLUS:
					// ok
					break;
				default:
					compliant = GMSEC_FALSE;
					ch = UNDERSCORE;
					break;
			}
		}
	}

	if (!compliant)
	{
		printToString(out_str, "The subject tag '%s' is not GMSEC compliant; it has been fixed.", tag);
		GMSEC_DEBUG(out_str);
	}

}


void sigHandler(int signo)
{
	char out_str[OUT_STR_LEN];

	printToString(out_str, "sigHandler was called with signal = %d", signo);

	GMSEC_ERROR(out_str);

	testCrashed();

	exit(signo);
}


int testDriver(int argc, char* argv[], int (*pf)())
{
	char out_str[512];
	char tmp_str[256];
	int  arg_count      = 0;
	int  code           = 0;

#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	printToString(out_str, "testDriver:  argc=%d.  argv contains ", argc);

	for (arg_count = 1; arg_count < argc; arg_count++) // argv[0] is program
	{
		strcat(out_str, argv[arg_count]);
		if (arg_count != argc-1)
		{
			strcat(out_str, ", ");
		}
		else
		{
			strcat(out_str, ".  ");
		}
	}

	printToString(tmp_str, "pfunction=%0X", (void* ) pf);
	strcat(out_str, tmp_str);

	GMSEC_DEBUG(out_str);

	//
	// Setup a signal handler to catch forced termination should the test
	// driver time-out or other anomaly occur.
	//
	/*DMW
	std::signal(SIGABRT, sigHandler);
	std::signal(SIGFPE,  sigHandler);
	std::signal(SIGILL,  sigHandler);
	std::signal(SIGINT,  sigHandler);
	std::signal(SIGSEGV, sigHandler);
	std::signal(SIGTERM, sigHandler);
	*/

	testStaticInit(argc, argv);

	code = (*pf)();

	testFinish();

	testCleanup();

	//
	// Remove signal handlers for non-fatal issues 
	//
	/*DMW
	std::signal(SIGINT, NULL);
	std::signal(SIGTERM, NULL);
	*/

	connectionShutdownAllMiddlewares();

	printToString(tmp_str, "testDriver:  code=%d", code);

	GMSEC_VERBOSE(tmp_str);

#ifdef WIN32
	WSACleanup();
#endif

	return code;
}


int vargsLength(const char* format, va_list pargs)
{
	va_list argcopy;
	va_copy(argcopy, pargs);
	int len = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return len;
}

int printToString(char* destination, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int len = vargsLength(format, args);
	int result = vsnprintf(destination, len+1, format, args);
	va_end(args);
	return result;
}


int strcontains(const char* haystack, const char* needle)
{
	return (strstr(haystack, needle) == NULL ? -1 : 0);
}


int strcompare(const char* s1, const char* s2)
{
	unsigned char uc1, uc2;
	/* Move s1 and s2 to the first differing characters 
	   in each string, or the ends of the strings if they
	   are identical.  */
	while (*s1 != '\0' && *s1 == *s2)
	{
		++s1;
		++s2;
	}

	/* Compare the characters as unsigned char and
	   return the difference.  */
	uc1 = (*(unsigned char *) s1);
	uc2 = (*(unsigned char *) s2);

	return ((uc1 < uc2) ? -1 : (uc1 > uc2));
}


size_t strlength(const char* str)
{
	assert(str != NULL);
	return strlen(str);
}


void strcopy(char* s1, const char* s2)
{
	while ((*s1++ = *s2++) != 0) ;
}


char* strduplicate(const char* str, size_t length)
{
	char* dup = (char* ) malloc(length + 1);
	strcopy(dup, str);
	dup[length] = '\0';
	return dup;
}


size_t strfind(const char* haystack, const char* needle)
{
	char* pos = strstr(haystack, needle);
	return (pos ? pos - haystack : -1);
}


char* substr(const char* str, size_t offset, size_t len, char* dest)
{
	size_t str_len = strlen(str);

	if (offset + len > str_len)
	{
		return NULL;
	}

	strncpy(dest, str + offset, len);
	dest[len] = (char) 0;

	return dest;
}


// Determine if specified Operating System is in use
GMSEC_BOOL checkForOS(const char* os)
{
	char   tmp_str[OUT_STR_LEN];
	size_t index;

	strcopy(tmp_str, os);

	for (index = 0; index < strlength(tmp_str); index++)
	{
		tmp_str[index] = tolower(tmp_str[index]);
	}

	if (strcompare(tmp_str, "windows") == 0 || strcompare(tmp_str, "win32") == 0 )
	{
#ifdef WIN32
		return GMSEC_TRUE;
#endif
	}
	else if (strcompare(tmp_str, "linux") == 0 || strcompare(tmp_str, "rhel") == 0)
	{
#ifdef __linux__
		return GMSEC_TRUE;
#endif
	}
	else if (strcompare(tmp_str, "hpux") == 0 || strcompare(tmp_str, "hp-unix") == 0)
	{
#ifdef __hppa
		return GMSEC_TRUE;
#endif
	}
	else if (strcompare(tmp_str, "solaris") == 0 || strcompare(tmp_str, "sunos") == 0)
	{
#ifdef __sun
		return GMSEC_TRUE;
#endif
	}

	return GMSEC_FALSE;
}


void setStandardFields(GMSEC_MessageFactory factory)
{
	const size_t numFields = sizeof(testStandardFields) / sizeof(struct FieldInfo);
	GMSEC_Field* fields = malloc( sizeof(GMSEC_Field) * numFields);
	size_t       i;

	GMSEC_Status status = statusCreate();

	for (i = 0; i < numFields; ++i)
	{
		switch (testStandardFields[i].type)
		{
		case GMSEC_STRING_TYPE:
			fields[i] = stringFieldCreate(testStandardFields[i].name, testStandardFields[i].value, GMSEC_TRUE, status);
			break;

		default:
			printf("Unsupported standard field type\n");
			break;
		}

		if (statusHasError(status) == GMSEC_TRUE)
		{
			printf("Error creating standard fields: %s\n", statusGet(status));
		}
	}

	messageFactorySetStandardFields(factory, fields, numFields, status);

	if (statusHasError(status) == GMSEC_TRUE)
	{
		printf("Error setting standard fields: %s\n", statusGet(status));
	}

	for (i = 0; i < numFields; ++i)
	{
		if (fields[i] != NULL) {
			fieldDestroy(&fields[i]);
		}
	}
	free(fields);

	statusDestroy(&status);
}


void getStandardFields(GMSEC_Field* fields[], size_t* numFields)
{
	size_t i;

	*numFields = sizeof(testStandardFields) / sizeof(struct FieldInfo);

	*fields = malloc(*numFields * sizeof(GMSEC_Field));

	for (i = 0; i < *numFields; ++i)
	{
		fields[i] = stringFieldCreate(testStandardFields[i].name, testStandardFields[i].value, GMSEC_TRUE, NULL);
	}
}


void destroyStandardFields(GMSEC_Field fields[], size_t numFields)
{
	size_t i;

	for (i = 0; i < numFields; ++i)
	{
		fieldDestroy(&fields[i]);
	}
}


void verifyStandardFields(GMSEC_Message msg)
{
	testRequireBool("Message is NULL!", msg != NULL);

	const size_t numFields = sizeof(testStandardFields) / sizeof(struct FieldInfo);
	size_t i;

	for (i = 0; i < numFields; ++i)
	{
		testRequireBool("Standard field is missing from Message", messageHasField(msg, testStandardFields[i].name, NULL) == GMSEC_TRUE);
		testCheckBool("Standard field unexpected type", messageGetFieldType(msg, testStandardFields[i].name, NULL) == testStandardFields[i].type);
	}
}


#endif
