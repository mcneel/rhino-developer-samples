using System;
using System.IO;

using Rhino.Runtime.InProcess;
using Rhino.Geometry;
using Rhino;

namespace Convert
{
  class Program
  {
    #region Program static constructor
    static Program()
    {
      RhinoInside.Resolver.Initialize();
    }
    #endregion

    // Use STAThread for this app as we are starting Rhino in a mode that does actually
    // include user interface (we just never show the main window). This allows for things
    // like RhinoApp().RunScript to properly run.
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

          string[] filePaths = Directory.GetFiles(path, "*.3dm");
          if (filePaths == null || filePaths.Length == 0)
          {
            Console.WriteLine("Directory is empty. No files to process. Press any key to exit.");
            Console.ReadKey();
            return;
          }
          
          foreach (string file in filePaths)
          {
            var doc = RhinoDoc.Open(file, out bool wasOpen);

            Console.WriteLine("Nº of objects in file: {0}", doc.Objects.Count);

            // View capture to .png file
            var pngPath = Path.ChangeExtension(file, ".png");
            var imgScript = string.Format("_-ViewCaptureToFile \"{0}\" _Enter", pngPath);
            RhinoApp.RunScript(imgScript, false);

            // Save the .obj file
            var fowo = new Rhino.FileIO.FileObjWriteOptions(new Rhino.FileIO.FileWriteOptions { SuppressAllInput = true })
            {
              ExportMaterialDefinitions = false,
              MapZtoY = true,
              MeshParameters = MeshingParameters.Default,
            };

            var objPath = Path.ChangeExtension(file, ".obj");
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
