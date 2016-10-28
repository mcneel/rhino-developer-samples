using Rhino;
using Rhino.Commands;
using Rhino.Input;

namespace examples_cs
{
  public class RhinoPageViewWidthHeightCommand : Command
  {
    public override string EnglishName { get { return "csSetRhinoPageViewWidthAndHeight"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var width = 1189;
      var height = 841;
      var page_views = doc.Views.GetPageViews();
      int page_number = (page_views==null) ? 1 : page_views.Length + 1;
      var pageview = doc.Views.AddPageView(string.Format("A0_{0}",page_number), width, height);

      int new_width = width;
      var rc = RhinoGet.GetInteger("new width", false, ref new_width);
      if (rc != Result.Success || new_width <= 0) return rc;

      int new_height = height;
      rc = RhinoGet.GetInteger("new height", false, ref new_height);
      if (rc != Result.Success || new_height <= 0) return rc;

      pageview.PageWidth = new_width;
      pageview.PageHeight = new_height;
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}