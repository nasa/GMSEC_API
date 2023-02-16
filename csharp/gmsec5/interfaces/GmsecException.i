/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module GmsecException


%insert(runtime) %{
  // Code to handle throwing of C# CustomApplicationException from C/C++ code.
  // The equivalent delegate to the callback, CSharpExceptionCallback_t, is CustomExceptionDelegate
  // and the equivalent customExceptionCallback instance is customDelegate
  typedef void (SWIGSTDCALL* CSharpExceptionCallback_t)(const char *);
  CSharpExceptionCallback_t customExceptionCallback = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL CustomExceptionRegisterCallback(CSharpExceptionCallback_t customCallback) {
    customExceptionCallback = customCallback;
  }

  // Note that SWIG detects any method calls named starting with
  // SWIG_CSharpSetPendingException for warning 845
  static void SWIG_CSharpSetPendingExceptionCustom(const char *msg) {
    customExceptionCallback(msg);
  }
%}

%pragma(csharp) imclasscode=%{
  class CustomExceptionHelper {
    // C# delegate for the C/C++ customExceptionCallback
    public delegate void CustomExceptionDelegate(string message);
    static CustomExceptionDelegate customDelegate = new CustomExceptionDelegate(SetPendingCustomException);

    [global::System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="CustomExceptionRegisterCallback")]
    public static extern void CustomExceptionRegisterCallback(CustomExceptionDelegate customCallback);

    static void SetPendingCustomException(string message) {
      SWIGPendingException.Set(new GmsecException(message));
    }

    static CustomExceptionHelper() {
      CustomExceptionRegisterCallback(customDelegate);
    }
  }
#pragma warning disable CS0414
  static CustomExceptionHelper exceptionHelper = new CustomExceptionHelper();
#pragma warning restore CS0414
%}


%pragma(csharp) moduleclassmodifiers=%{
using System;
using System.Text;
using System.Text.RegularExpressions;

// Kept to preserve binary-compatibility with API 4.5
public class DefaultLogHandler
{
    public static void Initialize() {
        //no-op
    }
}

// Custom C# Exception
public class GmsecException : global::System.ApplicationException
{
    int    clazz;
    int    code;
    string reason;
    int    custom;

    public GmsecException(string message)
        : base(message)
    {
        string regex = @"\[(?<clazz>\d+),(?<code>\d+),(?<custom>\d+)\]";

        Regex r = new Regex(regex, RegexOptions.None, TimeSpan.FromMilliseconds(150));
        Match m = r.Match(message);

        if (m.Success)
        {
            clazz  = Int32.Parse(m.Result("${clazz}"));
            code   = Int32.Parse(m.Result("${code}"));
            custom = Int32.Parse(m.Result("${custom}"));
            reason = message.Substring(message.IndexOf(":") + 2);
        }
    }

    public GmsecException(int clazz, int code, int custom, string reason)
        : base(BuildMessage(clazz, code, custom, reason))
    {
        this.clazz  = clazz;
        this.code   = code;
        this.custom = custom;
        this.reason = reason;
    }

    public GmsecException(Status status)
        : base(BuildMessage(status.GetClass(), status.GetCode(), status.GetCustomCode(), status.GetReason()))
    {
        this.clazz  = status.GetClass();
        this.code   = status.GetCode();
        this.custom = status.GetCustomCode();
        this.reason = status.GetReason();
    }

    public GmsecException(GmsecException other)
        : base(BuildMessage(other.clazz, other.code, other.custom, other.reason))
    {
        this.clazz  = other.clazz;
        this.code   = other.code;
        this.custom = other.custom;
        this.reason = other.reason;
    }

    public int GetErrorClass()
    {
        return clazz;
    }

    public int GetErrorCode()
    {
        return code;
    }

    public int GetCustomCode()
    {
        return custom;
    }

    public string GetErrorMessage()
    {
        return reason;
    }

    private static string BuildMessage(int clazz, int code, int custom, string reason)
    {
        StringBuilder sb = new StringBuilder();

        sb.Append("[").Append(clazz).Append(",").Append(code).Append(",").Append(custom).Append("] : ").Append(reason);

        return sb.ToString();
    }
}

public class%}

%exceptionclass gmsec::api5::GmsecException;

%exception {
    try
    {
        $action
    }
    catch (const gmsec::api5::GmsecException& e)
    {
        SWIG_CSharpSetPendingExceptionCustom(e.what());
    } 
}
