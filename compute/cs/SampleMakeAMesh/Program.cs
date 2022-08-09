using System;
using Rhino.Compute;

namespace MakeAMesh
{
    class Program
    {
        static void Main(string[] args)
        {

            ComputeServer.WebAddress = "http://localhost:5000/";
            // ComputeServer.ApiKey = "";

            // Use standard rhino3dm methods locally
            var sphere = new Rhino.Geometry.Sphere(Rhino.Geometry.Point3d.Origin, 12);
            var sphereAsBrep = sphere.ToBrep();

            // The following function calls your Rhino.Compute server to get access to something not
            // available in Rhino3dm
            var meshes = MeshCompute.CreateFromBrep(sphereAsBrep);

            // Back to regular rhino3dm local calls
            Console.WriteLine($"Got {meshes.Length} meshes");
            for (int i = 0; i < meshes.Length; i++)
            {
                Console.WriteLine($"  {i + 1} mesh has {meshes[i].Vertices.Count} vertices");
            }

            Console.WriteLine("Press any key to exit");
            Console.ReadKey();
        }
    }
}
