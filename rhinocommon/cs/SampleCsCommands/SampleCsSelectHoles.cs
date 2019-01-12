using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleGetHole : GetObject
  {
    private readonly double m_tolerance;

    public SampleGetHole(double tolerance)
    {
      m_tolerance = tolerance;
    }

    public override bool CustomGeometryFilter(RhinoObject rhObject, GeometryBase geometry, ComponentIndex componentIndex)
    {
      if (!IsBrepComponentIndex(componentIndex))
        return false;
      if (!(geometry is BrepLoop loop))
        return false;
      var brep = loop.Brep;
      return null != brep && IsHoleLoop(brep, componentIndex.Index, true, false, m_tolerance);
    }

    public static bool IsBrepComponentIndex(ComponentIndex componentIndex)
    {
      switch (componentIndex.ComponentIndexType)
      {
        case ComponentIndexType.BrepEdge:
        case ComponentIndexType.BrepFace:
        case ComponentIndexType.BrepLoop:
        case ComponentIndexType.BrepTrim:
        case ComponentIndexType.BrepVertex:
          {
            if (componentIndex.Index >= 0)
              return true;
          }
          break;
      }
      return false;
    }

    public bool IsHoleLoop(Brep brep, int loopIndex, bool bPlanarCheck, bool bBoundaryCheck, double tolerance)
    {
      if (null == brep)
        return false;
      if (loopIndex < 0 || loopIndex >= brep.Loops.Count)
        return false;
      var loop = brep.Loops[loopIndex];
      if (null == loop)
        return false;
      if (loop.LoopType != BrepLoopType.Inner)
        return false;
      var face = loop.Face;
      if (null == face)
        return false;
      var srf = face.UnderlyingSurface();
      if (null == srf)
        return false;

      if (bPlanarCheck)
      {
        if (
          (srf.Degree(0) != 1 && srf.Degree(1) != 1) ||
          (srf.SpanCount(0) != 1 && srf.SpanCount(1) != 1) ||
          !srf.IsPlanar(tolerance)
          )
          return false;
      }

      for (var lti = 0; lti < loop.Trims.Count; lti++)
      {
        var trim = loop.Trims[lti];
        if (null == trim)
          return false;
        var ti = trim.TrimIndex;
        var edge = trim.Edge;
        if (null == edge)
          return false;

        switch (edge.TrimCount)
        {
          case 1:
            if (edge.TrimIndices()[0] != ti)
              return false;
            break;
          case 2:
            {
              if (bBoundaryCheck)
                return false;
              var other_ti = -1;
              if (edge.TrimIndices()[0] == ti)
                other_ti = edge.TrimIndices()[1];
              else if (edge.TrimIndices()[1] == ti)
                other_ti = edge.TrimIndices()[0];
              else
                return false;
              var other_trim = brep.Trims[other_ti];
              if (null == other_trim)
                return false;
              var other_loop = brep.Loops[other_trim.Loop.LoopIndex];
              if (null == other_loop)
                return false;
              if (other_loop.LoopType != BrepLoopType.Outer)
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

  public class SampleCsSelectHoles : Command
  {
    public override string EnglishName => "SampleCsSelectHoles";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new SampleGetHole(doc.ModelAbsoluteTolerance);
      go.SetCommandPrompt("Select holes in one planar surface");
      go.GeometryFilter = ObjectType.BrepLoop;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // TODO...

      return Result.Success;
    }
  }
}