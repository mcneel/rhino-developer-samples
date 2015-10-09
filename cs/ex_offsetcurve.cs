using Rhino;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Commands;

namespace examples_cs
{
  public class OffsetCurveCommand : Command
  {
    public override string EnglishName { get { return "csOffsetCurve"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      var rs = RhinoGet.GetOneObject(
        "Select Curve", false, ObjectType.Curve, out obj_ref);
      if (rs != Result.Success)
        return rs;
      var curve = obj_ref.Curve();
      if (curve == null)
        return Result.Nothing;

      Point3d point;
      rs = RhinoGet.GetPoint("Select Side", false, out point);
      if (rs != Result.Success)
        return rs;
      if (point == Point3d.Unset)
        return Result.Nothing;

      var curves = curve.Offset(point, Vector3d.ZAxis, 1.0, 
        doc.ModelAbsoluteTolerance, CurveOffsetCornerStyle.None);

      foreach (var offset_curve in curves)
        doc.Objects.AddCurve(offset_curve);

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}