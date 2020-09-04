%module Log

%{
#include <map>
#include <gmsec4/util/Log.h>
using namespace gmsec::api;
%}

%ignore gmsec::api::util::Log::registerHandler(GMSEC_LogHandler*);
%ignore gmsec::api::util::Log::registerHandler(GMSEC_LogLevel, GMSEC_LogHandler*);

%rename("SetReportingLevel") setReportingLevel;
%rename("GetReportingLevel") getReportingLevel;
%rename("RegisterHandler") registerHandler;
%rename("ToString") toString;
%rename("FromString") fromString;

%typemap(cscode) gmsec::api::util::Log %{

    public static void Error(string message,
                             [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                             [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        logHelper("ERROR", message, fileName, lineNumber);
    }


    public static void Secure(string message,
                              [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                              [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        logHelper("SECURE", message, fileName, lineNumber);
    }


    public static void Warning(string message,
                               [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                               [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        logHelper("WARNING", message, fileName, lineNumber);
    }


    public static void Info(string message,
                            [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        logHelper("INFO", message, fileName, lineNumber);
    }


    public static void Verbose(string message,
                               [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                               [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        logHelper("VERBOSE", message, fileName, lineNumber);
    }


    public static void Debug(string message,
                             [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                             [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        logHelper("DEBUG", message, fileName, lineNumber);
    }


    private static void logHelper(string level, string message, string fileName, int lineNumber)
    {
        System.Text.StringBuilder sb = new System.Text.StringBuilder();

        sb.Append(TimeUtil.FormatTime(TimeUtil.GetCurrentTime()));
        sb.Append(" [").Append(level).Append("]");
        sb.Append(" [").Append(fileName).Append(":").Append(lineNumber).Append("] ");
        sb.Append(message).Append("\n");

        System.Console.WriteLine(sb.ToString().Replace("\n", "\n\t"));
    }
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Log.h>

