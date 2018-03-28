using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsProjectCurvesToBrep : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsProjectCurvesToBrep"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select curves to project
      var gc = new GetObject();
      gc.SetCommandPrompt("Select curves to project");
      gc.GeometryFilter = ObjectType.Curve;
      gc.GetMultiple(1, 0);
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      // Select planar surface to project onto
      var gs = new GetObject();
      gs.SetCommandPrompt("Select planar surface to project onto");
      gs.GeometryFilter = ObjectType.Surface;
      gs.EnablePreSelect(false, true);
      gs.DeselectAllBeforePostSelect = false;
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      // Get the Brep face
      var brep_face = gs.Object(0).Face();
      if (null == brep_face)
        return Result.Failure;

      // Verify the Brep face is planar
      var tolerance = doc.ModelAbsoluteTolerance;
      if (!brep_face.IsPlanar(tolerance))
      {
        RhinoApp.WriteLine("Surface is not planar.");
        return Result.Nothing;
      }

      // Get normal direction of Brep face
      var u = brep_face.Domain(0).Min;
      var v = brep_face.Domain(1).Min;
      var normal = brep_face.NormalAt(u, v);
      // If the Brep face's orientation is opposite of natural surface orientation,
      // then reverse the normal vector.
      if (brep_face.OrientationIsReversed)
        normal.Reverse();

      // Invert norma; direction of the vector for projection
      normal.Reverse();

      // Create a array of Breps to project onto
      var breps = new Brep[1];
      breps[0] = brep_face.DuplicateFace(true); // Create a single Brep, including trims

      // Create a collection of curves to project
      var curves = new List<Curve>(gc.ObjectCount);
      for (var i = 0; i < gc.ObjectCount; i++)
      {
        var curve = gc.Object(i).Curve();
        if (null != curve)
          curves.Add(curve);
      }

      // Do the projection
      var projected_curves = Curve.ProjectToBrep(curves, breps, normal, tolerance);

      // Add the results to the document
      foreach (var crv in projected_curves)
        doc.Objects.AddCurve(crv);

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
