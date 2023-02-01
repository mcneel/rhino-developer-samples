using System;
using System.IO;
using System.Reflection;

using Rhino.Runtime.InProcess;
using Rhino.Geometry;

namespace HelloWorld
{
  class Startup
  {
    static Startup()
    {
      RhinoInside.Resolver.Initialize();
    }

    [System.STAThread]
    static void Main(string[] args)
    {
      Program.Run(args);
    }
  }

  class Program
  {
    public static void Run(string[] args)
    {
      try
      {
        using (new RhinoCore(args))
        {
          MeshABrep();
          Console.WriteLine("press any key to exit");
          Console.ReadKey();
        }
      }
      catch (Exception ex)
      {
        Console.Error.WriteLine(ex.Message);
      }
    }

    static void MeshABrep()
    {
      var sphere = new Sphere(Point3d.Origin, 12);
      var brep = sphere.ToBrep();
      var mp = new MeshingParameters(0.5);
      var mesh = Mesh.CreateFromBrep(brep, mp);
      Console.WriteLine($"Mesh with {mesh[0].Vertices.Count} vertices created");
    }
  }
}
