using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsClashObjects : Command
  {
    private double m_distance = 1.0;

    public override string EnglishName => "SampleCsClashObjects";

    private double Distance
    {
      get => m_distance;
      set => m_distance = value <= 0 ? 1.0 : value;
    }

    /// <summary>
    /// Called by Rhino when the user runs the command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var setA = new List<RhinoObject>();
      var setB = new List<RhinoObject>();
      var rc = SelectClashSets(setA, setB);
      if (rc != Result.Success)
        return rc;

      var gd = new GetNumber();
      gd.SetCommandPrompt("Clearance distance");
      gd.SetDefaultNumber(Distance);
      gd.SetLowerLimit(0.0, true);
      gd.Get();
      if (gd.CommandResult() != Result.Success)
        return gd.CommandResult();

      Distance = gd.Number();

      // Search for object clashes
      var clashes = MeshClash.Search(setA, setB, Distance, MeshType.Render, MeshingParameters.FastRenderMesh);

      if (clashes.Length == 1)
        RhinoApp.WriteLine("1 clash found.");
      else
        RhinoApp.WriteLine("{0} clashes found.", clashes.Length);

      if (clashes.Length > 1)
      {
        foreach (var clash in clashes)
          doc.Objects.AddPoints(clash.HitPoints);
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    /// <summary>
    /// Selects Rhino objects to clash.
    /// </summary>
    private Result SelectClashSets(List<RhinoObject> setA, List<RhinoObject> setB)
    {
      var filter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Extrusion | ObjectType.SubD;

      var go0 = new GetObject();
      go0.SetCommandPrompt("Select first set of objects for clash detection");
      go0.GeometryFilter = filter;
      go0.GroupSelect = true;
      go0.SubObjectSelect = false;
      go0.GetMultiple(1, 0);
      if (go0.CommandResult() != Result.Success)
        return go0.CommandResult();

      foreach (var objref in go0.Objects())
      {
        var rh_obj = objref.Object();
        if (null == rh_obj)
          return Result.Failure;
        setA.Add(rh_obj);
      }

      var go1 = new GetObject();
      go1.SetCommandPrompt("Select second set of objects for clash detection");
      go1.GeometryFilter = filter;
      go1.GroupSelect = true;
      go1.SubObjectSelect = false;
      go1.EnablePreSelect(false, true);
      go1.DeselectAllBeforePostSelect = false;
      go1.GetMultiple(1, 0);
      if (go1.CommandResult() != Result.Success)
        return go1.CommandResult();

      foreach (var objref in go1.Objects())
      {
        var rh_obj = objref.Object();
        if (null == rh_obj)
          return Result.Failure;
        setB.Add(rh_obj);
      }

      return Result.Success;
    }
  }
}