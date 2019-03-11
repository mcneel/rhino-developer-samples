using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsCreateNestedBlock : Command
  {
    public override string EnglishName => "SampleCsCreateNestedBlock";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Create a circle curve
      var circle = new Circle(Plane.WorldXY, 5.0);
      var curve0 = new ArcCurve(circle);

      // Add an instance defintion that uses the circle curve
      var attrib = doc.CreateDefaultAttributes();
      var idef0_index = doc.InstanceDefinitions.Add("Circle", "Circle", Point3d.Origin, curve0, attrib);

      // Create a reference to the instance definition
      var idef0_id = doc.InstanceDefinitions[idef0_index].Id;
      var iref0 = new InstanceReferenceGeometry(idef0_id, Transform.Identity);

      // Create a polyline curve
      var rect = new Point3d[5];
      rect[0] = new Point3d(-5.0, -5.0, 0.0);
      rect[1] = new Point3d(5.0, -5.0, 0.0);
      rect[2] = new Point3d(5.0, 5.0, 0.0);
      rect[3] = new Point3d(-5.0, 5.0, 0.0);
      rect[4] = rect[0];
      var curve1 = new PolylineCurve(rect);

      // Add another instance definition that uses the polyline curve
      // and the instance reference
      var geometry = new GeometryBase[] { curve1, iref0 };
      var attributes = new[] { attrib, attrib };
      var idef1_index = doc.InstanceDefinitions.Add("Rectangle and Circle", "Rectangle and Circle", Point3d.Origin, geometry, attributes);

      // Add an instace of the new defintion to the document and redraw
      doc.Objects.AddInstanceObject(idef1_index, Transform.Identity);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}