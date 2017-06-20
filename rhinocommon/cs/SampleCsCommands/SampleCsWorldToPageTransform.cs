using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsWorldToPageTransform : Command
  {
    public override string EnglishName => "SampleCsWorldToPageTransform";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var page_view = view as RhinoPageView;
      if (null == page_view)
      {
        RhinoApp.WriteLine("The active view is neither a layout nor a detail view.");
        return Result.Failure;
      }

      var go = new GetObject();
      go.SetCommandPrompt("Select point object");
      go.GeometryFilter = ObjectType.Point;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var point_obj = go.Object(0).Point();
      if (null == point_obj)
        return Result.Failure;

      if (page_view.PageIsActive)
      {
        var gd = new GetDetailViewObject();
        gd.SetCommandPrompt("Select target detail view");
        gd.EnablePreSelect(false, true);
        gd.DeselectAllBeforePostSelect = false;
        gd.Get();
        if (gd.CommandResult() != Result.Success)
          return gd.CommandResult();

        var detail = gd.Object(0).Object() as DetailViewObject;
        if (null != detail)
        {
          var point = point_obj.Location;
          RhinoApp.WriteLine("Page location: {0}", point.ToString());
          point.Transform(detail.PageToWorldTransform);
          RhinoApp.WriteLine("World location: {0}", point.ToString());
        }
      }
      else
      {
        var detail = FindActiveDetailObject(page_view);
        if (null != detail)
        {
          var point = point_obj.Location;
          RhinoApp.WriteLine("World location: {0}", point.ToString());
          point.Transform(detail.WorldToPageTransform);
          RhinoApp.WriteLine("Page location: {0}", point.ToString());
        }
      }

      return Result.Success;
    }

    /// <summary>
    /// Finds the active detail object of a page view
    /// </summary>
    protected DetailViewObject FindActiveDetailObject(RhinoPageView pageView)
    {
      if (null == pageView || pageView.PageIsActive)
        return null;

      var details = pageView.GetDetailViews();
      if (null == details)
        return null;

      foreach (var detail in details)
      {
        if (detail.IsActive)
          return detail;
      }

      return null;
    }
  }

  /// <summary>
  /// Class to select detail view objects
  /// </summary>
  public class GetDetailViewObject : GetObject
  {
    public override bool CustomGeometryFilter(RhinoObject rhObject, GeometryBase geometry, ComponentIndex componentIndex)
    {
      var detail = rhObject as DetailViewObject;
      return detail != null;
    }
  }
}
