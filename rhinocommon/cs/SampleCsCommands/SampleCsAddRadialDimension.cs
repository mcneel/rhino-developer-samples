using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsAddRadialDimension : Command
  {
    public override string EnglishName => "SampleCsAddRadialDimension";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var plane = Plane.WorldXY;

      var cp = new Point3d(2.0, 2.0, 0.0);
      var p1 = new Point3d(4.0, 4.0, 0.0);
      var p2 = new Point3d(8.0, 6.0, 0.0);

      var style = doc.DimStyles.Current;
      if (style.LeaderContentAngleType == DimensionStyle.LeaderContentAngleStyle.Aligned)
        p2.Y = 8.0;

      var dim = RadialDimension.Create(style, AnnotationType.Radius, plane, cp, p1, p2);
      if (null != dim)
      {
        doc.Objects.Add(dim);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}