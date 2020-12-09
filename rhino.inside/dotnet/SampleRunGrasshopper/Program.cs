using System;
using Rhino.Geometry;

namespace RunGrasshopper
{
  class Program
  {
    static Program()
    {
      RhinoInside.Resolver.Initialize();
    }

    static void Main(string[] args)
    {
      using (var core = new Rhino.Runtime.InProcess.RhinoCore())
      {
        RunHelper();
      }
    }

    // Currently need a separate RunHelper function so the .NET runtime won't attempt to load the
    // Grasshopper assembly until after RhinoCore has been created. This should be "fixable" in a
    // future version of the RhinoInside nuget package
    static void RunHelper()
    {
      // Extract definition to sample location as executable
      var assembly = typeof(Program).Assembly;
      string dir = System.IO.Path.GetDirectoryName(assembly.Location);
      string filePath = System.IO.Path.Combine(dir, "simple_def.gh");

      using (var resStream = assembly.GetManifestResourceStream("RunGrasshopper.simple_def.gh"))
      using (var outStream = new System.IO.FileStream(filePath, System.IO.FileMode.Create))
      {
        resStream.CopyTo(outStream);
      }

      // Start grasshopper in "headless" mode
      var pluginObject = Rhino.RhinoApp.GetPlugInObject("Grasshopper") as Grasshopper.Plugin.GH_RhinoScriptInterface;
      pluginObject.RunHeadless();

      var io = new Grasshopper.Kernel.GH_DocumentIO();
      if (!io.Open(filePath))
        Console.WriteLine("File loading failed.");
      else
      {
        var doc = io.Document;

        // Documents are typically only enabled when they are loaded
        // into the Grasshopper canvas. In this case we -may- want to
        // make sure our document is enabled before using it.
        doc.Enabled = true;

        foreach (var obj in doc.Objects)
          if (obj is Grasshopper.Kernel.IGH_Param param)
            if (param.NickName == "CollectMe")
            {
              param.CollectData();
              param.ComputeData();

              foreach (var item in param.VolatileData.AllData(true))
                if (item.CastTo(out Line line))
                  Console.WriteLine($"Got a line: {line:0.000}");
                else
                  Console.WriteLine($"Unexpected data of type: {item.TypeName}");

              break;
            }
      }

      Console.WriteLine("Done... press any key to exit");
      Console.ReadKey();
    }
  }
}
