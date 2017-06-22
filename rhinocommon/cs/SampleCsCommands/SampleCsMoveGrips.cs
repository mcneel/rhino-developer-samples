using System;
using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("cc239c91-5818-494c-aa7e-2866af0eba71")]
  public class SampleCsMoveGrips : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsMoveGrips"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select grips to move");
      go.GeometryFilter = ObjectType.Grip;
      go.EnablePreSelect(true, true);
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move from");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();
 
      var from = gp.Point();
 
      gp.SetCommandPrompt("Point to move to");
      gp.SetBasePoint(from, true);
      gp.DrawLineFromPoint(from, true);
      gp.Get();
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();
 
      var to = gp.Point();

      var dir = to - from;
      if (dir.IsTiny(doc.ModelAbsoluteTolerance))
        return Result.Nothing;

      var xform = Transform.Translation(dir);

      var helper = new SampleCsGripHelper(doc);
      helper.AddGripObjects(go);
      helper.TransformAndUpdate(xform);

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  /// <summary>
  /// SampleCsGripHelper class
  /// </summary>
  public class SampleCsGripHelper
  {
    private readonly RhinoDoc m_doc;
    private readonly List<GripObject> m_grips;
    private readonly List<RhinoObject> m_grip_owners;

    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsGripHelper(RhinoDoc doc)
    {
      if (null == doc) throw new ArgumentNullException("doc");
      m_doc = doc;
      m_grips = new List<GripObject>(16);
      m_grip_owners = new List<RhinoObject>(16);
    }

    /// <summary>
    /// Adds grip objects selected by an object getter
    /// </summary>
    public int AddGripObjects(GetObject go)
    {
      if (null == go)
        return 0;

      var grip_count = m_grips.Count;

      foreach (var obj_ref in go.Objects())
      {
        var grip_obj = obj_ref.Object() as GripObject;
        if (null != grip_obj)
          AddGripObject(grip_obj);
      }

      return m_grips.Count - grip_count;
    }

    /// <summary>
    /// Adds grip objects
    /// </summary>
    public bool AddGripObject(GripObject gripObject)
    {
      if (null == gripObject)
        return false;

      var rh_object = m_doc.Objects.Find(gripObject.OwnerId);
      if (null == rh_object)
        return false;

      m_grips.Add(gripObject);

      var index = m_grip_owners.FindIndex(x => x.Id == rh_object.Id);
      if (index < 0)
        m_grip_owners.Add(rh_object);

      return true;
    }

    /// <summary>
    /// Transforms the grips and updates the grip owners
    /// </summary>
    public bool TransformAndUpdate(Transform xform)
    {
      if (!xform.IsValid || xform == Transform.Identity)
        return false;

      foreach (var grip in m_grips)
        grip.Move(xform);

      foreach (var rh_obj in m_grip_owners)
        m_doc.Objects.GripUpdate(rh_obj, true);

      return true;
    }
  }
}