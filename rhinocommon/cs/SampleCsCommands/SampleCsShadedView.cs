using System;
using Rhino;
using Rhino.Commands;
using Rhino.Display;

namespace SampleCsCommands
{
  public class SampleCsShadedView : Command
  {
    public override string EnglishName => "SampleCsShadedView";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;
      
      const string englishName = @"Shaded";
      DisplayModeDescription display_mode_description = DisplayModeDescription.FindByName(englishName);
      if (null != display_mode_description)
      {
        view.ActiveViewport.DisplayMode = display_mode_description;
        view.Redraw();
      }

      return Result.Success;
    }
  }
}
