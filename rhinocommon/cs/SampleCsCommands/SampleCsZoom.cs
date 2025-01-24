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
      Rhino.Display.RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      GetOption gz = new GetOption();
      gz.SetCommandPrompt("Zoom option");
      int b_opt = gz.AddOption("BoundingBox");
      int e_opt = gz.AddOption("Extents");
      int s_opt = gz.AddOption("Selected");
      gz.Get();
      if (gz.CommandResult() != Result.Success)
        return gz.CommandResult();

      CommandLineOption option = gz.Option();
      if (null == option)
        return Result.Failure;

      if (option.Index == b_opt)
      {
        GetObject go = new GetObject();
        go.SetCommandPrompt("Select objects");
        go.SubObjectSelect = false;
        go.GetMultiple(1, 0);
        if (go.CommandResult() != Result.Success)
          return go.CommandResult();

        BoundingBox bbox = new BoundingBox();
        for (int i = 0; i < go.ObjectCount; i++)
        {
          GeometryBase geom = go.Object(i).Geometry();
          if (null != geom)
          {
            BoundingBox b = geom.GetBoundingBox(true);
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
