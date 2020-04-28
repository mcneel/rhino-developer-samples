using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsHistoryDivide : Command
  {
    static int _historyVersion = 20121101; 

    public override string EnglishName => "SampleCsHistoryDivide";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const Rhino.DocObjects.ObjectType filter = Rhino.DocObjects.ObjectType.Curve;
      Rhino.DocObjects.ObjRef objref;
      Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetOneObject("Select curve to divide", false, filter, out objref);
      if (rc != Rhino.Commands.Result.Success || objref == null)
        return rc;

      Rhino.Geometry.Curve curve = objref.Curve();
      if (null == curve || curve.IsShort(Rhino.RhinoMath.ZeroTolerance))
        return Rhino.Commands.Result.Failure;

      int segmentCount = 2;
      rc = Rhino.Input.RhinoGet.GetInteger("Number of segments", false, ref segmentCount, 2, 100);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      Rhino.Geometry.Point3d[] points;
      curve.DivideByCount(segmentCount, true, out points);
      if (null == points)
        return Rhino.Commands.Result.Failure;

      // Create a history record
      Rhino.DocObjects.HistoryRecord history = new Rhino.DocObjects.HistoryRecord(this, _historyVersion);
      WriteHistory(history, objref, segmentCount, points.Length);

      for (int i = 0; i < points.Length; i++)
        doc.Objects.AddPoint(points[i], null, history, false);

      doc.Views.Redraw();

      return Rhino.Commands.Result.Success;
    }

    /// <summary>
    /// Rhino calls this function to remake objects when inputs have changed.  
    /// </summary>
    protected override bool ReplayHistory(Rhino.DocObjects.ReplayHistoryData replay)
    {
      Rhino.DocObjects.ObjRef objref = null;
      int segmentCount = 0;
      int pointCount = 0;

      if (!ReadHistory(replay, ref objref, ref segmentCount, ref pointCount))
        return false;

      Rhino.Geometry.Curve curve = objref.Curve();
      if (null == curve)
        return false;

      if (pointCount != replay.Results.Length)
        return false;

      Rhino.Geometry.Point3d[] points;
      curve.DivideByCount(segmentCount, true, out points);
      if (null == points)
        return false;

      for (int i = 0; i < points.Length; i++)
        replay.Results[i].UpdateToPoint(points[i], null);

      return true;
    }

    private bool ReadHistory(Rhino.DocObjects.ReplayHistoryData replay, ref Rhino.DocObjects.ObjRef objref, ref int segmentCount, ref int pointCount)
    {
      if (_historyVersion != replay.HistoryVersion)
        return false;

      objref = replay.GetRhinoObjRef(0);
      if (null == objref)
        return false;

      if (!replay.TryGetInt(1, out segmentCount))
        return false;

      if (!replay.TryGetInt(2, out pointCount))
        return false;

      return true;
    }

    private bool WriteHistory(Rhino.DocObjects.HistoryRecord history, Rhino.DocObjects.ObjRef objref, int segmentCount, int pointCount)
    {
      if (!history.SetObjRef(0, objref))
        return false;

      if (!history.SetInt(1, segmentCount))
        return false;

      if (!history.SetInt(2, pointCount))
        return false;

      return true;
    }
  }
}
