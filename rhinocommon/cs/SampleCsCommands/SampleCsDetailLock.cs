using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  public class SampleCsDetailLock : Command
  {
    public override string EnglishName => "SampleCsDetailLock";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gd = new GetDetailViewObject();
      gd.SetCommandPrompt("Select detail view");
      gd.EnablePreSelect(false, true);
      gd.DeselectAllBeforePostSelect = false;
      gd.Get();
      if (gd.CommandResult() != Result.Success)
        return gd.CommandResult();

      var detail_obj = gd.Object(0).Object() as DetailViewObject;
      if (null == detail_obj)
        return Result.Failure;

      var detail = detail_obj.DetailGeometry;
      if (null == detail)
        return Result.Failure;

      detail.IsProjectionLocked = !detail.IsProjectionLocked;
      detail_obj.CommitChanges();

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}