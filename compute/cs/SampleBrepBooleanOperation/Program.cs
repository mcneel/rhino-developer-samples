using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Rhino.Compute;
using Rhino.Geometry;

namespace BrepBooleanOperation
{
    class Program
    {
        static void Main(string[] args)
        {
            ComputeServer.WebAddress = "http://localhost:5000/";
            // ComputeServer.ApiKey = "";

            var cube = new Box(new BoundingBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5));
            var sphere = new Sphere(new Point3d(0.5, 0.5, 0.5), 0.8);

            Brep[] cubeBrep = new Brep[] { cube.ToBrep() };
            Brep[] sphereBrep = new Brep[] { sphere.ToBrep() };

            const double tolerance = 1.0e-8;

            List<Brep> allBreps = new List<Brep>();
            allBreps.AddRange(cubeBrep);
            allBreps.AddRange(sphereBrep);
            Brep[] union = BrepCompute.CreateBooleanUnion(allBreps, tolerance);
            Rhino.Compute.ObjExport.ExportBrepsToObj("cube_sphere_union.obj", union);

            Brep[] intersection = BrepCompute.CreateBooleanIntersection(cubeBrep, sphereBrep, tolerance);
            Rhino.Compute.ObjExport.ExportBrepsToObj("cube_sphere_intersection.obj", intersection);

            Brep[] difference = BrepCompute.CreateBooleanDifference(cubeBrep, sphereBrep, tolerance);
            Rhino.Compute.ObjExport.ExportBrepsToObj("cube_sphere_difference.obj", difference);
        }
    }
}
