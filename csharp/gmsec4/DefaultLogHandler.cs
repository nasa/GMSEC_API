// Copyright 2007-2018 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// No copyright is claimed in the United States under Title 17, U.S. Code.
// All Rights Reserved.


namespace GMSEC.API
{
    using System;
    using System.Text;

    public class DefaultLogHandler : LogHandler
    {
        static public void Initialize()
        {
            Log.RegisterHandler(null);

            for (int i = 0; i < 7; ++i)
            {
                Log.RegisterHandler((LogLevel)(i), new DefaultLogHandler());
            }
        }

        override public void OnMessage(LogEntry entry)
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(TimeUtil.FormatTime(entry.time));
            sb.Append(" [").Append(Log.ToString(entry.level)).Append("]");
            sb.Append(" [").Append(entry.file).Append(":").Append(entry.line).Append("] ");
            sb.Append(entry.message).Append("\n");

            Console.WriteLine(sb.ToString().Replace("\n", "\n\t"));
        }
    }
}

