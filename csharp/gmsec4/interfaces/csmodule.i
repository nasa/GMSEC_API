%{
class GMSEC_BinaryData
{
public:
    GMSEC_BinaryData() : data(0), size(0) {}

    GMSEC_BinaryData(unsigned char* d, int s) : data(d), size(s) {}

    unsigned char* data;
    int size;
};
%}


%insert(runtime) %{
    /* Callback for returning byte arrays to C#, the resulting array is not 
    marshaled back, so the caller should call GetBytes to get back the results */ 
    typedef void (SWIGSTDCALL* SWIG_CSharpByteArrayHelperCallback)(const unsigned char *, const int);
    static SWIG_CSharpByteArrayHelperCallback SWIG_csharp_bytearray_callback = NULL;
%}

%pragma(csharp) imclasscode=%{
    internal class SWIGByteArrayHelper : System.MarshalByRefObject {

        public delegate void SWIGByteArrayDelegate(global::System.IntPtr data, int size);
        static SWIGByteArrayDelegate bytearrayDelegate = new SWIGByteArrayDelegate(CreateByteArray);

        [global::System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="SWIGRegisterByteArrayCallback_$module")]
        public static extern void SWIGRegisterByteArrayCallback_Gmsec(SWIGByteArrayDelegate bytearrayDelegate);

        private static byte[] arraybuffer;

        static SWIGByteArrayHelper() {
            SWIGRegisterByteArrayCallback_$module(bytearrayDelegate);
        }

        static void CreateByteArray(global::System.IntPtr data, int size) {
            if (size == 0) return;
            arraybuffer = new byte[ size ];
            global::System.Runtime.InteropServices.Marshal.Copy(data, arraybuffer, 0, size);
        }

        public static byte[] GetBytes() {
            byte[] a = arraybuffer;
            arraybuffer = null;
            return a;
        }
    }

    public static readonly object binValueLock = new object();
%}

%insert(runtime) %{
#ifdef __cplusplus
    extern "C"
#endif
SWIGEXPORT void SWIGSTDCALL SWIGRegisterByteArrayCallback_$module(SWIG_CSharpByteArrayHelperCallback callback) {
    SWIG_csharp_bytearray_callback = callback;
}
%}

%typemap(out) GMSEC_BinaryData %{ SWIG_csharp_bytearray_callback($1.data, $1.size); %}

%typemap(csout) GMSEC_BinaryData {
    /* No longer needed, however the code is kept just in case it ever needs to be used again.
    object lockObj = System.AppDomain.CurrentDomain.GetData("GMSEC-LOCKOBJ");

    if (lockObj == null) {
        lockObj = $modulePINVOKE.binValueLock;
    }

    lock(lockObj) {
    */

    lock($modulePINVOKE.binValueLock) {
        new $modulePINVOKE.SWIGByteArrayHelper();
        $imcall;
        if ($modulePINVOKE.SWIGPendingException.Pending) throw $modulePINVOKE.SWIGPendingException.Retrieve();
        return $modulePINVOKE.SWIGByteArrayHelper.GetBytes();
    }
}
