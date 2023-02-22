#ifndef TESTCASE_H
#define TESTCASE_H

#include <gmsec5_cpp.h>

#include <gmsec5/internal/StringUtil.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <csignal>    // for signal()
#include <cstdlib>    // for exit()
#include <cctype>

#include <string.h>

#ifdef WIN32
	#include <windows.h>
	#include <process.h>

	#ifdef NO_ERROR
		#undef NO_ERROR
	#endif
#else
	#include <unistd.h>
#endif


#define TEST_DRIVER(fname) \
int main(int argc, char **argv) { \
	return testDriver(argc, argv, &fname); \
}


class TestException : public std::exception
{
public:
	TestException(const char *s) : text(s) {
	}

	TestException(const std::string &s) : text(s) {
	}

	virtual ~TestException() throw() {
	}

	virtual const char *what() const throw() {
		return text.c_str();
	}

private:
	std::string text;
};


class Test
{
public:
	Test(int argc, char** argv)
		: m_config(argc, argv),
		  m_nOk(0),
		  m_nBad(0)
	{
		setPrefix(argv[0]);

		for (int i = 1; i < argc; ++i)
		{
			const char* arg = argv[i];
			if (arg[0] == '-')
			{
				GMSEC_VERBOSE << "driver argument: " << arg;
				std::string s(arg);
				static std::string o_signal("-signal=");
				if (s.find(o_signal) == 0)
				{
					m_signal = s.substr(o_signal.length());
				}
				else
				{
					std::cout << "ignoring " << s << std::endl;
				}
			}
			else
			{
				GMSEC_DEBUG << "test argument: " << arg;
			}
		}

		const char* logLevel = m_config.getValue("LOGLEVEL");
		const char* logFile  = m_config.getValue("LOGFILE");

		if (!logLevel)
		{
			m_config.addValue("LOGLEVEL", "INFO");
		}

		if (!logFile)
		{
			m_config.addValue("LOGFILE", "STDOUT");
		}
	}


	~Test()
	{
	}


	gmsec::api5::Config& getConfig()
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

	/* check(text, flag)
	 * Log a message if flag is not true
	 */
	bool check(const char* what, bool flag)
	{
		return _check(what, flag, true, 0);
	}


	/* check(text, status)
	* Log a message if status is not valid
	*/
	bool check(const char* what, const gmsec::api5::Status& status)
	{
		const char* extra = (status.hasError() ? status.get() : 0);

		return _check(what, !status.hasError(), true, extra);
	}


	/* require(text, int)
	* Log a message and throw if flag is not true
	*/
	bool require(const char* what, int flag)
	{
		return _check(what, (flag != 0 ? true : false), false, 0);
	}


	/* require(text, flag)
	* Log a message and throw if flag is not true
	*/
	bool require(const char* what, bool flag)
	{
		return _check(what, flag, false, 0);
	}


	/* require(text, status)
	* Log a message and throw if status is not valid
	*/
	bool require(const char* what, const gmsec::api5::Status& status)
	{
		const char* extra = (status.hasError() ? status.get() : 0);

		return _check(what, !status.hasError(), false, extra);
	}


	std::string getSubject() const
	{
		return m_prefix;
	}


	std::string getSubject(const char* tag) const
	{
		return getSubject(std::string(tag));
	}


	std::string getSubject(const std::string& tag) const
	{
		std::string subject = m_prefix;
		std::string compliantTag = makeCompliant(tag);
		subject += ".";
		subject += compliantTag;
		return subject;
	}


	std::string getUniqueComponent()
	{
		std::string component = m_prefix;
		size_t i = 0;
		while ((i = component.find(".", i)) != std::string::npos)
		{
			component = component.replace(i, 1, "-");
		}
		return component;
	}


	void start()
	{
 		//
		// Same as finish(), but only put out initial signal file with
		// a valid description and that's all.  Meant to be called from
		// setDescription() so presumably it will be called at the start
		// of every unit test.
		//
		std::ostringstream os;

		os << "DESCRIPTION=" << getDescription() << std::endl;

		if (m_signal.length() > 0)
		{
			GMSEC_VERBOSE << "Test.start: signal=" << m_signal.c_str();
			std::ofstream osf(m_signal.c_str());
			osf << os.str() << '\n';
		}
	}


