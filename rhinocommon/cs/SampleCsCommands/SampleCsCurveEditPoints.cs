using System;
using Rhino;
using Rhino.Collections;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("e18531ba-e3ff-49ff-91e2-9f2800eedc00")]
  public class SampleCsCurveEditPoints : Command
  {
    public SampleCsCurveEditPoints()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsCurveEditPoints"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      const string prompt = "Select curve";
      const ObjectType object_type = ObjectType.Curve;
      Result res = RhinoGet.GetOneObject(prompt, false, object_type, out obj_ref);
      if (res != Result.Success)
        return res;

      Curve crv = obj_ref.Curve();
      if (null == crv)
        return Result.Failure;

      NurbsCurve nc = crv.ToNurbsCurve();
      if (null == nc)
        return Result.Failure;

      Point3dList points = nc.GrevillePoints();
      foreach (Point3d point in points)
        doc.Objects.AddPoint(point);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
