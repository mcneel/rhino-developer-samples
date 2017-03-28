using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("6a488cda-0e70-4ba0-8a5f-6c5359993a8f")]
  public class SampleCsRenderBackground : Command
  {
    public SampleCsRenderBackground()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsRenderBackground"; }
    }

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
