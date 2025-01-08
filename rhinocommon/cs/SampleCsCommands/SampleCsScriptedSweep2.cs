using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;
using System;
using System.Collections.Generic;
using System.Text;

namespace SampleCsCommands
{
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsScriptedSweep2 : Command
  {
    public override string EnglishName => "SampleCsScriptedSweep2";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Guid[] rails = new Guid[2];

      GetObject go = new GetObject();
      go.SetCommandPrompt("Select first rail");
      go.GeometryFilter = ObjectType.Curve;
      go.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve;
      go.DisablePreSelect();
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      RhinoObject obj = go.Object(0).Object();
      if (null == obj)
        return Result.Failure;
      rails[0] = obj.Id;

      go.SetCommandPrompt("Select second rail");
      go.DeselectAllBeforePostSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      obj = go.Object(0).Object();
      if (null == obj)
        return Result.Failure;
      rails[1] = obj.Id;

      GetObject gc = new GetObject();
      gc.SetCommandPrompt("Select cross section curves");
      gc.GeometryFilter = ObjectType.Curve;
      gc.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve;
      gc.DisablePreSelect();
      gc.DeselectAllBeforePostSelect = false;
      gc.SubObjectSelect = false;
      gc.GetMultiple(1, 0);
      if (gc.CommandResult() != Result.Success)
        return gc.CommandResult();

      Guid[] curves = new Guid[gc.ObjectCount];
      for (int i = 0; i < gc.ObjectCount; i++)
      {
        obj = gc.Object(i).Object();
        if (null == obj)
          return Result.Failure;
        curves[i] = obj.Id;
      }

      Guid[] object_ids = ScriptedSweep2(doc, rails[0], rails[1], curves);
      doc.Views.Redraw();

      return Result.Success;
    }

    private Guid[] ScriptedSweep2(RhinoDoc doc, Guid rail1, Guid rail2, IEnumerable<Guid> shapes)
    {
      doc.Objects.UnselectAll(true);

      StringBuilder sb = new StringBuilder("_-Sweep2 ");
      sb.Append(string.Format("_SelID {0} ", rail1));
      sb.Append(string.Format("_SelID {0} ", rail2));
      foreach (Guid shape in shapes)
        sb.Append(string.Format("_SelID {0} ", shape));
      sb.Append(string.Format("_Enter "));
      sb.Append(string.Format("_Simplify=_None _Closed=_Yes _MaintainHeight=_Yes _Enter"));

      uint start_sn = Rhino.DocObjects.RhinoObject.NextRuntimeSerialNumber;
      RhinoApp.RunScript(sb.ToString(), false);
      uint end_sn = Rhino.DocObjects.RhinoObject.NextRuntimeSerialNumber;
      if (start_sn == end_sn)
        return new Guid[0];

      List<Guid> object_ids = new List<Guid>();
      for (uint sn = start_sn; sn < end_sn; sn++)
      {
        RhinoObject obj = doc.Objects.Find(sn);
        if (null != obj)
          object_ids.Add(obj.Id);
      }

      return object_ids.ToArray();
    }
  }
}
