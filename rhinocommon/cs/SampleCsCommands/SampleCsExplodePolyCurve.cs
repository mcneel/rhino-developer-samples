using System;
using System.Text;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// GetPolyCurveObject
  /// </summary>
  class GetPolyCurveObject : GetObject
  {
    public override bool CustomGeometryFilter(RhinoObject rhObject, GeometryBase geometry, ComponentIndex ci)
    {
      bool rc = false;
      if (null != geometry)
      {
        PolyCurve poly_curve = geometry as PolyCurve;
        if (null != poly_curve)
          rc = true;
      }
      return rc;
    }
  }

  /// <summary>
  /// SampleCsExplodePolyCurve
  /// </summary>
  [System.Runtime.InteropServices.Guid("308666d1-24f7-46d0-85d2-15f81438e08f")]
  public class SampleCsExplodePolyCurve : Command
  {
    public SampleCsExplodePolyCurve()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsExplodePolyCurve"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetPolyCurveObject go = new GetPolyCurveObject();
      go.SetCommandPrompt("Select polycurve to explode");
      go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      Curve curve = go.Object(0).Curve();
      if (null == curve)
        return Result.Failure;
    
      PolyCurve poly_curve = curve as PolyCurve;
      if (null == poly_curve)
        return Result.Failure;

      Curve[] segments = poly_curve.Explode();
      foreach (Curve segment in segments)
        doc.Objects.AddCurve(segment);

      doc.Objects.Delete(go.Object(0), false);

      doc.Views.Redraw();
 
      return Result.Success;
    }
  }
}
