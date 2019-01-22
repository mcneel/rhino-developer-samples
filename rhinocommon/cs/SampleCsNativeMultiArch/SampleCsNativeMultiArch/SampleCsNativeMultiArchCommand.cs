using System;
using Rhino;
using Rhino.Commands;
using System.Runtime.InteropServices;

namespace SampleCsNativeMultiArch
{
  public class SampleCsNativeMultiArchCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "SampleCsNativeMultiArchCommand"; }
    }

    protected override Result RunCommand(Rhino.RhinoDoc doc, RunMode mode)
    {
      string rhino_arch = IntPtr.Size == 8 ? "64" : "32";
      RhinoApp.WriteLine("This plug-in is running in {0}-bit Rhino", rhino_arch);

      IntPtr version;
      try
      {
        version = UnsafeNativeMethods.curl_version();
      }
      catch (Exception e)
      {
        RhinoApp.WriteLine("ERROR: " + e.Message);
        RhinoApp.WriteLine("Couldn't load libcurl");
        return Result.Failure;
      }

      RhinoApp.WriteLine("Successfully loaded libcurl, version: " + Marshal.PtrToStringAnsi(version));
      return Result.Success;
    }
  }
}
