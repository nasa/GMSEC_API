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
%rename("ToString") toString;
%rename("FromString") fromString;

%typemap(cscode) gmsec::api::util::Log %{

    class DefaultLogHandler : LogHandler
    {
        Mutex m_mutex;

        public DefaultLogHandler()
        {
            m_mutex = new Mutex();
        }

        override public void OnMessage(LogEntry entry)
        {
            m_mutex.EnterMutex();

            System.Text.StringBuilder sb = new System.Text.StringBuilder();

            sb.Append(TimeUtil.FormatTime(entry.time));
            sb.Append(" [").Append(Log.ToString(entry.level)).Append("]");
            sb.Append(" [").Append(entry.file).Append(":").Append(entry.line).Append("] ");
            sb.Append(entry.message).Append("\n");

            System.Console.Error.WriteLine(sb.ToString().Replace("\n", "\n\t"));

            m_mutex.LeaveMutex();
        }
    }


    static DefaultLogHandler defaultLogHandler = new DefaultLogHandler();

    static System.Collections.Generic.IDictionary<LogLevel, LogHandler> LogHandlers =
        new System.Collections.Generic.Dictionary<LogLevel, LogHandler>()
    {
        { LogLevel.logERROR,   defaultLogHandler },
        { LogLevel.logSECURE,  defaultLogHandler },
        { LogLevel.logWARNING, defaultLogHandler },
        { LogLevel.logINFO,    defaultLogHandler },
        { LogLevel.logVERBOSE, defaultLogHandler },
        { LogLevel.logDEBUG,   defaultLogHandler }
    };


    public static void RegisterHandler(LogHandler logHandler)
    {
        LogHandler handler = (logHandler == null ? defaultLogHandler : logHandler);

        LogHandlers[LogLevel.logERROR]   = handler;
        LogHandlers[LogLevel.logSECURE]  = handler;
        LogHandlers[LogLevel.logWARNING] = handler;
        LogHandlers[LogLevel.logINFO]    = handler;
        LogHandlers[LogLevel.logVERBOSE] = handler;
        LogHandlers[LogLevel.logDEBUG]   = handler;

        // Do NOT register handler with the Core API for logVERBOSE or logDEBUG;
        // log messages can originate from a middleware wrapper 'long' after the
        // LogHandler is garbage collected.
        for (int i = (int)LogLevel.logERROR; i < (int)LogLevel.logVERBOSE; ++i)
        {
            Log.registerHandler((LogLevel) i, handler);
        }
    }


    public static void RegisterHandler(LogLevel level, LogHandler logHandler)
    {
        LogHandler handler = (logHandler == null ? defaultLogHandler : logHandler);

        LogHandlers[level] = handler;

        // Do NOT register handler with the Core API for logVERBOSE or logDEBUG;
        // log messages can originate from a middleware wrapper 'long' after the
        // LogHandler is garbage collected.
        if (level != LogLevel.logVERBOSE && level != LogLevel.logDEBUG)
        {
            Log.registerHandler(level, handler);
        }
    }


    public static void Error(string message,
                             [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                             [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        if (LogLevel.logERROR > GetReportingLevel())
            return;

        LogEntry entry = MakeEntry(LogLevel.logERROR, fileName, lineNumber, message);
        LogHandlers[LogLevel.logERROR].OnMessage(entry);
    }


    public static void Secure(string message,
                              [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                              [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        if (LogLevel.logSECURE > GetReportingLevel())
            return;

        LogEntry entry = MakeEntry(LogLevel.logSECURE, fileName, lineNumber, message);
        LogHandlers[LogLevel.logSECURE].OnMessage(entry);
    }


    public static void Warning(string message,
                               [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                               [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        if (LogLevel.logWARNING > GetReportingLevel())
            return;

        LogEntry entry = MakeEntry(LogLevel.logWARNING, fileName, lineNumber, message);
        LogHandlers[LogLevel.logWARNING].OnMessage(entry);
    }


    public static void Info(string message,
                            [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        if (LogLevel.logINFO > GetReportingLevel())
            return;

        LogEntry entry = MakeEntry(LogLevel.logINFO, fileName, lineNumber, message);
        LogHandlers[LogLevel.logINFO].OnMessage(entry);
    }


    public static void Verbose(string message,
                               [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                               [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        if (LogLevel.logVERBOSE > GetReportingLevel())
            return;

        LogEntry entry = MakeEntry(LogLevel.logVERBOSE, fileName, lineNumber, message);
        LogHandlers[LogLevel.logVERBOSE].OnMessage(entry);
    }


    public static void Debug(string message,
                             [System.Runtime.CompilerServices.CallerFilePath] string fileName = "",
                             [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
    {
        if (LogLevel.logDEBUG > GetReportingLevel())
            return;

        LogEntry entry = MakeEntry(LogLevel.logDEBUG, fileName, lineNumber, message);
        LogHandlers[LogLevel.logDEBUG].OnMessage(entry);
    }


    private static LogEntry MakeEntry(LogLevel level, string fileName, int lineNumber, string message)
    {
        LogEntry entry = new LogEntry();
        entry.time     = TimeUtil.GetCurrentTime();
        entry.file     = System.IO.Path.GetFileName(fileName);
        entry.line     = lineNumber;
        entry.level    = level;
        entry.message  = message;
        return entry;
    }
%}

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/util/Log.h>

