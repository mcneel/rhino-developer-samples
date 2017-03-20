using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("f34abe7d-050c-4067-8548-2f449670e0d6")]
  public class SampleCsDuplicateBorder : Command
  {
    public SampleCsDuplicateBorder()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsDuplicateBorder"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.DocObjects.ObjectType filter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter;
      Rhino.DocObjects.ObjRef objref = null;
      Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface", false, filter, out objref);
      if (rc != Rhino.Commands.Result.Success || objref == null)
        return rc;

      Rhino.DocObjects.RhinoObject rhobj = objref.Object();
      Rhino.Geometry.Brep brep = objref.Brep();
      if (rhobj == null || brep == null)
        return Rhino.Commands.Result.Failure;

      rhobj.Select(false);

      System.Collections.Generic.List<Rhino.Geometry.Curve> curves = new System.Collections.Generic.List<Rhino.Geometry.Curve>();
      foreach (Rhino.Geometry.BrepEdge edge in brep.Edges)
      {
        // Find only the naked edges 
        if (edge.Valence == Rhino.Geometry.EdgeAdjacency.Naked)
        {
          Rhino.Geometry.Curve crv = edge.DuplicateCurve();
          if (null != crv)
            curves.Add(crv);
        }
      }

      double tol = 2.1 * doc.ModelAbsoluteTolerance;
      Rhino.Geometry.Curve[] output = Rhino.Geometry.Curve.JoinCurves(curves, tol);
      for (int i = 0; i < output.Length; i++)
      {
        Guid id = doc.Objects.AddCurve(output[i]);
        doc.Objects.Select(id);
      }

      doc.Views.Redraw();
      return Result.Success;
    }
  }
}
