
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

import java.util.Date;
import java.util.logging.*;
import java.util.regex.Pattern;
import java.text.SimpleDateFormat;


public class Log
{
	static final Level MONITOR = makeLevel("MONITOR", Level.WARNING, Level.INFO);
	static final Level ERROR = makeLevel("ERROR", Level.SEVERE, Level.WARNING);

	static {
		Logger logger = Logger.getLogger("");
		for (Handler handler : logger.getHandlers()) {
			Formatter formatter = handler.getFormatter();
			if (handler instanceof ConsoleHandler || formatter instanceof SimpleFormatter)
				handler.setFormatter(new LogFormatter());
		}
	}


	static class LogFormatter extends Formatter
	{
		private SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/DDD/HH:mm:ss.SSS");
		private Pattern pattern = Pattern.compile("[\\n\\r]+");
		private static final String PREFIX = "\n\t";

		public String format(LogRecord record) {
			Date date = new Date(record.getMillis());
			String content = record.getMessage();
			String meta = dateFormat.format(date) + ' ' + record.getLevel()
					+ ' ' + record.getLoggerName()
					;

			String[] lines = pattern.split(content);
			StringBuilder sb = new StringBuilder(content.length() + lines.length + 2);
			for (String line : lines)
				sb.append(PREFIX).append(line);
			sb.append("\n\n");

			return meta + sb.toString();
		}
	}

	public static Logger createSource (String id) {
		return Logger.getLogger("bolt." + id);
	}
	
	
	public static void exception (Logger logger, Exception e) {
		exception(logger, e, true);
	}
	
	
	public static void exception (Logger logger, Exception e, boolean stackTrace) {
		String message;
		if (stackTrace) {
			StringBuilder builder = new StringBuilder(1024);
			builder.append(e).append("\n");
			for (StackTraceElement ste : e.getStackTrace())
				builder.append(ste).append("\n");
			message = builder.toString();
		}
		else
			message = e.toString();
		log(Level.SEVERE, logger, message);
	}
	
	
	public static void error (Logger logger, String message) {
		log(Level.SEVERE, logger, message);
	}
	
	
	public static void warning (Logger logger, String message) {
		log(Level.WARNING, logger, message);
	}
	
	
	public static void monitor (Logger logger, String message) {
		log(MONITOR, logger, message);
	}
	
	
	public static void setup (Logger logger, String message) {
		log(Level.CONFIG, logger, message);
	}
	
	
	public static void info (Logger logger, String message) {
		log(Level.INFO, logger, message);
	}

	
	public static void detail (Logger logger, String message) {
		log(Level.FINER, logger, message);
	}
	
	public static void debug (Logger logger, String message) {
		log(Level.FINEST, logger, message);
	}
	

	static long last_ms = System.currentTimeMillis();
	public static void log (Level level, Logger logger, String message) {
		logger.log(level, message);
		if (false) {
			long now_ms = System.currentTimeMillis();
			long delta_ms = now_ms - last_ms;
			last_ms = now_ms;
			if (level.intValue() > Level.FINE.intValue())
				System.out.println("[dt=" + delta_ms + "] Source="
						+ logger.getName() + ": " + message);
		}
	}

	private static class Sublevel extends Level
	{
		Sublevel(String name, int value) {
			super(name, value);
		}
	}

	static private Level makeLevel(String name, Level p, Level q) {
		return new Sublevel(name, (p.intValue() + q.intValue()) / 2);
	}

}
