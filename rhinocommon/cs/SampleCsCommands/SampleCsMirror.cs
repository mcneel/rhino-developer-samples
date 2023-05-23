using Rhino;
using Rhino.Collections;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// Inherit from TransformCommand, not Command
  /// </summary>
  public class SampleCsMirror : TransformCommand
  {
    private bool CopyObjects { get; set; } = false;

    public override string EnglishName => "SampleCsMirror";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var list = new TransformObjectList();
      var rc = SelectObjects("Select objects to mirror", list);
      if (rc != Result.Success)
        return rc;

      var copyObjects = CopyObjects;
      var copyOption = new OptionToggle(copyObjects, "No", "Yes");

      var gp = new GetPoint();
      gp.SetCommandPrompt("Start of mirror plane");
      gp.AddOptionToggle("Copy", ref copyOption);
      while (true)
      {
        var res = gp.Get();
        if (res == GetResult.Option)
          continue;
        else if (res != GetResult.Point)
          return Result.Cancel;
        break;
      }

      var origin = gp.Point();

      var gx = new GetMirrorTransform();
      gx.AddOptionToggle("Copy", ref copyOption);
      gx.AddTransformObjects(list);
      gx.ConstrainToConstructionPlane(true);
      gx.DrawLineFromPoint(origin, true);
      gx.SetBasePoint(origin, true);
      gx.SetCommandPrompt("End of mirror plane");

      while (true)
      {
        var res = gx.GetXform();
        if (res == GetResult.Option)
          continue;
        else if (res != GetResult.Point)
          return Result.Cancel;
        break;
      }

      CopyObjects = copyOption.CurrentValue;

      var view = gx.View();
      if (null != view)
      {
        var xform = gx.CalculateTransform(view.ActiveViewport, gx.Point());
        if (xform.IsValid && !xform.IsIdentity)
        {
          // Enable auto-history recording when CopyObjects = true
          TransformObjects(list, xform, CopyObjects, CopyObjects);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  /// <summary>
  /// Dynamic transformation getter
  /// </summary>
  internal class GetMirrorTransform : GetTransform
  {
    public override Transform CalculateTransform(RhinoViewport viewport, Point3d point)
    {
      var rc = Transform.Identity;
      if (TryGetBasePoint(out var basePoint))
      {
        var plane = viewport.ConstructionPlane();
        var dir = Vector3d.CrossProduct(point - basePoint, plane.Normal);
        if (!dir.IsTiny())
        {
          dir.Unitize();
          rc = Transform.Mirror(basePoint, dir);
        }
      }
      return rc;
    }
  }
}