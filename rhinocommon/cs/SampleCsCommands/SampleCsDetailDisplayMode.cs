using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.UI;

namespace SampleCsCommands
{
  public class SampleCsDetailDisplayMode : Command
  {
    public override string EnglishName => "SampleCsDetailDisplayMode";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      // Make sure a layout view is active
      if (!(view is RhinoPageView pageView))
      {
        RhinoApp.WriteLine("A layout view must be active.");
        return Result.Cancel;
      }

      // Get the detail or details to modify
      var detailViews = new List<DetailViewObject>();
      if (!pageView.PageIsActive)
      {
        detailViews.Add(pageView.ActiveDetail);
      }
      else
      {
        var filter = ObjectType.Detail;
        var rc = RhinoGet.GetMultipleObjects("Select detail views to set display mode", false, filter, out var objRefs);
        if (rc == Result.Success)
        {
          foreach (var objref in objRefs)
          {
            var rhObj = objref.Object();
            if (null != rhObj && rhObj is DetailViewObject detail)
              detailViews.Add(detail);
          }
        }
      }

      if (0 == detailViews.Count)
        return Result.Cancel;

      // Get the display mode to set
      var displayMode = GetDisplayModeDescription();
      if (null == displayMode)
        return Result.Cancel;

      // Set the detail display mode
      foreach (var detail in detailViews)
      {
        detail.Viewport.DisplayMode = displayMode;
        detail.CommitViewportChanges(); // critical
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    private DisplayModeDescription GetDisplayModeDescription()
    {
      var strings = new List<string>();

      var displayModes = Rhino.Display.DisplayModeDescription.GetDisplayModes();
      foreach (var dm in displayModes)
        strings.Add(dm.EnglishName);

      var title = "Set Detail Display Mode";
      var message = "Select the display mode to set:";
      var result = Dialogs.ShowListBox(title, message, strings, strings[0]);

      if (null != result && result is string englishName)
      {
        foreach (var dm in displayModes)
        {
          if (dm.EnglishName == englishName)
            return dm;
        }
      }

      return null;
    }
  }
}