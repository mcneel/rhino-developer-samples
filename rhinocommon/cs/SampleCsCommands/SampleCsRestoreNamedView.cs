using Rhino;
using Rhino.Commands;
using Rhino.Input;

namespace SampleCsCommands
{
  public class SampleCsRestoreNamedView : Command
  {
    public override string EnglishName => "SampleCsRestoreNamedView";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Display.RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      string name = null;
      Result rc = RhinoGet.GetString("Named view to restore", false, ref name);
      if (rc != Result.Success || string.IsNullOrEmpty(name))
        return Result.Cancel;

      int index = doc.NamedViews.FindByName(name);
      if (index < 0 || index >= doc.NamedViews.Count)
      {
        RhinoApp.WriteLine("Named view not found");
        return Result.Nothing;
      }

      doc.NamedViews.Restore(index, view.ActiveViewport);
      view.Redraw();

      return Result.Success;
    }
  }
}
