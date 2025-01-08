using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsSetCameraTarget : Command
  {
    public override string EnglishName => "SampleCsSetCameraTarget";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Display.RhinoView view = doc.Views.ActiveView;
      Rhino.Display.RhinoViewport vp = view.ActiveViewport;

      Rhino.Geometry.Point3d target = vp.CameraTarget;
      Rhino.Geometry.Point3d camera = vp.CameraLocation;

      Rhino.Input.Custom.GetPoint gp = new Rhino.Input.Custom.GetPoint();
      gp.SetCommandPrompt("New target location");
      gp.SetDefaultPoint(target);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      target = gp.Point();

      gp.ClearDefault();
      gp.SetCommandPrompt("New camera location");
      gp.SetDefaultPoint(camera);
      gp.SetBasePoint(target, true);
      gp.DrawLineFromPoint(target, true);
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      camera = gp.Point();

      Rhino.Geometry.Vector3d camdir = target - camera;
      camdir.Unitize();
      if (camdir.IsTiny())
        return Result.Cancel;

      vp.SetCameraLocations(target, camera);
      view.Redraw();

      return Result.Success;
    }
  }
}
