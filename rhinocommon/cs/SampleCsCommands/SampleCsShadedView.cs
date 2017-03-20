using System;
using Rhino;
using Rhino.Commands;
using Rhino.Display;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("d1e5fb6f-647c-4e8c-bf92-eba401d87b42")]
  public class SampleCsShadedView : Command
  {
    public SampleCsShadedView()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsShadedView"; }
    }

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
