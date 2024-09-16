using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsContourMesh : Command
  {
    public override string EnglishName => "SampleCsContourMesh";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjectType filter = Rhino.DocObjects.ObjectType.Mesh;
      Rhino.DocObjects.ObjRef objref = null;
      Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select mesh to contour", false, filter, out objref);
      if (rc != Rhino.Commands.Result.Success || objref == null)
        return rc;

      Rhino.Geometry.Mesh mesh = objref.Geometry() as Rhino.Geometry.Mesh;
      if (null == mesh)
        return Rhino.Commands.Result.Failure;

      Rhino.Geometry.BoundingBox bbox = mesh.GetBoundingBox(false);
      Rhino.Geometry.Point3d start_pt = bbox.Corner(true, true, true);
      Rhino.Geometry.Point3d end_pt = bbox.Corner(false, true, true);
      double interval = start_pt.DistanceTo(end_pt) / 10;

      Rhino.Geometry.Curve[] curves = Rhino.Geometry.Mesh.CreateContourCurves(mesh, start_pt, end_pt, interval, doc.ModelAbsoluteTolerance);
      if (null != curves && curves.Length > 0)
      {
        for (int i = 0; i < curves.Length; i++)
          doc.Objects.AddCurve(curves[i]);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}
