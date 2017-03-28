using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("22435645-2882-4598-8519-e3efecbe0699")]
  public class SampleCsExtrusion : Command
  {
    public SampleCsExtrusion()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsExtrusion"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var get_outer = new GetObject();
      get_outer.SetCommandPrompt("Outer profile curve");
      get_outer.GeometryFilter = ObjectType.Curve;
      get_outer.SubObjectSelect = false;
      get_outer.Get();
      if (get_outer.CommandResult() != Result.Success)
        return get_outer.CommandResult();

      var outer = get_outer.Object(0).Curve();
      if (null == outer)
        return Result.Failure;

      var get_inner = new GetObject();
      get_inner.SetCommandPrompt("Inner profile curve");
      get_inner.GeometryFilter = ObjectType.Curve;
      get_inner.SubObjectSelect = false;
      get_inner.EnablePreSelect(false, true);
      get_inner.DeselectAllBeforePostSelect = false;
      get_inner.GetMultiple(1, 0);
      if (get_inner.CommandResult() != Result.Success)
        return get_outer.CommandResult();

      var inners = new Curve[get_inner.ObjectCount];
      for (var i = 0; i < get_inner.ObjectCount; i++)
      {
        var inner = get_inner.Object(i).Curve();
        if (null == inner)
          return Result.Failure;
        inners[i] = inner;
      }

      var extrusion = CreateExtrusion(outer, inners, 10);
      if (null != extrusion)
      {
        doc.Objects.AddExtrusion(extrusion);
        doc.Views.Redraw();
      }

      return Result.Success;
    }

    protected Extrusion CreateExtrusion(Curve outerProfile, Curve[] innerProfiles, double height)
    {
      if (null == outerProfile || height <= RhinoMath.ZeroTolerance)
        return null;

      Plane plane;
      if (!outerProfile.TryGetPlane(out plane))
        return null;

      var path = new Line
      {
        From = plane.PointAt(0.0, 0.0, 0.0),
        To = plane.PointAt(0.0, 0.0, height)
      };
      if (!path.IsValid || !(path.Length > RhinoMath.ZeroTolerance))
        return null;

      var up = plane.YAxis;
      var tangent = path.To - path.From;
      tangent.Unitize();
      if (!up.IsValid || !up.IsUnitVector || Math.Abs(up * tangent) > RhinoMath.SqrtEpsilon)
        return null;

      var xform = Transform.ChangeBasis(Plane.WorldXY, plane);

      var curve = outerProfile.DuplicateCurve();
      curve.Transform(xform);
      curve.ChangeDimension(2);

      var extrusion = new Extrusion();
      extrusion.SetOuterProfile(curve, true);

      foreach (var profile in innerProfiles)
      {
        Plane curve_plane;
        if (profile.TryGetPlane(out curve_plane))
        {
          if (plane.IsCoplanar(curve_plane, RhinoMath.ZeroTolerance))
          {
            curve = profile.DuplicateCurve();
            curve.Transform(xform);
            curve.ChangeDimension(2);
            extrusion.AddInnerProfile(curve);
          }
        }
      }

      extrusion.SetPathAndUp(path.From, path.To, up);

      return extrusion.IsValid ? extrusion : null;
    }
  }

  /// <summary>
  /// Plane extension methods class
  /// </summary>
  public static class PlaneExtensions
  {
    public static bool IsCoplanar(this Plane plane, Plane testPlane, double tolerance)
    {
      if (!plane.IsValid || !testPlane.IsValid)
        return false;

      if (tolerance < RhinoMath.ZeroTolerance)
        tolerance = RhinoMath.ZeroTolerance;

      var eq0 = plane.GetPlaneEquation();
      var eq1 = testPlane.GetPlaneEquation();

      return Math.Abs(eq0[0] - eq1[0]) < tolerance &&
             Math.Abs(eq0[1] - eq1[1]) < tolerance &&
             Math.Abs(eq0[2] - eq1[2]) < tolerance &&
             Math.Abs(eq0[3] - eq1[3]) < tolerance;
    }
  }

}
