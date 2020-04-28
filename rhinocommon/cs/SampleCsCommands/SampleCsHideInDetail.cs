using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsHideInDetail : Command
  {
    private bool m_bActive = true;

    public override string EnglishName => "SampleCsHideInDetail";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      if (view.ActiveViewport.ViewportType != ViewportType.DetailViewport)
      {
        RhinoApp.WriteLine("Must have a detail viewport active to run this command.");
        return Result.Cancel;
      }

      var active_option = new OptionToggle(m_bActive, "Inactive", "Active");

      var go = new GetObject();
      go.SetCommandPrompt("Select objects to hide");
      go.GroupSelect = true;
      for (; ; )
      {
        go.ClearCommandOptions();
        go.AddOptionToggle("Detail", ref active_option);

        var res = go.GetMultiple(1, 0);

        if (res == GetResult.Option)
          continue;
        else if (res != GetResult.Object)
          return Result.Cancel;

        break;
      }

      if (go.ObjectCount < 1)
        return Result.Cancel;

      m_bActive = active_option.CurrentValue;

      // Confirm we are still in a detail view

      view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      if (view.ActiveViewport.ViewportType != ViewportType.DetailViewport)
      {
        RhinoApp.WriteLine("Must have a detail viewport active to run this command.");
        return Result.Cancel;
      }

      var viewport_id_list = new List<Guid>(16);
      if (m_bActive)
      {
        viewport_id_list.Add(view.ActiveViewportID);
      }
      else
      {
        if (view is RhinoPageView page_view)
        {
          var detail_views = page_view.GetDetailViews();
          if (null != detail_views)
          {
            foreach (var detail in detail_views)
            {
              if (detail.Viewport.Id != view.ActiveViewportID)
                viewport_id_list.Add(detail.Viewport.Id);
            }
          }
        }
      }

      if (0 == viewport_id_list.Count)
        return Result.Nothing;

      foreach (var objref in go.Objects())
      {
        var obj = objref.Object();
        if (null != obj)
        {
          var attributes = obj.Attributes.Duplicate();
          foreach (var viewport_id in viewport_id_list)
            attributes.AddHideInDetailOverride(viewport_id);
          doc.Objects.ModifyAttributes(objref, attributes, true);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}