	void finish()
	{
		std::ostringstream os;
		const char* result = "FAIL";
		if (m_nBad == 0 && m_nOk > 0)
		{
			result = "PASS";
		}
		if (!m_reason.empty())
		{
			result = "EXCUSE";
		}

		os << "RESULT=" << result
		   << "\nDESCRIPTION=" << getDescription()
		   << "\nN_OK=" << m_nOk
		   << "\nN_BAD=" << m_nBad
		   << "\nPREFIX=" << m_prefix;

		if (!m_reason.empty())
		{
			os << "\nEXCUSE=" << m_reason;
		}

		GMSEC_INFO << "summary:\n" << os.str().c_str();

		if (m_signal.length() > 0)
		{
			GMSEC_VERBOSE << "Test.finish: signal=" << m_signal.c_str();
			std::ofstream osf(m_signal.c_str());
			osf << os.str() << '\n';
		}
	}


	void crashed()
	{
		++m_nBad;
		finish();
	}


	bool allowMiddleware(const std::vector<const char*>& allowedMiddleware)
	{
		bool        mw_allowed = false;
		const char* mw = getConfig().getValue("MW-ID");

		if (mw)
		{
			for (size_t i = 0; !mw_allowed && i < allowedMiddleware.size(); ++i)
			{
				mw_allowed = (std::string(mw).find(allowedMiddleware[i]) != std::string::npos);
			}

			if (!mw_allowed)
			{
				std::string msg = std::string("Test is being excused for ") + mw + ".";
				excuse(msg.c_str());
			}
		}
		else
		{
			excuse("Test excused; mw-id not defined in configuration.");
		}

		return mw_allowed;
	}


	void excuse(const char* s)
	{
		m_reason = s;
	}


	std::string getDataFile(const char* name)
	{
		std::string dir;

		const char* addonDir = getConfig().getValue("addons");
		if (addonDir)
		{
			dir = addonDir;
		}
		else
		{
			// default to argv[0]/../src/addons;
			dir = m_directory + "/../src/addons";
			GMSEC_VERBOSE << "unable to retrieve addons directory. defaulting to src/addons";
		}

		return dir + "/" + name;
	}


	std::string getDirectory() const
	{
		return m_directory;
	}


	void setDescription(const std::string& desc)
	{
		m_description = desc;
		start();
	}


	std::string getDescription() const
	{
		return m_description.empty() ? "Unknown C++ Unit Test" : m_description;
	}


	std::string getHostname() const
	{
		std::string hostname = "HOST";

		char tmp[256] = {0};

#ifdef WIN32
		DWORD nsize = sizeof(tmp);
		if (GetComputerName(tmp, &nsize) != 0)
		{
			hostname = tmp;
		}
#else
		if (gethostname(tmp, sizeof(tmp)) == 0)
		{
			hostname = tmp;

			// Ensure short host name (as opposed to fully-qualified hostname) is returned
			hostname = hostname.substr(0, hostname.find('.'));
		}
#endif

		return hostname;
	}


	int getProcessID() const
	{
		int pid;
#ifdef WIN32
		pid = (int) _getpid();
#else
		pid = (int) getpid();
#endif
		return pid;
	}


private:
	bool _check(const char* what, bool flag, bool noThrow, const char* info)
	{
		GMSEC_DEBUG << "check '" << what << "'";
		if (flag)
		{
			++m_nOk;
		}
		else
		{
			++m_nBad;
			if (info)
			{
				GMSEC_WARNING << "'" << what << "' failed: " << info;
			}
			else
			{
				GMSEC_WARNING << "'" << what << "' failed";
			}
			if (!noThrow)
			{
				throw TestException(what);
			}
		}
		return flag;
	}


	void setPrefix(const char* s)
	{
		m_directory = ".";
		std::string unitTest(s);

		{
			size_t pos = unitTest.rfind('/');

			if (pos != std::string::npos)
			{
				m_directory = unitTest.substr(0, pos);
				unitTest    = unitTest.substr(pos+1);
			}
			else
			{
				pos = unitTest.rfind('\\');

				if (pos != std::string::npos)
				{
					m_directory = unitTest.substr(0, pos);
					unitTest    = unitTest.substr(pos+1);
				}
			}
		}

		std::ostringstream os;

		os << "NIGHTRUN.CXX." << getProcessID() << "_";

		// add hostname
		std::string host = getHostname();

		for (const char* p = host.c_str(); *p; ++p)
		{
			char c = *p;
			if (c == '.')
			{
				os << '_';
			}
			else
			{
				os << char(toupper(c));
			}
		}

		os << ".T";

		for (const char* p = unitTest.c_str() + 1; *p; ++p)
		{
			char c = *p;

			if (isdigit(c))
			{
				os << c;
			}
			else
			{
				break;
			}
		}

		m_prefix = os.str();
	}


