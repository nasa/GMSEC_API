/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.internal;

import gov.nasa.gsfc.gmsecapi.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

/**
 * @class CallbackLookup
 *
 * @brief Maps a subject to a callback, based on the syntax of the subject, taking
 * into consideration wildcards [*] and at-least-one-more [>].  For internal
 * API use only, DO NOT DIRECTLY ACCESS THIS CLASS.
 *
 */
public class CallbackLookup
{
	private ArrayList<Callback> m_callbacks = new ArrayList<Callback>();
	private HashMap<String, CallbackLookup> m_callbackLookups = new HashMap<String, CallbackLookup>();

	private static final String PREFIX_SYMBOL = ".";

	/**
	 * @brief Register a subject with a specific callback.
	 * @param subject subject being registered
	 * @param cb callback being registered
	 * @return Status
	 */
	public Status addCallback(String subject, Callback cb)
	{
		//..null checking
		Status result = new Status();
		if ((cb == null) || (subject == null))
		{
			result.Set(gmsecAPI.GMSEC_STATUS_CALLBACKLKP_ERROR, gmsecAPI.GMSEC_INVALID_CALLBACK, "Error in Java class CallbackLookup > Either the callback or the subject is null");
			return result;
		} //..if a null callback

		// The callbacks are stored in a HashMap.  For this map, a unique substring of the
		// subject name is used as the key, and the value is the actual callback handle.
		// This means, that we need to iterate through the suffixes of the subject name,
		// to determine at what point the suffix name is unique, and also handle the
		// wildcard cases.
		if (subject.equals(""))
		{
			if (!m_callbacks.contains(cb))
				m_callbacks.add(cb);
		}
		else
		{
			String first = new String(subject);
			String last = "";

			int t = subject.indexOf(".");
			if (t > 0)
			{
				first = subject.substring(0, t);
				last = subject.substring(t + 1);
			}

			CallbackLookup callbackList = (CallbackLookup)m_callbackLookups.get(first);
			if (callbackList == null)
			{
				callbackList = new CallbackLookup();
				m_callbackLookups.put(first, callbackList);
			}

			if (!first.equals(""))
			{
				callbackList.addCallback(last, cb);
			}
		} //..find insertion point into tree

		return result;
	} //..addCallback()

	/**
	 * @brief Notify callback handlers for the included subject.
	 * @param subject subject for dispatched message
	 * @param conn connection dispatching the message
	 * @param msg message being dispatched
	 * @return Status
	 */
	public Status DispatchMsg(GMSEC_String gmsec_subject, Connection conn, Message msg)
	{
		String subject = gmsec_subject.toString();

		//* if subject == "" then we dispatch
		if (subject.equals(""))
		{
			for (Iterator itr = m_callbacks.iterator(); itr.hasNext();)
			{
				Callback cb = (Callback)itr.next();
				cb.OnMessage(conn, msg);
			}
		}
		else
		{
			//* 2) get first level item "first.*" to save callback
			String first = new String(subject.toString());
			String last = "";

			int t = subject.indexOf(".");
			if (t > 0)
			{
				first = subject.substring(0, t);
				last = subject.substring(t + 1);
			}

			// 3) lookup "*" entries
			CallbackLookup lkps = (CallbackLookup)m_callbackLookups.get("*");
			if (null != lkps)
			{
				// dispatch as if this level matched
				lkps.DispatchMsg(new GMSEC_String(last), conn, msg);
			}

			// 4) lookup ">" entries
			lkps = (CallbackLookup)m_callbackLookups.get(">");
			if (null != lkps)
			{
				// dispatch as if the rest of the levels matched
				lkps.DispatchMsg(new GMSEC_String(""), conn, msg);
			}

			// 5) lookup existing CallbackLookup for first level
			lkps = (CallbackLookup)m_callbackLookups.get(first);
			if (null != lkps)
			{
				lkps.DispatchMsg(new GMSEC_String(last), conn, msg);
			}
		} //else
		return new Status();
	} //..DispatchMsg()

	public boolean IsSubscribed(String subject)
	{
		/* 1) if subject = "" then we clear cb's here */
		if (subject.equals(""))
		{
			if (m_callbacks.size() > 0)
				return true;
		}
		else
		{
			/* 2) get first level item "first.*" to save callback */
			String first = new String(subject);
			String last = "";

			int t = subject.indexOf(PREFIX_SYMBOL);
			if (t > 0)
			{
				first = subject.substring(0, t);
				last = subject.substring(t + 1);
			}
			/* 3) lookup existing CallbackLookup for first level */

			CallbackLookup callbackLookup = (CallbackLookup)m_callbackLookups.get(first);
			if (null != callbackLookup)
			{
				return callbackLookup.IsSubscribed(last);
			}
		} //..else

		return false;
	}

	public Status RemoveCallback(String subject, Callback cb)
	{
		Status result = new Status();

		/* 1) if subject = "" then we clear cb's here */
		if (subject.equals(""))
		{
			m_callbacks.remove(cb);
		}
		else
		{
			/* 2) get first level item "first.*" to save callback */
			String first = new String(subject);
			String last = "";

			int t = subject.indexOf(PREFIX_SYMBOL);
			if (t > 0)
			{
				first = subject.substring(0, t);
				last = subject.substring(t + 1);
			}
			/* 3) lookup existing CallbackLookup for first level */

			CallbackLookup callbackLookup = (CallbackLookup)m_callbackLookups.get(first);
			if (null != callbackLookup)
			{
				return callbackLookup.RemoveCallback(last, cb);
			}

			result.Set(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_INVALID_CALLBACK, "No callback registered.");

		} //..else

		return result;
	}

	/**
	 * @brief Removes a registered callback.
	 * @param subject subject for which callbacks are being removed
	 * @return Status
	 */
	public Status RemoveCallbacks(String subject)
	{
		Status result = new Status();

		/* 1) if subject = "" then we clear cb's here */
		if (subject.equals(""))
		{
			m_callbacks.clear();
		}
		else
		{
			/* 2) get first level item "first.*" to save callback */
			String first = new String(subject);
			String last = "";

			int t = subject.indexOf(PREFIX_SYMBOL);
			if (t > 0)
			{
				first = subject.substring(0, t);
				last = subject.substring(t + 1);
			}
			/* 3) lookup existing CallbackLookup for first level */

			CallbackLookup callbackLookup = (CallbackLookup)m_callbackLookups.get(first);
			if (null != callbackLookup)
			{
				callbackLookup.RemoveCallbacks(last);
			}
		} //..else

		return result;
	} //..removeCallback()
} //..class CallbackLookup
