using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;

namespace examples_cs
{
  public class CopyGroupsCommand : Command
  {
    public override string EnglishName { get { return "csCopyGroups"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new Rhino.Input.Custom.GetObject();
      go.SetCommandPrompt("Select objects to copy in place");
      go.GroupSelect = true;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var xform = Transform.Identity;
      var group_map = new Dictionary<int, int>();

      foreach (var obj_ref in go.Objects())
      {
        if (obj_ref != null)
        {
          var obj = obj_ref.Object();
          var duplicate = doc.Objects.Transform(obj_ref.ObjectId, xform, false);
          RhinoUpdateObjectGroups(ref obj, ref group_map);
        } 
      }
      doc.Views.Redraw();
      return Result.Success;
    }

    static void RhinoUpdateObjectGroups(ref RhinoObject obj, ref Dictionary<int, int> group_map)
    {
      if (obj == null) return;

      int attrib_group_count = obj.Attributes.GroupCount;
      if (attrib_group_count == 0) return;

      var doc = obj.Document;
      if (doc == null) return;

      var groups = doc.Groups;

      int group_count = groups.Count;
      if (group_count == 0) return;

      if (group_map.Count == 0)
        for (int i = 0; i < group_count; i++)
          group_map.Add(i, -1);

      var attributes = obj.Attributes;
      var group_list = attributes.GetGroupList();
      if (group_list == null) return;
      attrib_group_count = group_list.Length;

      for (int i = 0; i < attrib_group_count; i++)
      {
        int old_group_index = group_list[i];
        int new_group_index = group_map[old_group_index];
        if (new_group_index == -1)
        {
          new_group_index = doc.Groups.Add();
          group_map[old_group_index] = new_group_index;
        }
        group_list[i] = new_group_index;
      }

      attributes.RemoveFromAllGroups();
      for (int i = 0; i < attrib_group_count; i++)
        attributes.AddToGroup(group_list[i]);

      obj.CommitChanges();
    }
  }
}