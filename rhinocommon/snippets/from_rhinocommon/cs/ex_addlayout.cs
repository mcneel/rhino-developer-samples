partial class Examples
{
  /// <summary>
  /// Generate a layout with a single detail view that zooms to a list of objects
  /// </summary>
  /// <param name="doc"></param>
  /// <returns></returns>
  public static Rhino.Commands.Result AddLayout(Rhino.RhinoDoc doc)
  {
    doc.PageUnitSystem = Rhino.UnitSystem.Millimeters;
    var page_views = doc.Views.GetPageViews();
    int page_number = (page_views==null) ? 1 : page_views.Length + 1;
    var pageview = doc.Views.AddPageView(string.Format("A0_{0}",page_number), 1189, 841);
    if( pageview!=null )
    {
      Rhino.Geometry.Point2d top_left = new Rhino.Geometry.Point2d(20,821);
      Rhino.Geometry.Point2d bottom_right = new Rhino.Geometry.Point2d(1169, 20);
      var detail = pageview.AddDetailView("ModelView", top_left, bottom_right, Rhino.Display.DefinedViewportProjection.Top);
      if (detail != null)
      {
        pageview.SetActiveDetail(detail.Id);
        detail.Viewport.ZoomExtents();
        detail.DetailGeometry.IsProjectionLocked = true;
        detail.DetailGeometry.SetScale(1, doc.ModelUnitSystem, 10, doc.PageUnitSystem);
        // Commit changes tells the document to replace the document's detail object
        // with the modified one that we just adjusted
        detail.CommitChanges();
      }
      pageview.SetPageAsActive();
      doc.Views.ActiveView = pageview;
      doc.Views.Redraw();
      return Rhino.Commands.Result.Success;
    }
    return Rhino.Commands.Result.Failure;
  }
}
