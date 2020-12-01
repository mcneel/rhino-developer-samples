using System;
using System.Windows.Forms;

namespace WinFormsApp
{
  static class Program
  {
    /// <summary>The main entry point for the application.</summary>
    [STAThread]
    static void Main()
    {
      // NOTE: Make sure to adjust your project settings so this is compiled as a 64 bit
      //       application. Rhino.Inside will only run in 64 bit

      // Set up RhinoInside resolver at the very beginning of the application
      // This helps find and load Rhino assemblies while the program runs
      RhinoInside.Resolver.Initialize();

      // Used by McNeel devs to debug against internal debug builds
      // RhinoInside.Resolver.RhinoSystemDirectory = @"C:\dev\github\mcneel\rhino\src4\bin\Debug";

      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new MainForm());
    }
  }
}
