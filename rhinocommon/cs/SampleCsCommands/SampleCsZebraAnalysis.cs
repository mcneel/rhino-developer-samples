using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsZebraAnalysis : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsZebraAnalysis"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var zebra_id = VisualAnalysisMode.RhinoZebraStripeAnalysisModeId;
      var analysis_mode = VisualAnalysisMode.Find(zebra_id);
      if (null == analysis_mode)
        return Result.Failure;

      var go = new GetObject();
      go.SetCommandPrompt("Select surfaces or polysurfaces for Zebra analysis");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (var obj_ref in go.Objects())
      {
        var obj = obj_ref.Object();
        if (null == obj)
          return Result.Failure;

        obj.EnableVisualAnalysisMode(analysis_mode, true);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
