using System;
using Rhino;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public static class SampleCsUtilities
  {
    /// <summary>
    /// Tests if a Brep evaluates to a box.
    /// </summary>
    /// <param name="brep">The Brep to examine.</param>
    /// <param name="tolerance">A tolerance, the document tolerance is sufficient.</param>
    /// <returns>True if the Brep is box, false otherwise.</returns>
    public static bool IsBrepBox(Brep brep, double tolerance)
    {
      var normals = new Vector3d[6];

      var rc = null != brep;

      if (rc)
        rc = brep.IsSolid;

      if (rc)
        rc = brep.Faces.Count == 6;

      if (rc)
      {
        for (var i = 0; rc && i < 6; i++)
        {
          if (brep.Faces[i].TryGetPlane(out var plane, tolerance))
          {
            normals[i] = plane.Normal;
            normals[i].Unitize();
          }
          else
          {
            rc = false;
          }
        }
      }

      if (rc)
      {
        for (var i = 0; rc && i < 6; i++)
        {
          var count = 0;
          for (var j = 0; rc && j < 6; j++)
          {
            var dot = Math.Abs(normals[i] * normals[j]);
            if (Math.Abs(dot) <= tolerance)
              continue;
            if (Math.Abs(dot - 1.0) <= tolerance)
              count++;
            else
              rc = false;
          }

          if (rc)
            rc = count == 2;
        }
      }

      return rc;
    }

    /// <summary>
    /// Calculates the u, v, n directions of a NURBS curve control point
    /// similar to the method used by the MoveUVN command.
    /// </summary>
    /// <param name="nurb">The NURBS curve to evaluate.</param>
    /// <param name="cvIndex">The index of the control point to evaluate.</param>
    /// <param name="uDir">The u direction.</param>
    /// <param name="vDir">The v direction.</param>
    /// <param name="nDir">The n direction.</param>
    /// <returns>true if successful, false otherwise</returns>
    public static bool GetNurbsCurveControlPointDirections(
      NurbsCurve nurb,
      int cvIndex,
      out Vector3d uDir,
      out Vector3d vDir,
      out Vector3d nDir
    )
    {
      uDir = vDir = nDir = Vector3d.Unset;
      var rc = false;
      if (null != nurb && cvIndex >= 0 && cvIndex < nurb.Points.Count)
      {
        var t = nurb.GrevilleParameter(cvIndex);
        if (RhinoMath.IsValidDouble(t))
        {
          if (t < nurb.Domain.Min)
            t += nurb.Domain.Length;

          uDir = nurb.TangentAt(t);

          var kappa = nurb.CurvatureAt(t);
          if (nurb.TryGetPlane(out Plane plane))
          {
            vDir = plane.ZAxis;
            nDir = Vector3d.CrossProduct(uDir, vDir);
            vDir.Unitize();
          }
          else if (kappa.Unitize())
          {
            nDir = kappa;
            vDir = Vector3d.CrossProduct(nDir, uDir);
            nDir.Unitize();
          }
          else
          {
            vDir.PerpendicularTo(uDir);
            vDir.Unitize();
            nDir = Vector3d.CrossProduct(uDir, vDir);
            nDir.Unitize();
          }

          const double tol = 1E-15;

          if (Math.Abs(uDir.X) <= tol)
            uDir.X = 0.0;
          if (Math.Abs(uDir.Y) <= tol)
            uDir.Y = 0.0;
          if (Math.Abs(uDir.Z) <= tol)
            uDir.Z = 0.0;

          if (Math.Abs(vDir.X) <= tol)
            vDir.X = 0.0;
          if (Math.Abs(vDir.Y) <= tol)
            vDir.Y = 0.0;
          if (Math.Abs(vDir.Z) <= tol)
            vDir.Z = 0.0;

          if (Math.Abs(nDir.X) <= tol)
            nDir.X = 0.0;
          if (Math.Abs(nDir.Y) <= tol)
            nDir.Y = 0.0;
          if (Math.Abs(nDir.Z) <= tol)
            nDir.Z = 0.0;

          rc = true;
        }
      }

      return rc;
    }
  }
}
