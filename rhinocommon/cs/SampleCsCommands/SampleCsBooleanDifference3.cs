using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsBooleanDifference3 : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsBooleanDifference3"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const double height = 20.0;
      var tolerance = doc.ModelAbsoluteTolerance;

      var points0 = new List<Point3d>
      {
        new Point3d(-1.0, 0.0, 0.0),
        new Point3d(-1.0, -1.0, 0.0),
        new Point3d(-2.0, -1.0, 0.0),
        new Point3d(-2.0, -2.0, 0.0),
        new Point3d(2.0, -2.0, 0.0),
        new Point3d(1.0, -1.0, 0.0),
        new Point3d(1.0, 0.0, 0.0),
        new Point3d(-1.0, 0.0, 0.0)
      };

      var brep0 = CreateBrep(points0, height, tolerance);
      if (null == brep0)
        return Result.Failure;

      var points1 = new List<Point3d>
      {
        new Point3d(-1.0, 0.0, 0.0),
        new Point3d(-1.0, -1.0, 0.0),
        new Point3d(1.0, -1.0, 0.0),
        new Point3d(1.0, 0.0, 0.0),
        new Point3d(-1.0, 0.0, 0.0)
      };

      var brep1 = CreateBrep(points1, height, tolerance);
      if (null == brep1)
        return Result.Failure;

      brep1.Rotate((Math.PI * 0.5), new Vector3d(1.0, 0.0, 0.0), new Point3d(0.0, 0.0, (0.5 * height)));
      brep1.Translate(new Vector3d(0.0, -1.0, 5.0));

      var results = Brep.CreateBooleanDifference(brep0, brep1, tolerance);
      if (null != results)
      {
        foreach (var rc in results)
          doc.Objects.AddBrep(rc);
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    /// <summary>
    /// Create Brep
    /// </summary>
    protected Brep CreateBrep(List<Point3d> points, double height, double tolerance)
    {
      if (points.Count < 4 || height <= 0.0)
        return null;

      // Create the profile curve
      var profile = new PolylineCurve(points);
      if (!profile.IsClosed || !profile.IsPlanar(tolerance))
        return null;

      // Create a surface by extruding the profile curve
      var surface = Surface.CreateExtrusion(profile, new Vector3d(0.0, 0.0, height));
      if (null == surface)
        return null;

      // Create a Brep from the surface
      var brep = surface.ToBrep();

      // The profile curve is a degree=1 curve. Thus, the extruded surface will
      // have kinks. Because kinked surface can cause problems down stream, Rhino
      // always splits kinked surfaces when adding Breps to the document. Since
      // we are not adding this Brep to the document, lets split the kinked
      // surfaces ourself.
      brep.Faces.SplitKinkyFaces(RhinoMath.DefaultAngleTolerance, true);

      // Cap any planar holes
      var capped_brep = brep.CapPlanarHoles(tolerance);
      if (null == capped_brep)
        return null;

      // The profile curve, created by the input points, is oriented clockwise.
      // Thus when the profile is extruded, the resulting surface will have its
      // normals pointed inwards. So lets check the orientation and, if inwards,
      // flip the face normals.
      if (BrepSolidOrientation.Inward == capped_brep.SolidOrientation)
        capped_brep.Flip();

      string log;
      if (!capped_brep.IsValidWithLog(out log))
      {
        RhinoApp.WriteLine(log);
        return null;
      }

      return capped_brep;
    }
  }
}
