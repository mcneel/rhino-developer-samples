using System;

namespace EtoApp
{
  static class Program
  {
    [STAThread]
    static void Main()
    {
      RhinoInside.Resolver.Initialize();

      // Used by McNeel devs to debug against internal debug builds
      //RhinoInside.Resolver.RhinoSystemDirectory = @"C:\dev\github\mcneel\rhino\src4\bin\Debug";

      RunApplication();
    }

    static void RunApplication()
    {
      MainForm.Run();
    }
  }
}
