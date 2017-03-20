using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("6db23deb-41ed-4567-8bc7-80f6bd11cce8")]
  public class SampleCsCurvePoints : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsCurvePoints"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curve");
      go.GeometryFilter = ObjectType.Curve;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curve = go.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      // Get start and end point the easy way
      //RhinoApp.WriteLine("Start point = {0}", curve.PointAtStart);
      //RhinoApp.WriteLine("End point = {0}", curve.PointAtEnd);

      // Get start, mid, and end points 
      double t;
      Point3d point;

      if (curve.NormalizedLengthParameter(0.0, out t))
      {
        point = curve.PointAt(t);
        doc.Objects.AddPoint(point);
        if (curve.IsClosed)
          RhinoApp.WriteLine("Start/End point = {0}", point);
        else
          RhinoApp.WriteLine("Start point = {0}", point);
      }

      if (curve.NormalizedLengthParameter(0.5, out t))
      {
        point = curve.PointAt(t);
        doc.Objects.AddPoint(point);
        RhinoApp.WriteLine("Mid point = {0}", point);
      }

      if (!curve.IsClosed && curve.NormalizedLengthParameter(1.0, out t))
      {
        point = curve.PointAt(t);
        doc.Objects.AddPoint(point);
        RhinoApp.WriteLine("End point = {0}", point);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
