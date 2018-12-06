using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsAddAlignedDimension : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsAddAlignedDimension"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var style = doc.DimStyles.Current;
      var plane = Plane.WorldXY;

      var p1 = new Point3d(1.0, 1.0, 0.0);
      var p2 = new Point3d(5.0, 2.0, 0.0);
      var pl = new Point3d(5.0, 4.0, 0.0);

      var xaxis = p2 - p1;
      var yaxis = pl - p1;
      if (xaxis.Unitize() && yaxis.Unitize())
      {
        var zaxis = Vector3d.CrossProduct(xaxis, yaxis);
        if (zaxis.Unitize())
        {
          plane = new Plane(p1, xaxis, yaxis);
        }
      }

      var dim = LinearDimension.Create(AnnotationType.Aligned, style, plane, Plane.WorldXY.XAxis, p1, p2, pl, 0.0);

      //string displaytext = dim.GetDistanceDisplayText(doc.ModelUnitSystem, style);

      doc.Objects.Add(dim);

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}