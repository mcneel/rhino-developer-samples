using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsDuplicateBorder : Command
  {
    public override string EnglishName => "SampleCsDuplicateBorder";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjectType filter = ObjectType.Surface | ObjectType.PolysrfFilter;
      ObjRef objref;
      Result rc = RhinoGet.GetOneObject("Select surface or polysurface", false, filter, out objref);
      if (rc != Result.Success || objref == null)
        return rc;

      RhinoObject rhobj = objref.Object();
      Brep brep = objref.Brep();
      if (rhobj == null || brep == null)
        return Result.Failure;

      rhobj.Select(false);

      List<Curve> curves = new List<Curve>();
      foreach (BrepEdge edge in brep.Edges)
      {
        // Find only the naked edges 
        if (edge.Valence == EdgeAdjacency.Naked)
        {
          Curve crv = edge.DuplicateCurve();
          if (crv.IsLinear())
            crv = new LineCurve(crv.PointAtStart, crv.PointAtEnd);
          curves.Add(crv);
        }
      }

      double tol = 2.1 * doc.ModelAbsoluteTolerance;
      Curve[] output = Curve.JoinCurves(curves, tol);
      for (int i = 0; i < output.Length; i++)
      {
        System.Guid id = doc.Objects.AddCurve(output[i]);
        doc.Objects.Select(id);
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}
