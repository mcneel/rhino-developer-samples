using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsZebraAnalysis : Command
  {
    public override string EnglishName => "SampleCsZebraAnalysis";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      System.Guid zebra_id = VisualAnalysisMode.RhinoZebraStripeAnalysisModeId;
      VisualAnalysisMode analysis_mode = VisualAnalysisMode.Find(zebra_id);
      if (null == analysis_mode)
        return Result.Failure;

      GetObject go = new GetObject();
      go.SetCommandPrompt("Select surfaces or polysurfaces for Zebra analysis");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      foreach (ObjRef obj_ref in go.Objects())
      {
        RhinoObject obj = obj_ref.Object();
        if (null == obj)
          return Result.Failure;

        obj.EnableVisualAnalysisMode(analysis_mode, true);
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
