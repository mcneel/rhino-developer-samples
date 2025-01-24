using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsAddAlignedDimension : Command
  {
    public override string EnglishName => "SampleCsAddAlignedDimension";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.DimensionStyle style = doc.DimStyles.Current;
      Plane plane = Plane.WorldXY;

      Point3d p1 = new Point3d(1.0, 1.0, 0.0);
      Point3d p2 = new Point3d(5.0, 2.0, 0.0);
      Point3d pl = new Point3d(5.0, 4.0, 0.0);

      Vector3d xaxis = p2 - p1;
      Vector3d yaxis = pl - p1;
      if (xaxis.Unitize() && yaxis.Unitize())
      {
        Vector3d zaxis = Vector3d.CrossProduct(xaxis, yaxis);
        if (zaxis.Unitize())
        {
          plane = new Plane(p1, xaxis, yaxis);
        }
      }

      LinearDimension dim = LinearDimension.Create(AnnotationType.Aligned, style, plane, Plane.WorldXY.XAxis, p1, p2, pl, 0.0);

      //string displaytext = dim.GetDistanceDisplayText(doc.ModelUnitSystem, style);

      doc.Objects.Add(dim);

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}