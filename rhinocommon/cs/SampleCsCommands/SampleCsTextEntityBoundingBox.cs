using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsTextEntityBoundingBox : Command
  {
    public override string EnglishName => "SampleCsTextEntityBoundingBox";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      var plane = view.ActiveViewport.ConstructionPlane();

      var dimstyle = doc.DimStyles.FindName("TestDimStyle");
      if (null == dimstyle)
      {
        dimstyle = new DimensionStyle
        {
          TextHeight = 2,
          TextHorizontalAlignment = TextHorizontalAlignment.Center,
          LeaderTextVerticalAlignment = TextVerticalAlignment.Bottom,
          Name = "TestDimStyle",
          DimensionScale = 1,
          Font = new Rhino.DocObjects.Font("Book Antiqua")
        };

        var dimstyle_index = doc.DimStyles.Add(dimstyle, false);
        if (dimstyle_index >= 0 && dimstyle_index < doc.DimStyles.Count)
          dimstyle = doc.DimStyles[dimstyle_index];
        else
          return Result.Failure;
      }

      var text_entity = TextEntity.Create("Hello Rhino!", plane, dimstyle, false, 0, 0.0);
      text_entity.GetBoundingBox(plane, out var box);
      var corners = box.GetCorners();

      var points = new Point3d[] { corners[0], corners[1], corners[2], corners[3], corners[0] };
      doc.Objects.AddPolyline(points);

      text_entity.IsValidWithLog(out var log);
      doc.Objects.AddText(text_entity);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}