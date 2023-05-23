using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsContourMesh : Command
  {
    public override string EnglishName => "SampleCsContourMesh";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var filter = ObjectType.Mesh;
      var rc = Rhino.Input.RhinoGet.GetOneObject("Select mesh to contour", false, filter, out var objref);
      if (rc != Result.Success || objref == null)
        return rc;

      var mesh = objref.Geometry() as Rhino.Geometry.Mesh;
      if (null == mesh)
        return Result.Failure;

      var bbox = mesh.GetBoundingBox(false);
      var startPt = bbox.Corner(true, true, true);
      var endPt = bbox.Corner(false, true, true);
      var interval = startPt.DistanceTo(endPt) / 10;

      var curves = Mesh.CreateContourCurves(mesh, startPt, endPt, interval, doc.ModelAbsoluteTolerance);
      if (null != curves && curves.Length > 0)
      {
        foreach (var crv in curves)
          doc.Objects.AddCurve(crv);
      }
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
