using System;
using System.IO;
using System.Collections.Generic;

using Rhino.Runtime.InProcess;

namespace Convert
{
    class Program
    {
        static Program()
        {
            RhinoInside.Resolver.Initialize();
            Console.WriteLine($"Loading Rhino @ {RhinoInside.Resolver.RhinoSystemDirectory}");
        }

        // Use STAThread for this app as we are starting Rhino in a mode that does actually
        // include user interface (we just never show the main window). This allows for things
        // like RhinoApp().RunScript to properly run.
        [System.STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("Please wait while Rhino is starting...");
            try
            {
                RhinoProgram.LoadAndConvert();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
                Console.WriteLine("press any key to exit");
                Console.ReadKey();
            }
        }
    }

    // Keep all rhinocommon access in a separate static class. Otherwise RhinoCommon.dll
    // needs to be loaded when this executable is being loaded and since Rhino location
    // is not known when loading this executable, dotnet fails to find RhinoCommon
    // Using this method, the Program class first intializes the assembly resolver and
    // calls into this class later causing runtime to search and attempt to load
    // RhinoCommon after the main executable and Rhino are both loaded.
    static class RhinoProgram
    {
        public static void LoadAndConvert()
        {
            using (new RhinoCore())
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

                RhinoProgram.Convert(filePaths);

                Console.WriteLine("Finished converting. Press any key to exit...");
                Console.ReadKey();
            }
        }

        static void Convert(IEnumerable<string> filePaths)
        {
            foreach (string file in filePaths)
            {
                var doc = Rhino.RhinoDoc.Open(file, out bool _);

                Console.WriteLine("Nº of objects in file: {0}", doc.Objects.Count);

                // View capture to .png file
                var pngPath = Path.ChangeExtension(file, ".png");
                var imgScript = string.Format("_-ViewCaptureToFile \"{0}\" _Enter", pngPath);
                Rhino.RhinoApp.RunScript(imgScript, false);

                // Save the .obj file
                var fowo = new Rhino.FileIO.FileObjWriteOptions(new Rhino.FileIO.FileWriteOptions { SuppressAllInput = true })
                {
                    ExportMaterialDefinitions = false,
                    MapZtoY = true,
                    MeshParameters = Rhino.Geometry.MeshingParameters.Default,
                };

                var objPath = Path.ChangeExtension(file, ".obj");
                var result = Rhino.FileIO.FileObj.Write(objPath, doc, fowo);

                if (result == Rhino.PlugIns.WriteFileResult.Success)
                    Console.WriteLine("Converted file: {0}", objPath);
                else
                    Console.WriteLine("File conversion failed.");
            }
        }
    }
}
