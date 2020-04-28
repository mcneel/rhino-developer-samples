using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsDuplicateBorder : Command
  {
    public override string EnglishName => "SampleCsDuplicateBorder";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var filter = ObjectType.Surface | ObjectType.PolysrfFilter;
      ObjRef objref;
      var rc = RhinoGet.GetOneObject("Select surface or polysurface", false, filter, out objref);
      if (rc != Result.Success || objref == null)
        return rc;

      var rhobj = objref.Object();
      var brep = objref.Brep();
      if (rhobj == null || brep == null)
        return Result.Failure;

      rhobj.Select(false);

      var curves = new List<Curve>();
      foreach (var edge in brep.Edges)
      {
        // Find only the naked edges 
        if (edge.Valence == EdgeAdjacency.Naked)
        {
          var crv = edge.DuplicateCurve();
          if (crv.IsLinear())
            crv = new LineCurve(crv.PointAtStart, crv.PointAtEnd);
          curves.Add(crv);
        }
      }

      var tol = 2.1 * doc.ModelAbsoluteTolerance;
      var output = Curve.JoinCurves(curves, tol);
      for (var i = 0; i < output.Length; i++)
      {
        var id = doc.Objects.AddCurve(output[i]);
        doc.Objects.Select(id);
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}
