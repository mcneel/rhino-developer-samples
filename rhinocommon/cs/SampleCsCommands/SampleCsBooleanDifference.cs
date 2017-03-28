using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("1206d7fe-e8e2-4b83-b657-7f1ca1a97e95")]
  public class SampleCsBooleanDifference : Command
  {
    public SampleCsBooleanDifference()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsBooleanDifference"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      double tolerance = doc.ModelAbsoluteTolerance;

      Rhino.Geometry.Brep brep;
      bool rc = CreateSolid(tolerance, out brep);
      if (!rc)
        return Result.Failure;

      Rhino.Geometry.Brep hole;
      rc = CreateHole(out hole);
      if (!rc)
        return Result.Failure;

      Rhino.Geometry.Brep[] breps = Rhino.Geometry.Brep.CreateBooleanDifference(brep, hole, tolerance);
      if (null != breps)
      {
        for (int i = 0; i < breps.Length; i++)
          doc.Objects.AddBrep(breps[i]);
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    bool CreateSolid(double tolerance, out Rhino.Geometry.Brep outBrep)
    {
      outBrep = null;

      Rhino.Geometry.Point3d origin = new Rhino.Geometry.Point3d(0.0, 0.0, 0.0);
      Rhino.Geometry.Circle circle = new Rhino.Geometry.Circle(origin, 10.0);
      Rhino.Geometry.ArcCurve curve = new Rhino.Geometry.ArcCurve(circle);

      Rhino.Geometry.Vector3d direction = new Rhino.Geometry.Vector3d(0, 0, 10);
      Rhino.Geometry.Surface surface = Rhino.Geometry.Surface.CreateExtrusion(curve, direction);

      Rhino.Geometry.Brep brep = Rhino.Geometry.Brep.CreateFromSurface(surface);
      Rhino.Geometry.Brep solid = brep.CapPlanarHoles(tolerance);

      bool rc = solid.IsValid;
      if (rc)
        outBrep = solid;

      return rc;
    }

    bool CreateHole(out Rhino.Geometry.Brep outBrep)
    {
      outBrep = null;

      // Make sure the "hole" goes completely through the solid so
      // the Boolean operation will succeed.

      Rhino.Geometry.Point3d origin = new Rhino.Geometry.Point3d(0.0, 0.0, -1.0);
      Rhino.Geometry.Circle circle = new Rhino.Geometry.Circle(origin, 4.0);

      Rhino.Geometry.Cylinder cylinder = new Rhino.Geometry.Cylinder(circle, 12.0);
      Rhino.Geometry.Brep brep = cylinder.ToBrep(true, true);

      bool rc = brep.IsValid;
      if (rc)
        outBrep = brep;

      return rc;
    }
  }
}
