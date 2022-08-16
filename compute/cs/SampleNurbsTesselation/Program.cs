using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Rhino.Compute;
using Rhino.Geometry;

namespace NurbsTesselation
{
    class Program
    {
        static void Main(string[] args)
        {
            ComputeServer.WebAddress = "http://localhost:5000/";
            // ComputeServer.ApiKey = "";

            Curve curveA = Curve.CreateControlPointCurve(new Point3d[] {
                new Point3d (0.0, 0.0, 0.0),
                new Point3d (0.0, 0.5, -0.5),
                new Point3d (0.0, 1.0, 0.0)
            });
            Curve curveB = Curve.CreateControlPointCurve(new Point3d[] {
                new Point3d (1.0, 0.0, 0.0),
                new Point3d (1.0, 0.3, 0.6),
                new Point3d (1.0, 0.5, 0.0),
                new Point3d (1.0, 0.7, 0.8),
                new Point3d (1.0, 1.0, 0.0)
            });

            NurbsSurface ruled = NurbsSurface.CreateRuledSurface(curveA, curveB);

            Mesh mesh1 = MeshCompute.CreateFromSurface(ruled);
            Rhino.Compute.ObjExport.ExportMeshesToObj("ruled_mesh_default.obj", new Mesh[] { mesh1 });

            MeshingParameters meshingParameters = new MeshingParameters();
            meshingParameters.MaximumEdgeLength = 0.1;
            Mesh mesh2 = MeshCompute.CreateFromSurface(ruled, meshingParameters);
            Rhino.Compute.ObjExport.ExportMeshesToObj("ruled_mesh_refined1.obj", new Mesh[] { mesh2 });

            meshingParameters.MaximumEdgeLength = 0.05;
            Mesh mesh3 = MeshCompute.CreateFromSurface(ruled, meshingParameters);
            Rhino.Compute.ObjExport.ExportMeshesToObj("ruled_mesh_refined2.obj", new Mesh[] { mesh3 });
        }
    }
}
