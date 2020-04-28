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
      var go = new GetObject();
      go.SetCommandPrompt("Select holes");
      go.GeometryFilter = ObjectType.BrepLoop;
      go.GeometryAttributeFilter = GeometryAttributeFilter.InnerLoop;
      go.GetMultiple(1, 0);
      if (go.CommandResult() == Result.Success)
      {
        for (var i = 0; i < go.ObjectCount; i++)
        {
          var obj_ref = go.Object(i);
          var ci = obj_ref.GeometryComponentIndex;
          if (ci.ComponentIndexType != ComponentIndexType.BrepLoop)
            return Result.Failure;

          var brep = obj_ref.Brep();
          if (null == brep)
            return Result.Failure;

          var loop = brep.Loops[ci.Index];
          if (null == loop)
            return Result.Failure;

          for (var lti = 0; lti < loop.Trims.Count; lti++)
          {
            var ti = loop.Trims[lti].TrimIndex;
            var trim = brep.Trims[ti];
            if (null != trim )
            {
              var edge = brep.Edges[trim.Edge.EdgeIndex];
              if (null != edge)
              {
                // TODO: do somethign with edge curve.
                // In this case, we'll just add a copy to the document.
                var curve = edge.DuplicateCurve();
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

	    var loop = geometry as BrepLoop;
	    if (null == loop)
		      return false;

	    var brep = loop.Brep;
	    if (null == brep)
		    return false;

      return IsHoleLoop(brep, loop.LoopIndex, true, false, m_tolerance);
    }

    public static bool IsHoleLoop(Brep brep, int loopIndex, bool bPlanarCheck, bool bBoundaryCheck, double tolerance)
    {
      var loop = (null != brep) ? brep.Loops[loopIndex] : null;
      if (null == loop )
        return false;

      if (BrepLoopType.Inner != loop.LoopType)
        return false;

      var face = brep.Faces[loop.Face.FaceIndex];
      if (null == face)
        return false;

      var srf = face.UnderlyingSurface();
      if (null == srf)
        return false;

      if (bPlanarCheck)
      {
        if ((1 != srf.Degree(0) && 1 != srf.Degree(1)) || 
            (1 != srf.SpanCount(0) && 1 != srf.SpanCount(1)) || 
            !srf.IsPlanar(tolerance))
        return false;
      }
      
      for (var lti = 0; lti < loop.Trims.Count; lti++)
      {
        var ti = loop.Trims[lti].TrimIndex;
        var trim = brep.Trims[ti];
        if (null == trim )
          return false;

        var edge = brep.Edges[trim.Edge.EdgeIndex];
        if (null == edge)
          return false;

        var edge_ti = edge.TrimIndices();
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
            if (bBoundaryCheck )
              return false;

            int other_ti;
            if (ti == edge_ti[0])
              other_ti = edge_ti[1];
            else if (ti == edge_ti[1])
              other_ti = edge_ti[0];
            else
              return false;

            var other_trim = brep.Trims[other_ti];
            if (null == other_trim)
              return false;

            var other_loop = brep.Loops[other_trim.Loop.LoopIndex];
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