	std::string makeCompliant(const std::string& tag) const
	{
		const char PERIOD     = '.';
		const char UNDERSCORE = '_';
		const char DASH       = '-';
		const char GREATER    = '>';
		const char WILDCARD   = '*';
		const char PLUS       = '+';

		std::string tmp = tag;
		bool compliant = true;

		for (size_t i = 0; i < tmp.length(); ++i)
		{
			char& ch = tmp.at(i);

			if (isalnum(ch))
			{
				if (isalpha(ch) && islower(ch))
				{
					compliant = false;
					ch = toupper(ch);
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
						compliant = false;
						ch = UNDERSCORE;
						break;
				}
			}
		}

		if (!compliant)
		{
			GMSEC_DEBUG << "The subject tag '" << tag.c_str() << "' is not GMSEC compliant; it has been fixed.";
		}

		return tmp;
	}


	// Member data
	gmsec::api5::Config m_config;
	unsigned int        m_nOk;
	unsigned int        m_nBad;
	std::string         m_directory;
	std::string         m_prefix;
	std::string         m_signal;
	std::string         m_reason;
	std::string         m_description;
};


static Test* testHandler = 0;


extern "C" void sigHandler(int signo)
{
	GMSEC_ERROR << "sigHandler was called with signal = " << signo;
	if (testHandler != 0)
	{
		testHandler->crashed();
		gmsec::api5::Connection::shutdownAllMiddlewares();
	}
	std::exit(signo);
}


static int testDriver(int argc, char** argv, int (*pf)(Test &))
{
	GMSEC_DEBUG << "testDriver: argc=" << argc << ", argv=" << argv << ", pfunction=" << (void*) pf;

	// Setup a signal handler to catch forced termination should the test
	// driver time-out or other anomaly occur.
	/*DMW
	std::signal(SIGABRT, sigHandler);
	std::signal(SIGFPE,  sigHandler);
	std::signal(SIGILL,  sigHandler);
	std::signal(SIGINT,  sigHandler);
	std::signal(SIGSEGV, sigHandler);
	std::signal(SIGTERM, sigHandler);
	*/

	Test test(argc, argv);
	testHandler = &test;

	int code = 0;

	try
	{
		code = (*pf)(test);
	}
	catch (std::exception& e)
	{
		code = -1;
		GMSEC_ERROR << "caught: " << e.what();
	}

	test.finish();

	// Remove signal handlers for non-fatal issues 
	/*DMW
	std::signal(SIGINT, NULL);
	std::signal(SIGTERM, NULL);
	*/

	gmsec::api5::Connection::shutdownAllMiddlewares();

	GMSEC_VERBOSE << "testDriver: code=" << code;

	return code;
}


int printToString(char* destination, const char* format, ...)
{
	std::va_list args;
	va_start(args, format);
#ifdef WIN32
	int len    = _vscprintf(format, args) + 1;   // an extra byte for the terminating null-byte
	int result = vsprintf_s(destination, len, format, args);
#else
	int result = std::vsprintf(destination, format, args);
#endif
	va_end(args);
	return result;
}


bool strcontains(const char* str, const char* pattern)
{
	if (str == NULL)
		throw TestException("String cannot be NULL");

	if (pattern == NULL)
		throw TestException("Pattern string cannot be NULL");

	return std::string(str).find(pattern) != std::string::npos;
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


bool strEqualsIgnoreCase(const char* s1, const char* s2)
{
#if defined (WIN32)
    return _stricmp(s1, s2) == 0;
#else
    return strcasecmp(s1, s2) == 0;
#endif
}


size_t strlength(const char* str)
{
	std::string s(str);
	return s.length();
}	


void strcopy(char* s1, const char* s2)
{
	while ((*s1++ = *s2++) != 0) ;
}


char* strduplicate(const std::string& str)
{
	char* dup = new char[str.length() + 1];
	strcopy(dup, str.c_str());
	dup[str.length()] = '\0';
	return dup;
}


// Determine if specified Operating System is in use
bool checkForOS(const std::string& os)
{
	std::string tmpStr = os;
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), tolower);

	if (tmpStr.compare("windows") == 0 || tmpStr.compare("win32") == 0 )
	{
#ifdef WIN32
		return true;
#endif
	}
	else if (tmpStr.compare("linux") == 0 || tmpStr.compare("rhel") == 0)
	{
#ifdef __linux__
		return true;
#endif
	}
	else if (tmpStr.compare("hpux") == 0 || tmpStr.compare("hp-unix") == 0)
	{
#ifdef __hppa
		return true;
#endif
	}
	else if (tmpStr.compare("solaris") == 0 || tmpStr.compare("sunos") == 0)
	{
#ifdef __sun
		return true;
#endif
	}

	return false;
}


