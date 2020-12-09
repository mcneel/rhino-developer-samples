using System;
using System.IO;
using System.Reflection;

using Rhino.Runtime.InProcess;
using Rhino.Geometry;
using Rhino;

namespace HelloWorld
{
  class Program
  {
    #region Program static constructor
    static Program()
    {
      RhinoInside.Resolver.Initialize();
    }
    #endregion

    [System.STAThread]
    static void Main(string[] args)
    {
      Console.WriteLine("Please wait while Rhino is starting...");  
      try
      {
        using (new RhinoCore(new string[] { "/NOSPLASH" }, WindowStyle.Hidden ))
        {

          Console.WriteLine("Enter path to directory which contains files to convert and press ENTER:");
          string path = Console.ReadLine();

          string[] filePaths = null;

          try
          {
            filePaths = System.IO.Directory.GetFiles(path, "*.*", System.IO.SearchOption.AllDirectories);
          }
          catch (Exception e)
          {
            Console.WriteLine("{0}, Press any key to exit...",e.Message);
            Console.ReadKey();
            return;
          }

          if (filePaths == null || filePaths.Length == 0)
          {
            Console.WriteLine("Directory is empty. No files to process. Press any key to exit.");
            Console.ReadKey();
            return;
          }
          
          foreach (string file in filePaths)
          {

            var doc = RhinoDoc.Open(file, out bool wasOpen);

            // Change the original extension to .obj
            var objPath = System.IO.Path.ChangeExtension(file, ".obj");

            // Change the original extension to .png
            var pngPath = System.IO.Path.ChangeExtension(file, ".png");
            var imgScript = string.Format("_-ViewCaptureToFile \"{0}\" _Enter", pngPath);
            RhinoApp.RunScript(imgScript, false);

            // Export options:
            var fowo = new Rhino.FileIO.FileObjWriteOptions(new Rhino.FileIO.FileWriteOptions())
            {
              ExportMaterialDefinitions = false,
              MapZtoY = true,
              MeshParameters = MeshingParameters.Default
            };

            // Save the .obj
            Console.WriteLine("Nº of objects in file: {0}", doc.Objects.Count);

            var result = Rhino.FileIO.FileObj.Write(objPath, doc, fowo);

            if(result == Rhino.PlugIns.WriteFileResult.Success)
              Console.WriteLine("Converted file: {0}", objPath);
            else
              Console.WriteLine("File conversion failed.");

          }

          Console.WriteLine("Finished converting. Press any key to exit...");
          Console.ReadKey();
        }
      }
      catch (Exception ex)
      {
        Console.Error.WriteLine(ex.Message);
        Console.WriteLine("press any key to exit");
        Console.ReadKey();
      }
    }
  }
}
