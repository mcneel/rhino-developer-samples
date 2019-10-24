using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Geometry.Intersect;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsClashMeshes : Command
  {
    private double m_distance = 1.0;

    public override string EnglishName => "SampleCsClashMeshes";

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
      ClashObjects(doc, setA, setB, Distance);

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

    /// <summary>
    /// Search for object clashes.
    /// </summary>
    private Result ClashObjects(RhinoDoc doc, List<RhinoObject> setA, List<RhinoObject> setB, double distance)
    {
      // Get the meshes for each Rhino object
      var map = new Dictionary<Mesh, RhinoObject>[2];
      var mp = MeshingParameters.FastRenderMesh;
      for (var dex = 0; dex < 2; dex++)
      {
        map[dex] = new Dictionary<Mesh, RhinoObject>();
        var rh_objects = (0 == dex) ? setA : setB;
        for (var i = 0; i < rh_objects.Count; i++)
        {
          if (null != rh_objects[i])
          {
            var meshes = GetGeometryMeshes(rh_objects[i].Geometry, mp);
            if (meshes.Length > 0)
            {
              for (var j = 0; j < meshes.Length; j++)
                map[dex].Add(meshes[j], rh_objects[i]);
            }
          }
        }
      }
      if (0 == map[0].Count || 0 == map[1].Count)
        return Result.Failure;

      // Do the mesh clash detection
      var clashes = MeshClash.Search(map[0].Keys.ToArray(), map[1].Keys.ToArray(), distance, 0);

      if (clashes.Length == 1)
        RhinoApp.WriteLine("1 clash found.");
      else
        RhinoApp.WriteLine("{0} clashes found.", clashes.Length);

      foreach (var clash in clashes)
      {
        if (map[0].TryGetValue(clash.MeshA, out RhinoObject a) && map[1].TryGetValue(clash.MeshB, out RhinoObject b))
        {
          RhinoApp.WriteLine("{0} clashes with {1} at ({2})", a.Id, b.Id, clash.ClashPoint);
          doc.Objects.AddPoint(clash.ClashPoint);
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    /// <summary>
    /// Gets one or more meshes from a geometry object.
    /// </summary>
    private Mesh[] GetGeometryMeshes(GeometryBase geometry, MeshingParameters mp)
    {
      var rc = new Mesh[0];
      if (null != geometry)
      {
        if (null == mp)
          mp = MeshingParameters.FastRenderMesh;

        switch (geometry.ObjectType)
        {
          case ObjectType.Surface:
            {
              var surface = geometry as Surface;
              if (null != surface)
              {
                var mesh = Mesh.CreateFromSurface(surface, mp);
                if (null != mesh)
                  rc = new Mesh[] { mesh };
              }
            }
            break;

          case ObjectType.Brep:
            {
              var brep = geometry as Brep;
              if (null != brep)
              {
                var meshes = Mesh.CreateFromBrep(brep, mp);
                if (null != meshes && meshes.Length > 0)
                  rc = meshes;
              }
            }
            break;

          case ObjectType.Extrusion:
            {
              var extrusion = geometry as Extrusion;
              if (null != extrusion)
              {
                var brep = extrusion.ToBrep();
                if (null != brep)
                {
                  var meshes = Mesh.CreateFromBrep(brep, mp);
                  if (null != meshes && meshes.Length > 0)
                    rc = meshes;
                }
              }
            }
            break;

          case ObjectType.Mesh:
            {
              var mesh = geometry as Mesh;
              if (null != mesh)
                rc = new Mesh[] { mesh };
            }
            break;

          case ObjectType.SubD:
            {
              var subd = geometry as SubD;
              if (null != subd)
              {
                // 2 == ON_SubDDisplayParameters::CourseDensity
                var mesh = Mesh.CreateFromSubD(geometry as SubD, 2);
                if (null != mesh)
                  rc = new Mesh[] { mesh };
              }
            }
            break;
        }
      }
      return rc;
    }


  }
}