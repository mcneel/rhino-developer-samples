using System;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("e85a7770-79e9-4cd3-9712-5342d3bca344")]
  public class SampleCsAddNurbsSurface : Command
  {
    public SampleCsAddNurbsSurface()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsAddNurbsSurface"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const bool rational = false;
      const int dim = 3;
      const int u_degree = 2;
      const int v_degree = 3;
      const int u_cv_count = 3;
      const int v_cv_count = 5;
      
      var i = 0;
      var j = 0;

      // The knot vectors do NOT have the 2 superfluous knots
      // at the start and end of the knot vector.  If you are
      // coming from a system that has the 2 superfluous knots,
      // just ignore them when creating NURBS surfaces.
      var u_knot = new double[u_cv_count + u_degree - 1];
      var v_knot = new double[v_cv_count + v_degree - 1];

      // make up a quadratic knot vector with no interior knots
      u_knot[0] = u_knot[1] = 0.0;
      u_knot[2] = u_knot[3] = 1.0;

      // make up a cubic knot vector with one simple interior knot
      v_knot[0] = v_knot[1] = v_knot[2] = 0.0;
      v_knot[3] = 1.5;
      v_knot[4] = v_knot[5] = v_knot[6] = 2.0;

      // Rational control points can be in either homogeneous
      // or euclidean form. Non-rational control points do not
      // need to specify a weight.  
      var cv = new Point3d[u_cv_count, v_cv_count];
      for (i = 0; i < u_cv_count; i++)
      {
        for (j = 0; j < v_cv_count; j++)
          cv[i, j] = new Point3d {X = i, Y = j, Z = i - j};
      }

      var nurbs_surface = NurbsSurface.Create(dim, rational, u_degree + 1, v_degree + 1, u_cv_count, v_cv_count);

      for (i = 0; i < nurbs_surface.KnotsU.Count; i++)
        nurbs_surface.KnotsU[i] = u_knot[i];

      for (i = 0; i < nurbs_surface.KnotsV.Count; i++)
        nurbs_surface.KnotsV[i] = v_knot[i];

      for (i = 0; i < nurbs_surface.Points.CountU; i++)
      {
        for (j = 0; j < nurbs_surface.Points.CountV; j++)
          nurbs_surface.Points.SetPoint(i, j, cv[i, j]);
      }

      var rc = Result.Failure;
      if (nurbs_surface.IsValid)
      {
        doc.Objects.AddSurface(nurbs_surface);
        doc.Views.Redraw();
        rc = Result.Success;
      }

      return rc;
    }
  }
}
