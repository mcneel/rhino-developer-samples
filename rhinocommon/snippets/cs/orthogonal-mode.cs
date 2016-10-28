partial class Examples
{
  public static Result Ortho(RhinoDoc doc)
  {
    var gp = new GetPoint();
    gp.SetCommandPrompt("Start of line");
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var start_point = gp.Point();

    var original_ortho = ModelAidSettings.Ortho;
    if (!original_ortho)
      ModelAidSettings.Ortho = true;

    gp.SetCommandPrompt("End of line");
    gp.SetBasePoint(start_point, false);
    gp.DrawLineFromPoint(start_point, true);
    gp.Get();
    if (gp.CommandResult() != Result.Success)
      return gp.CommandResult();
    var end_point = gp.Point();

    if (ModelAidSettings.Ortho != original_ortho)
      ModelAidSettings.Ortho = original_ortho;

    doc.Objects.AddLine(start_point, end_point);
    doc.Views.Redraw();
    return Result.Success;
  }
}
