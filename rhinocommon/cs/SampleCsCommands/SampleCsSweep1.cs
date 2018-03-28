using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsSweep1 : Command
  {
    /// <summary>
    /// History recording version.
    /// This field is used to ensure the version of the replay function matches
    /// the version that originally created the geometry.
    /// </summary>
    private const int HISTORY_VERSION = 20140805;

    public SampleCsSweep1()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsSweep1"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      const ObjectType geometryFilter =  ObjectType.Curve;

      GetObject get_rail = new GetObject();
      get_rail.SetCommandPrompt("Select rail curve");
      get_rail.GeometryFilter = geometryFilter;
      //get_rail.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      get_rail.SubObjectSelect = false;
      get_rail.Get();
      if (get_rail.CommandResult() != Result.Success)
        return get_rail.CommandResult();

      ObjRef rail_objref = get_rail.Object(0);
      Curve rail_curve = rail_objref.Curve();
      if (null == rail_curve)
        return Result.Failure;

      GetObject get_shape = new GetObject();
      get_shape.SetCommandPrompt("Select cross section curve");
      get_shape.GeometryFilter = geometryFilter;
      //get_shape.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      get_shape.SubObjectSelect = false;
      get_shape.EnablePreSelect(false, false);
      get_shape.DeselectAllBeforePostSelect = false;
      get_shape.Get();
      if (get_shape.CommandResult() != Result.Success)
        return get_shape.CommandResult();

      ObjRef shape_objref = get_shape.Object(0);
      Curve shape_curve = shape_objref.Curve();
      if (null == shape_curve)
        return Result.Failure;

      double tolerance = doc.ModelAbsoluteTolerance;

      Brep[] brep = Brep.CreateFromSweep(rail_curve, shape_curve, false, tolerance);
      if (null == brep || 0 == brep.Length)
        return Result.Failure;

      // Create a history record
      HistoryRecord history = new HistoryRecord(this, HISTORY_VERSION);
      WriteHistory(history, rail_objref, shape_objref, tolerance);

      for (int i = 0; i < brep.Length; i++)
        doc.Objects.AddBrep(brep[i], null, history, false);

      doc.Views.Redraw();
      
      return Result.Success;
    }

    /// <summary>
    /// Rhino calls the virtual ReplayHistory functions to to remake an objects when inputs have changed.  
    /// </summary>
    protected override bool ReplayHistory(ReplayHistoryData replay)
    {
      ObjRef rail_objref = null;
      ObjRef shape_objref = null;
      double tolerance = RhinoMath.UnsetValue;

      if (!ReadHistory(replay, ref rail_objref, ref shape_objref, ref tolerance))
        return false;

      Rhino.Geometry.Curve rail_curve = rail_objref.Curve();
      if (null == rail_curve)
        return false;

      Rhino.Geometry.Curve shape_curve = shape_objref.Curve();
      if (null == shape_curve)
        return false;

      if (!Rhino.RhinoMath.IsValidDouble(tolerance))
        tolerance = RhinoDoc.ActiveDoc.ModelAbsoluteTolerance;

      Brep[] brep = Brep.CreateFromSweep(rail_curve, shape_curve, false, tolerance);
      if (null == brep || 0 == brep.Length)
        return false;

      for (int i = 0; i < brep.Length; i++)
      {
        if (null != replay.Results[i])
          replay.Results[i].UpdateToBrep(brep[i], null);
        else
          break;
      }

      return true;
    }

    /// <summary>
    /// Read a history record and extract the references for the antecedent objects.
    /// </summary>
    private bool ReadHistory(ReplayHistoryData replay, ref ObjRef railObjRef, ref ObjRef shapeObjRef, ref double tolerance)
    {
      if (HISTORY_VERSION != replay.HistoryVersion)
        return false;

      railObjRef = replay.GetRhinoObjRef(0);
      if (null == railObjRef)
        return false;

      shapeObjRef = replay.GetRhinoObjRef(1);
      if (null == shapeObjRef)
        return false;

      if (!replay.TryGetDouble(2, out tolerance))
        return false;

      return true;
    }

    /// <summary>
    /// Write a history record referencing the antecedent objects
    /// The history should contain all the information required to reconstruct the input.
    /// This may include parameters, options, or settings.
    /// </summary>
    private bool WriteHistory(HistoryRecord history, ObjRef railObjRef, ObjRef shapeObjRef, double tolerance)
    {
      if (null == railObjRef || null == shapeObjRef)
        return false;

      if (!history.SetObjRef(0, railObjRef))
        return false;

      if (!history.SetObjRef(1, shapeObjRef))
        return false;

      if (!history.SetDouble(2, tolerance))
        return false;

      return true;
    }
  }
}
