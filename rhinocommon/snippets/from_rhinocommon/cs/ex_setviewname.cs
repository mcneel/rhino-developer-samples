using Rhino;
using Rhino.Commands;

namespace examples_cs
{
  public class SetViewNameCommand : Command
  {
    public override string EnglishName { get { return "csSetViewName"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (view == null)
        return Result.Failure;
     
      view.MainViewport.Name = "Facade";
      return Result.Success;
    }
  }
}