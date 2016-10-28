using Rhino;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.Display;

namespace examples_cs
{
  public class GetTranslation : GetTransform
  {
    public override Transform CalculateTransform(RhinoViewport viewport, Point3d point)
    {
      var xform = Transform.Identity;
      Point3d base_point;
      if (TryGetBasePoint(out base_point))
      {
        var v = point - base_point;
        if (!v.IsZero)
        {
          xform = Transform.Translation(v);
          if (!xform.IsValid)
            xform = Transform.Identity;
        }
      }
      return xform;
    }
  }

  public class RhinoGetTransformCommand : TransformCommand
  {
    public RhinoGetTransformCommand()
    {
      // simple example of handling the BeforeTransformObjects event
      RhinoDoc.BeforeTransformObjects += RhinoDocOnBeforeTransformObjects;
    }

    private void RhinoDocOnBeforeTransformObjects(object sender, RhinoTransformObjectsEventArgs ea)
    {
      RhinoApp.WriteLine("Transform Objects Count: {0}", ea.ObjectCount);
    }

    public override string EnglishName { get { return "csGetTranslation"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var list = new Rhino.Collections.TransformObjectList();
      var rc = SelectObjects("Select objects to move", list);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move from");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();


      var gt = new GetTranslation();
      gt.SetCommandPrompt("Point to move to");
      gt.SetBasePoint(gp.Point(), true);
      gt.DrawLineFromPoint(gp.Point(), true);
      gt.AddTransformObjects(list);
      gt.GetXform();
      if (gt.CommandResult() != Result.Success)
        return gt.CommandResult();

      var xform = gt.CalculateTransform(gt.View().ActiveViewport, gt.Point());
      TransformObjects(list, xform, false, false);
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}
