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
      Rhino.Display.RhinoView view = doc.Views.ActiveView;
      if (null == view)
        return Result.Failure;

      Plane plane = view.ActiveViewport.ConstructionPlane();

      DimensionStyle dimstyle = doc.DimStyles.FindName("TestDimStyle");
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

        int dimstyle_index = doc.DimStyles.Add(dimstyle, false);
        if (dimstyle_index >= 0 && dimstyle_index < doc.DimStyles.Count)
          dimstyle = doc.DimStyles[dimstyle_index];
        else
          return Result.Failure;
      }

      TextEntity text_entity = TextEntity.Create("Hello Rhino!", plane, dimstyle, false, 0, 0.0);
      text_entity.GetBoundingBox(plane, out Box box);
      Point3d[] corners = box.GetCorners();

      Point3d[] points = new Point3d[] { corners[0], corners[1], corners[2], corners[3], corners[0] };
      doc.Objects.AddPolyline(points);

      text_entity.IsValidWithLog(out string log);
      doc.Objects.AddText(text_entity);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}