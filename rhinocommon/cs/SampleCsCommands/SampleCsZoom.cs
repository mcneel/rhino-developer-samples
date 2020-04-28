using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsZoom : Command
  {
    public override string EnglishName => "SampleCsZoom";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var gz = new GetOption();
      gz.SetCommandPrompt("Zoom option");
      var b_opt = gz.AddOption("BoundingBox");
      var e_opt = gz.AddOption("Extents");
      var s_opt = gz.AddOption("Selected");
      gz.Get();
      if (gz.CommandResult() != Result.Success)
        return gz.CommandResult();

      var option = gz.Option();
      if (null == option)
        return Result.Failure;

      if (option.Index == b_opt)
      {
        var go = new GetObject();
        go.SetCommandPrompt("Select objects");
        go.SubObjectSelect = false;
        go.GetMultiple(1, 0);
        if (go.CommandResult() != Result.Success)
          return go.CommandResult();

        var bbox = new BoundingBox();
        for (var i = 0; i < go.ObjectCount; i++)
        {
          var geom = go.Object(i).Geometry();
          if (null != geom)
          {
            var b = geom.GetBoundingBox(true);
            if (b.IsValid)
            {
              if (0 == i)
                bbox = b;
              else
                bbox.Union(b);
            }
          }
        }

        if (bbox.IsValid)
        {
          view.ActiveViewport.ZoomBoundingBox(bbox);
          view.Redraw();
        }
      }
      else if (option.Index == e_opt)
      {
        view.ActiveViewport.ZoomExtents();
        view.Redraw();
      }
      else if (option.Index == s_opt)
      {
        view.ActiveViewport.ZoomExtentsSelected();
        view.Redraw();
      }

      return Result.Success;
    }
  }
}
