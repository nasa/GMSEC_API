
%{
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char* data;
    int size;
} GMSEC_BinaryData;

#ifdef __cplusplus
}
#endif
%}


%insert(runtime) %{
    /* Callback for returning byte arrays to C#, the resulting array is not 
    marshaled back, so the caller should call GetBytes to get back the results */ 
    typedef void (SWIGSTDCALL* SWIG_CSharpByteArrayHelperCallback)(const unsigned char *, const int);
    static SWIG_CSharpByteArrayHelperCallback SWIG_csharp_bytearray_callback = NULL;
%}

%pragma(csharp) imclasscode=%{
    protected class SWIGByteArrayHelper {

        public delegate void SWIGByteArrayDelegate(global::System.IntPtr data, int size);
        static SWIGByteArrayDelegate bytearrayDelegate = new SWIGByteArrayDelegate(CreateByteArray);

        [global::System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="SWIGRegisterByteArrayCallback_$module")]
        public static extern void SWIGRegisterByteArrayCallback_Gmsec(SWIGByteArrayDelegate bytearrayDelegate);

        static void CreateByteArray(global::System.IntPtr data, int size) {
            arraybuffer = new byte [size];
            global::System.Runtime.InteropServices.Marshal.Copy(data, arraybuffer, 0, size);
        }

        static SWIGByteArrayHelper() {
            SWIGRegisterByteArrayCallback_$module(bytearrayDelegate);
        }
    }

    static SWIGByteArrayHelper bytearrayHelper = new SWIGByteArrayHelper();
    static byte[] arraybuffer;
    internal static byte[] GetBytes() { return arraybuffer; } 
%}

%insert(runtime) %{
#ifdef __cplusplus
    extern "C"
#endif
SWIGEXPORT void SWIGSTDCALL SWIGRegisterByteArrayCallback_$module(SWIG_CSharpByteArrayHelperCallback callback) {
    SWIG_csharp_bytearray_callback = callback;
}
%}

/*
%typemap(imtype) GMSEC_BIN %{byte[]%}
*/

/*
%typemap(cstype) GMSEC_BIN %{byte[]%}
*/

%typemap(out) GMSEC_BinaryData

%{ SWIG_csharp_bytearray_callback($1.data, $1.size); %}

%typemap(csout) GMSEC_BinaryData {
    $imcall;
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return $modulePINVOKE.GetBytes();
}

/*
%typemap(ctype) GMSEC_BIN %{GMSEC_BIN %}
*/