struct FieldInfo
{
	const char*              name;
	gmsec::api5::Field::Type type;
	const char*              value;
};


FieldInfo testStandardFields[] =
{
	{ "MISSION-ID",       gmsec::api5::Field::Type::STRING, "MY-MISSION" },
	{ "CONSTELLATION-ID", gmsec::api5::Field::Type::STRING, "MY-CONSTELLATION" },
	{ "SAT-ID-PHYSICAL",  gmsec::api5::Field::Type::STRING, "MY-SAT-ID" },
	{ "SAT-ID-LOGICAL",   gmsec::api5::Field::Type::STRING, "MY-SAT-ID" },
	{ "FACILITY",         gmsec::api5::Field::Type::STRING, "MY-FACILITY" },
	{ "COMPONENT",        gmsec::api5::Field::Type::STRING, "MY-COMPONENT" },
	{ "DOMAIN1",          gmsec::api5::Field::Type::STRING, "MY-DOMAIN-1" },
	{ "DOMAIN2",          gmsec::api5::Field::Type::STRING, "MY-DOMAIN-2" },
};


void set_standard_fields(gmsec::api5::MessageFactory& msgFactory)
{
	gmsec::api5::util::List<gmsec::api5::Field*> fieldList;

	for (size_t i = 0; i < sizeof(testStandardFields) / sizeof(FieldInfo); ++i)
	{
		switch (testStandardFields[i].type)
		{
		case gmsec::api5::Field::Type::STRING:
			fieldList.push_back( new gmsec::api5::StringField(testStandardFields[i].name, testStandardFields[i].value, true) );
			break;

		default:
			std::cerr << "Unsupported StandardField Type" << std::endl;
			break;
		}
	}

	msgFactory.setStandardFields(fieldList);

	for (gmsec::api5::util::List<gmsec::api5::Field*>::iterator it = fieldList.begin(); it != fieldList.end(); ++it)
	{
		delete *it;
	}
}


void get_standard_fields(gmsec::api5::util::List<gmsec::api5::Field*>& standardFields)
{
	for (size_t i = 0; i < sizeof(testStandardFields) / sizeof(FieldInfo); ++i)
	{
		switch (testStandardFields[i].type)
		{
		case gmsec::api5::Field::Type::STRING:
		{
			gmsec::api5::Field* field = new gmsec::api5::StringField(testStandardFields[i].name, testStandardFields[i].value, true);
			standardFields.push_back(field);

			break;
		}
		default:
			std::cerr << "Unsupported StandardField Type" << std::endl;
			break;
		}
	}
}


void destroy_standard_fields(gmsec::api5::util::List<gmsec::api5::Field*>& standardFields)
{
	for (gmsec::api5::util::List<gmsec::api5::Field*>::iterator it = standardFields.begin(); it != standardFields.end(); ++it)
	{
		delete *it;
	}
	standardFields.clear();
}


void verify_standard_fields(Test& test, const gmsec::api5::Message& msg)
{
    for (size_t i = 0; i < sizeof(testStandardFields) / sizeof(FieldInfo); ++i)
    {
        test.require("Message is missing standard field", msg.hasField(testStandardFields[i].name));
        test.check("Message has unexpected standard field type", testStandardFields[i].type == msg.getField(testStandardFields[i].name)->getType());
        test.check("Message has unexpected standard field value", std::string(testStandardFields[i].value) == msg.getStringValue(testStandardFields[i].name));
        test.check("Message is not a header field", msg.getField(testStandardFields[i].name)->isHeader());
    }
}


#endif
