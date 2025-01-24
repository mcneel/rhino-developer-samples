using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;
using System;

namespace SampleCsCommands
{
  public class SampleCsBooleanDifference4 : Command
  {
    public override string EnglishName => "SampleCsBooleanDifference4";

    /// <summary>
    /// History recording version.
    /// This field is used to ensure the version of the replay function matches
    /// the version that originally created the geometry.
    /// </summary>
    private const int HISTORY_VERSION = 20220323;

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go0 = new GetObject();
      go0.SetCommandPrompt("Select surface or polysurface to subtract from");
      go0.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go0.SubObjectSelect = false;
      go0.Get();
      if (go0.CommandResult() != Result.Success)
        return go0.CommandResult();

      ObjRef objref0 = go0.Object(0);
      RhinoObject rh_obj = objref0.Object();
      Brep brep0 = objref0.Brep();
      if (null == rh_obj || null == brep0)
        return Result.Failure;

      GetObject go1 = new GetObject();
      go1.SetCommandPrompt("Select surface or polysurface to subtract with");
      go1.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go1.SubObjectSelect = false;
      go1.EnablePreSelect(false, true);
      go1.DeselectAllBeforePostSelect = false;
      go1.Get();
      if (go1.CommandResult() != Result.Success)
        return go1.CommandResult();

      ObjRef objref1 = go1.Object(0);
      Brep brep1 = objref1.Brep();
      if (null == brep1)
        return Result.Failure;

      double tolerance = doc.ModelAbsoluteTolerance;

      Brep[] out_breps = Brep.CreateBooleanDifference(brep0, brep1, tolerance);
      if (null == out_breps || 0 == out_breps.Length)
      {
        RhinoApp.WriteLine("Boolean difference failed.");
        return Result.Failure;
      }

      // Create a history record
      HistoryRecord history = new HistoryRecord(this, HISTORY_VERSION);
      WriteHistory(history, objref0, objref1, tolerance);

      ObjectAttributes attributes = rh_obj.Attributes.Duplicate();
      attributes.ObjectId = Guid.Empty;
      attributes.RemoveFromAllGroups();

      foreach (Brep brep in out_breps)
        doc.Objects.AddBrep(brep, attributes, history, false);

      doc.Views.Redraw();
      return Result.Success;
    }

    /// <summary>
    /// Rhino calls this function to remake objects when inputs have changed.  
    /// </summary>
    protected override bool ReplayHistory(ReplayHistoryData replay)
    {
      ObjRef objref0 = null;
      ObjRef objref1 = null;
      double tolerance = RhinoMath.UnsetValue;

      if (!ReadHistory(replay, ref objref0, ref objref1, ref tolerance))
        return false;

      Brep brep0 = objref0.Brep();
      if (null == brep0)
        return false;

      Brep brep1 = objref1.Brep();
      if (null == brep1)
        return false;

      if (!RhinoMath.IsValidDouble(tolerance))
        return false;

      Brep[] out_breps = Brep.CreateBooleanDifference(brep0, brep1, tolerance);
      if (null == out_breps || 0 == out_breps.Length)
        return false;

      for (int i = 0; i < out_breps.Length; i++)
        replay.Results[i].UpdateToBrep(out_breps[i], null);

      return true;
    }

    /// <summary>
    /// ReadHistory
    /// </summary>
    private bool ReadHistory(ReplayHistoryData replay, ref ObjRef objref0, ref ObjRef objref1, ref double tolerance)
    {
      if (HISTORY_VERSION != replay.HistoryVersion)
        return false;

      objref0 = replay.GetRhinoObjRef(0);
      if (null == objref0)
        return false;

      objref1 = replay.GetRhinoObjRef(1);
      if (null == objref1)
        return false;

      if (!replay.TryGetDouble(2, out tolerance))
        return false;

      return true;
    }

    /// <summary>
    /// WriteHistory
    /// </summary>
    private bool WriteHistory(HistoryRecord history, ObjRef objref0, ObjRef objref1, double tolerance)
    {
      if (!history.SetObjRef(0, objref0))
        return false;

      if (!history.SetObjRef(1, objref1))
        return false;

      if (!history.SetDouble(2, tolerance))
        return false;

      return true;
    }
  }
}