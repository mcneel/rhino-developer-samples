using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Input;
using Rhino.Input.Custom;
using System;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsHideInDetail : Command
  {
    private bool m_bActive = true;

    public override string EnglishName => "SampleCsHideInDetail";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      if (view.ActiveViewport.ViewportType != ViewportType.DetailViewport)
      {
        RhinoApp.WriteLine("Must have a detail viewport active to run this command.");
        return Result.Cancel;
      }

      OptionToggle active_option = new OptionToggle(m_bActive, "Inactive", "Active");

      GetObject go = new GetObject();
      go.SetCommandPrompt("Select objects to hide");
      go.GroupSelect = true;
      for (; ; )
      {
        go.ClearCommandOptions();
        go.AddOptionToggle("Detail", ref active_option);

        GetResult res = go.GetMultiple(1, 0);

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

      List<Guid> viewport_id_list = new List<Guid>(16);
      if (m_bActive)
      {
        viewport_id_list.Add(view.ActiveViewportID);
      }
      else
      {
        if (view is RhinoPageView page_view)
        {
          Rhino.DocObjects.DetailViewObject[] detail_views = page_view.GetDetailViews();
          if (null != detail_views)
          {
            foreach (Rhino.DocObjects.DetailViewObject detail in detail_views)
            {
              if (detail.Viewport.Id != view.ActiveViewportID)
                viewport_id_list.Add(detail.Viewport.Id);
            }
          }
        }
      }

      if (0 == viewport_id_list.Count)
        return Result.Nothing;

      foreach (Rhino.DocObjects.ObjRef objref in go.Objects())
      {
        Rhino.DocObjects.RhinoObject obj = objref.Object();
        if (null != obj)
        {
          Rhino.DocObjects.ObjectAttributes attributes = obj.Attributes.Duplicate();
          foreach (Guid viewport_id in viewport_id_list)
            attributes.AddHideInDetailOverride(viewport_id);
          doc.Objects.ModifyAttributes(objref, attributes, true);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}