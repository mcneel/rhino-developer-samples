using System;
using System.IO;
using System.Runtime.InteropServices;

namespace SampleCsNativeMultiArch
{
  internal static class UnsafeNativeMethods
  {
    static UnsafeNativeMethods()
    {
      var myPath = new Uri(typeof(UnsafeNativeMethods).Assembly.CodeBase).LocalPath;
      var myFolder = Path.GetDirectoryName(myPath);

      var is64 = IntPtr.Size == 8;
      var subfolder = is64 ? "\\win64\\" : "\\win32\\";

      LoadLibrary(myFolder + subfolder + "libcurl.dll");
    }

    [DllImport("kernel32.dll")]
    private static extern IntPtr LoadLibrary(string dllToLoad);

    [DllImport("libcurl.dll", CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr curl_version();
  }
}
