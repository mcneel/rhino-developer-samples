using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsSetCameraTarget : Command
  {
    public override string EnglishName => "SampleCsSetCameraTarget";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      var vp = view.ActiveViewport;

      var target = vp.CameraTarget;
      var camera = vp.CameraLocation;

      var gp = new Rhino.Input.Custom.GetPoint();
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

      var camdir = target - camera;
      camdir.Unitize();
      if (camdir.IsTiny())
        return Result.Cancel;

      vp.SetCameraLocations(target, camera);
      view.Redraw();

      return Result.Success; 
    }
  }
}
