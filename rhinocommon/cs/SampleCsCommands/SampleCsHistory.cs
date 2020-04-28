using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsHistory : Command
  {
    /// <summary>
    /// The only instance of the SampleCsHistory command.
    /// </summary>
    static SampleCsHistory g_command_instance;

    /// <summary>
    /// History recording version.
    /// This field is used to ensure the version of the replay function matches
    /// the version that originally created the geometry.
    /// </summary>
    private const int HISTORY_VERSION = 20131107;

    /// <summary>
    /// Private data member
    /// </summary>
    private int m_segment_count = 2;

    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsHistory()
    {
      g_command_instance = this;
    }

    /// <summary>
    /// Returns the only instance of the SampleCsHistory command.
    /// </summary>
    public static SampleCsHistory Instance => g_command_instance;

    /// <summary>
    /// Returns the English name of this command.
    /// </summary>
    public override string EnglishName => "SampleCsHistory";

    /// <summary>
    /// Rhino calls this function to run the command.
    /// </summary>
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

      rc = Rhino.Input.RhinoGet.GetInteger("Number of segments", false, ref m_segment_count, 2, 100);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      Rhino.Geometry.Point3d[] points;
      curve.DivideByCount(m_segment_count, true, out points);
      if (null == points)
        return Rhino.Commands.Result.Failure;

      // Create a history record
      Rhino.DocObjects.HistoryRecord history = new Rhino.DocObjects.HistoryRecord(this, HISTORY_VERSION);
      WriteHistory(history, objref, m_segment_count, points.Length);

      for (int i = 0; i < points.Length; i++)
        doc.Objects.AddPoint(points[i], null, history, false);

      doc.Views.Redraw();

      return Rhino.Commands.Result.Success;
    }

    /// <summary>
    /// Rhino calls the virtual ReplayHistory functions to to remake an objects when inputs have changed.  
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

    /// <summary>
    /// Read a history record and extract the references for the antecedent objects.
    /// </summary>
    private bool ReadHistory(Rhino.DocObjects.ReplayHistoryData replay, ref Rhino.DocObjects.ObjRef objref, ref int segmentCount, ref int pointCount)
    {
      if (HISTORY_VERSION != replay.HistoryVersion)
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

    /// <summary>
    /// Write a history record referencing the antecedent objects
    /// The history should contain all the information required to reconstruct the input.
    /// This may include parameters, options, or settings.
    /// </summary>
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
