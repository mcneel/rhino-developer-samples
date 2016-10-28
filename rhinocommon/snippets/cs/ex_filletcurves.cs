using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace examples_cs
{
  public class FilletCurvesCommand : Command
  {
    public override string EnglishName { get { return "csFilletCurves"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gc1 = new GetObject();
      gc1.DisablePreSelect();
      gc1.SetCommandPrompt("Select first curve to fillet (close to the end you want to fillet)");
      gc1.GeometryFilter = ObjectType.Curve;
      gc1.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      gc1.Get();
      if (gc1.CommandResult() != Result.Success)
        return gc1.CommandResult();
      var curve1_obj_ref = gc1.Object(0);
      var curve1 = curve1_obj_ref.Curve();
      if (curve1 == null) return Result.Failure;
      var curve1_point_near_end = curve1_obj_ref.SelectionPoint();
      if (curve1_point_near_end == Point3d.Unset)
        return Result.Failure;

      var gc2 = new GetObject();
      gc2.DisablePreSelect();
      gc2.SetCommandPrompt("Select second curve to fillet (close to the end you want to fillet)");
      gc2.GeometryFilter = ObjectType.Curve;
      gc2.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      gc2.Get();
      if (gc2.CommandResult() != Result.Success)
        return gc2.CommandResult();
      var curve2_obj_ref = gc2.Object(0);
      var curve2 = curve2_obj_ref.Curve();
      if (curve2 == null) return Result.Failure;
      var curve2_point_near_end = curve2_obj_ref.SelectionPoint();
      if (curve2_point_near_end == Point3d.Unset)
        return Result.Failure;

      double radius = 0;
      var rc = RhinoGet.GetNumber("fillet radius", false, ref radius);
      if (rc != Result.Success) return rc;

      var join = false;
      var trim = true;
      var arc_extension = true;
      var fillet_curves = Curve.CreateFilletCurves(curve1, curve1_point_near_end, curve2, curve2_point_near_end, radius,
        join, trim, arc_extension, doc.ModelAbsoluteTolerance, doc.ModelAngleToleranceDegrees);
      if (fillet_curves == null /*|| fillet_curves.Length != 3*/)
        return Result.Failure;

      foreach(var fillet_curve in fillet_curves)
        doc.Objects.AddCurve(fillet_curve);
      doc.Views.Redraw();
      return rc;
    }
  }
}