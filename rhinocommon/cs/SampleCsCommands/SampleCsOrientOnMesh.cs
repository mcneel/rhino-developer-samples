using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsOrientOnMesh : Command
  {
    /// <summary>
    /// The command name
    /// </summary>
    public override string EnglishName
    {
      get { return "SampleCsOrientOnMesh"; }
    }

    /// <summary>
    /// Called by Rhino to 'run' this command
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select objects to orient
      var go = new GetObject();
      go.SetCommandPrompt("Select objects to orient");
      go.SubObjectSelect = false;
      go.EnableIgnoreGrips(true);
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Point to move from
      var gp = new GetPoint();
      gp.SetCommandPrompt("Point to move from");
      gp.Get();
      if (gp.CommandResult() != Result.Success)
        return gp.CommandResult();

      // Calculate source plane
      var plane = gp.View().ActiveViewport.ConstructionPlane();
      plane.Origin = gp.Point();

      // Mesh to orient on
      var gm = new GetObject();
      gm.SetCommandPrompt("Mesh to orient on");
      gm.GeometryFilter = ObjectType.Mesh;
      gm.EnablePreSelect(false, true);
      gm.DeselectAllBeforePostSelect = false;
      gm.Get();
      if (gm.CommandResult() != Result.Success)
        return gm.CommandResult();

      var mesh = gm.Object(0).Mesh();
      if (null == mesh)
        return Result.Failure;

      // Point on mesh to orient to
      var gpm = new GetPointOnMesh(mesh, plane);
      gpm.SetCommandPrompt("Point on mesh to orient to");
      gpm.AppendObjects(go);
      gpm.Get();
      if (gpm.CommandResult() != Result.Success)
        return gpm.CommandResult();

      // One final calculation
      var xform = new Transform(1);
      if (gpm.CalculateTransform(gpm.View().ActiveViewport, gpm.Point(), ref xform))
      {
        foreach (var objRef in go.Objects())
        {
          var obj = objRef.Object();
          if (null != obj)
            doc.Objects.Transform(obj, xform, true);
        }
        doc.Views.Redraw();
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// Custom GetPoint class
  /// </summary>
  class GetPointOnMesh : GetPoint
  {
    private Mesh m_mesh;
    private Plane m_object_plane;
    private bool m_draw;
    private Transform m_xform;
    private List<RhinoObject> m_objects;

    // Mesh face plane cache
    private Dictionary<int, Plane> m_face_planes; 

    /// <summary>
    /// Public constructor
    /// </summary>
    public GetPointOnMesh(Mesh mesh, Plane plane)
    {
      m_mesh = mesh;
      Constrain(m_mesh, false);
      m_object_plane = plane;
      m_draw = false;
      m_xform = new Transform(1.0);
      m_face_planes = new Dictionary<int, Plane>();
    }

    /// <summary>
    /// Append objects to dynamically draw
    /// </summary>
    public void AppendObjects(GetObject go)
    {
      if (null != go)
      {
        if (null == m_objects)
          m_objects = new List<RhinoObject>();

        foreach (var objRef in go.Objects())
        {
          var obj = objRef.Object();
          if (null != obj)
            m_objects.Add(obj);
        }
      }
    }

    /// <summary>
    /// Calculate the transform
    /// </summary>
    public bool CalculateTransform(RhinoViewport vp, Point3d pt, ref Transform xform)
    {
      var rc = false;
      var mp = m_mesh.ClosestMeshPoint(pt, 0.0);
      if (null != mp)
      {
        Plane meshPlane;
        if (MeshFacePlane(mp, out meshPlane))
        {
          meshPlane.Origin = pt;
          xform = Transform.PlaneToPlane(m_object_plane, meshPlane);
          rc = xform.IsValid;
        }
      }
      return rc;
    }

    /// <summary>
    /// OnMouseMove override
    /// </summary>
    protected override void OnMouseMove(GetPointMouseEventArgs e)
    {
      m_draw = CalculateTransform(e.Viewport, e.Point, ref m_xform);
    }

    /// <summary>
    /// OnDynamicDraw override
    /// </summary>
    protected override void OnDynamicDraw(GetPointDrawEventArgs e)
    {
      if (m_draw)
      {
        foreach (var obj in m_objects)
          e.Display.DrawObject(obj, m_xform);
      }
    }

    /// <summary>
    /// Get a plane from a mesh face
    /// </summary>
    private bool MeshFacePlane(MeshPoint mp, out Plane plane)
    {
      plane = new Plane();
      var rc = false;
      if (null != mp && mp.ComponentIndex.ComponentIndexType == ComponentIndexType.MeshFace)
      {
        var mesh = mp.Mesh;
        if (null != mesh && 0 <= mp.FaceIndex && mp.FaceIndex < mesh.Faces.Count)
        {
          if (m_face_planes.ContainsKey(mp.FaceIndex))
          {
            plane = m_face_planes[mp.FaceIndex];
            rc = true;
          }
          else
          {
            if (mesh.FaceNormals.Count != mesh.Faces.Count)
              mesh.FaceNormals.ComputeFaceNormals();
            if (mesh.FaceNormals.Count == mesh.Faces.Count)
            {
              plane = new Plane(mesh.Vertices[mesh.Faces[mp.FaceIndex].A], mesh.FaceNormals[mp.FaceIndex]);
              rc = plane.IsValid;
              if (rc)
                m_face_planes.Add(mp.FaceIndex, plane);
            }
          }
        }
      }
      return rc;
    }
  }
}
