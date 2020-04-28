using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsRenderBackground : Command
  {
    public override string EnglishName => "SampleCsRenderBackground";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Render.RenderSettings settings = new Rhino.Render.RenderSettings();
      settings.BackgroundStyle = Rhino.Display.BackgroundStyle.Gradient;
      settings.BackgroundColorTop = System.Drawing.Color.Blue;
      settings.BackgroundColorBottom = System.Drawing.Color.White;

      doc.RenderSettings = settings;
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
