using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsRTree : Command
  {
    public override string EnglishName => "SampleCsRTree";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go0 = new GetObject();
      go0.SetCommandPrompt("Select first set of meshes for interference check");
      go0.GeometryFilter = ObjectType.Mesh;
      go0.SubObjectSelect = false;
      go0.GroupSelect = true;
      go0.EnablePreSelect(false, true);
      go0.GetMultiple(1, 0);
      if (go0.CommandResult() != Result.Success)
        return go0.CommandResult();

      var mesh_list0 = new List<Mesh>(go0.ObjectCount);
      foreach (var obj_ref in go0.Objects())
      {
        var mesh = obj_ref.Mesh();
        if (null == mesh)
          return Result.Failure;
        mesh_list0.Add(mesh);
      }

      var go1 = new GetObject();
      go1.SetCommandPrompt("Select second set of meshes for interference check");
      go1.GeometryFilter = ObjectType.Mesh;
      go1.SubObjectSelect = false;
      go1.GroupSelect = true;
      go1.EnablePreSelect(false, true);
      go1.DeselectAllBeforePostSelect = false;
      go1.GetMultiple(1, 0);
      if (go1.CommandResult() != Result.Success)
        return go1.CommandResult();

      var mesh_list1 = new List<Mesh>(go1.ObjectCount);
      foreach (var obj_ref in go1.Objects())
      {
        var mesh = obj_ref.Mesh();
        if (null == mesh)
          return Result.Failure;
        mesh_list1.Add(mesh);
      }

      var check = new SampleInterferenceCheck(mesh_list0.ToArray(), mesh_list1.ToArray());
      var rc = check.BoxCheck(doc.ModelAbsoluteTolerance);

      var count = check.EventCount;
      if (0 == count)
        RhinoApp.WriteLine("0 interference events.");
      else if (1 == count)
        RhinoApp.WriteLine("1 interference events.");
      else
        RhinoApp.WriteLine("{0} interference events.", count);

      if (count > 0)
      {
        doc.Objects.UnselectAll(true);

        foreach (var result in check.EventResults)
        {
          go0.Object(result.Item1).Object().Select(true, true);
          go1.Object(result.Item2).Object().Select(true, true);
        }

        doc.Views.Redraw();
      }

      return Result.Success;
    }


    /// <summary>
    /// Mesh interference checker
    /// </summary>
    internal class SampleInterferenceCheck
    {
      private readonly List<Mesh>[] m_mesh_list;
      private readonly List<Tuple<int, int>> m_events;

      /// <summary>
      ///  Public constructor
      /// </summary>
      public SampleInterferenceCheck(IEnumerable<Mesh> meshes0, IEnumerable<Mesh> meshes1)
      {
        m_mesh_list = new List<Mesh>[2];
        m_events = new List<Tuple<int, int>>();

        m_mesh_list[0] = new List<Mesh>();
        m_mesh_list[1] = new List<Mesh>();

        m_mesh_list[0].AddRange(meshes0);
        m_mesh_list[1].AddRange(meshes1);
      }

      /// <summary>
      /// Do a bounding box check
      /// </summary>
      public bool BoxCheck(double tolerance)
      {
        if (tolerance <= 0)
          tolerance = RhinoMath.SqrtEpsilon;

        var box_tree = new RTree[2];
        box_tree[0] = new RTree();
        box_tree[1] = new RTree();

        var ok = new bool[] { false, false };
        Parallel.For(0, m_mesh_list.Length, i => { ok[i] = MakeBoxTree(m_mesh_list[i].ToArray(), box_tree[i]); });
        var rc = ok[0] && ok[1];

        if (rc)
          rc = RTree.SearchOverlaps(box_tree[0], box_tree[1], tolerance, SearchCallback);

        return rc;
      }

      /// <summary>
      /// Returns the number of "hit" events
      /// </summary>
      public int EventCount => m_events.Count;

      /// <summary>
      /// Returns the "hit" events.
      /// </summary>
      public List<Tuple<int, int>> EventResults => m_events;

      /// <summary>
      /// Fills in an RTree
      /// </summary>
      private bool MakeBoxTree(IEnumerable<Mesh> meshes, RTree tree)
      {
        if (null == meshes || null == tree)
          return false;

        var rc = false;
        var element = 0;
        foreach (var mesh in meshes)
        {
          if (null == mesh)
            continue;
          var bbox = mesh.GetBoundingBox(false);
          if (bbox.IsValid)
          {
            tree.Insert(bbox, element++);
            rc = true;
          }
        }

        return rc;
      }

      /// <summary>
      /// RTree.SearchOverlap callback
      /// </summary>
      private void SearchCallback(object sender, RTreeEventArgs e)
      {
        m_events.Add(new Tuple<int, int>(e.Id, e.IdB));
      }

    }
  }
}