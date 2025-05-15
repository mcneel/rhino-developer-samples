using Rhino.Geometry;
using Rhino.Runtime.InProcess;
using System;

namespace HelloWorld
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
                RhinoProgram.MeshABrep();
                Console.WriteLine("press any key to exit");
                Console.ReadKey();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
                Console.WriteLine("press any key to exit");
                Console.ReadKey();
            }
        }

        static class RhinoProgram
        {
            public static void MeshABrep()
            {
                using (new RhinoCore())
                {
                    var sphere = new Sphere(Point3d.Origin, 12);
                    var brep = sphere.ToBrep();
                    var mp = new MeshingParameters(0.5);
                    var mesh = Mesh.CreateFromBrep(brep, mp);
                    Console.WriteLine($"Mesh with {mesh[0].Vertices.Count} vertices created");
                }
            }
        }
    }
}