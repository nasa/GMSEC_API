/*
 * Copyright 2007-2021 United States Government as represented by the
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
    StatusClass clazz;
    StatusCode  code;
    string      reason;
    int         custom;

    public GmsecException(string message)
        : base(message)
    {
        string regex = @"\[(?<clazz>\d+),(?<code>\d+),(?<custom>\d+)\]";

        Regex r = new Regex(regex, RegexOptions.None, TimeSpan.FromMilliseconds(150));
        Match m = r.Match(message);

        if (m.Success)
        {
            clazz  = (StatusClass) Int32.Parse(m.Result("${clazz}"));
            code   = (StatusCode) Int32.Parse(m.Result("${code}"));
            custom = Int32.Parse(m.Result("${custom}"));
            reason = message.Substring(message.IndexOf(":") + 2);
        }
    }

    public GmsecException(StatusClass clazz, StatusCode code, string reason, int custom)
        : base(BuildMessage(clazz, code, reason, custom))
    {
        this.clazz  = clazz;
        this.code   = code;
        this.reason = reason;
        this.custom = custom;
    }

    public GmsecException(Status status)
        : base(BuildMessage(status.GetClass(), status.GetCode(), status.GetReason(), status.GetCustomCode()))
    {
        this.clazz  = status.GetClass();
        this.code   = status.GetCode();
        this.reason = status.GetReason();
        this.custom = status.GetCustomCode();
    }

    public GmsecException(GmsecException other)
        : base(BuildMessage(other.clazz, other.code, other.reason, other.custom))
    {
        this.clazz  = other.clazz;
        this.code   = other.code;
        this.reason = other.reason;
        this.custom = other.custom;
    }

    public StatusClass GetErrorClass()
    {
        return clazz;
    }

    public StatusCode GetErrorCode()
    {
        return code;
    }

    public string GetErrorMessage()
    {
        return reason;
    }

    public int GetCustomCode()
    {
        return custom;
    }

    private static string BuildMessage(StatusClass clazz, StatusCode code, string reason, int custom)
    {
        StringBuilder sb = new StringBuilder();

        sb.Append("[").Append(clazz).Append(",").Append(code).Append(",").Append(custom).Append("]: ").Append(reason);

        return sb.ToString();
    }
}

public class%}

%exceptionclass gmsec::api::Exception;

%exception {
    try
    {
        $action
    }
    catch (const gmsec::api::Exception& e)
    {
        SWIG_CSharpSetPendingExceptionCustom(e.what());
    } 
}
