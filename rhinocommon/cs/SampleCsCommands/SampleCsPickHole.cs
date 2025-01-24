using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsPickHole : Command
  {
    public override string EnglishName => "SampleCsPickHole";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select holes");
      go.GeometryFilter = ObjectType.BrepLoop;
      go.GeometryAttributeFilter = GeometryAttributeFilter.InnerLoop;
      go.GetMultiple(1, 0);
      if (go.CommandResult() == Result.Success)
      {
        for (int i = 0; i < go.ObjectCount; i++)
        {
          ObjRef obj_ref = go.Object(i);
          ComponentIndex ci = obj_ref.GeometryComponentIndex;
          if (ci.ComponentIndexType != ComponentIndexType.BrepLoop)
            return Result.Failure;

          Brep brep = obj_ref.Brep();
          if (null == brep)
            return Result.Failure;

          BrepLoop loop = brep.Loops[ci.Index];
          if (null == loop)
            return Result.Failure;

          for (int lti = 0; lti < loop.Trims.Count; lti++)
          {
            int ti = loop.Trims[lti].TrimIndex;
            BrepTrim trim = brep.Trims[ti];
            if (null != trim)
            {
              BrepEdge edge = brep.Edges[trim.Edge.EdgeIndex];
              if (null != edge)
              {
                // TODO: do somethign with edge curve.
                // In this case, we'll just add a copy to the document.
                Curve curve = edge.DuplicateCurve();
                if (null != curve)
                  doc.Objects.AddCurve(curve);
              }
            }
          }
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  internal class GetHoleLoop : GetObject
  {
    private readonly double m_tolerance;

    public GetHoleLoop(double tolerance)
    {
      m_tolerance = tolerance;
      GeometryFilter = ObjectType.BrepLoop;
      GeometryAttributeFilter = GeometryAttributeFilter.InnerLoop;
    }

    public override bool CustomGeometryFilter(RhinoObject rhObject, GeometryBase geometry, ComponentIndex componentIndex)
    {
      if (null == rhObject || null == geometry)
        return false;

      if (ComponentIndexType.BrepLoop != componentIndex.ComponentIndexType)
        return false;

      BrepLoop loop = geometry as BrepLoop;
      if (null == loop)
        return false;

      Brep brep = loop.Brep;
      if (null == brep)
        return false;

      return IsHoleLoop(brep, loop.LoopIndex, true, false, m_tolerance);
    }

    public static bool IsHoleLoop(Brep brep, int loopIndex, bool bPlanarCheck, bool bBoundaryCheck, double tolerance)
    {
      BrepLoop loop = (null != brep) ? brep.Loops[loopIndex] : null;
      if (null == loop)
        return false;

      if (BrepLoopType.Inner != loop.LoopType)
        return false;

      BrepFace face = brep.Faces[loop.Face.FaceIndex];
      if (null == face)
        return false;

      Surface srf = face.UnderlyingSurface();
      if (null == srf)
        return false;

      if (bPlanarCheck)
      {
        if ((1 != srf.Degree(0) && 1 != srf.Degree(1)) ||
            (1 != srf.SpanCount(0) && 1 != srf.SpanCount(1)) ||
            !srf.IsPlanar(tolerance))
          return false;
      }

      for (int lti = 0; lti < loop.Trims.Count; lti++)
      {
        int ti = loop.Trims[lti].TrimIndex;
        BrepTrim trim = brep.Trims[ti];
        if (null == trim)
          return false;

        BrepEdge edge = brep.Edges[trim.Edge.EdgeIndex];
        if (null == edge)
          return false;

        int[] edge_ti = edge.TrimIndices();
        if (0 == edge_ti.Length)
          return false;

        switch (edge.TrimCount)
        {
          case 1:
            {
              if (ti != edge_ti[0])
                return false;
            }
            break;
          case 2:
            {
              if (bBoundaryCheck)
                return false;

              int other_ti;
              if (ti == edge_ti[0])
                other_ti = edge_ti[1];
              else if (ti == edge_ti[1])
                other_ti = edge_ti[0];
              else
                return false;

              BrepTrim other_trim = brep.Trims[other_ti];
              if (null == other_trim)
                return false;

              BrepLoop other_loop = brep.Loops[other_trim.Loop.LoopIndex];
              if (null == other_loop)
                return false;

              if (BrepLoopType.Outer != other_loop.LoopType)
                return false;

              if (other_loop.Face.FaceIndex == loop.Face.FaceIndex)
                return false;
            }
            break;

          default:
            return false;
        }
      }

      return true;
    }
  }

}
