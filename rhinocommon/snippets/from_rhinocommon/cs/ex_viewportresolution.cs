using Rhino;
using Rhino.Commands;

namespace examples_cs
{
  public class ViewportResolutionCommand : Command
  {
    public override string EnglishName { get { return "csViewportResolution"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var active_viewport = doc.Views.ActiveView.ActiveViewport;
      RhinoApp.WriteLine("Name = {0}: Width = {1}, Height = {2}", 
        active_viewport.Name, active_viewport.Size.Width, active_viewport.Size.Height);
      return Result.Success;
    }
  }
}