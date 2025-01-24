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
      Plane plane = Plane.WorldXY;

      Point3d cp = new Point3d(2.0, 2.0, 0.0);
      Point3d p1 = new Point3d(4.0, 4.0, 0.0);
      Point3d p2 = new Point3d(8.0, 6.0, 0.0);

      DimensionStyle style = doc.DimStyles.Current;
      if (style.LeaderContentAngleType == DimensionStyle.LeaderContentAngleStyle.Aligned)
        p2.Y = 8.0;

      RadialDimension dim = RadialDimension.Create(style, AnnotationType.Radius, plane, cp, p1, p2);
      if (null != dim)
      {
        doc.Objects.Add(dim);
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }
}