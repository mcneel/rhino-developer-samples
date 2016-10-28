using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace examples_cs
{
  public class SelectGroupObjecCommand : Command
  {
    public override string EnglishName
    {
      get { return "csSelectObjectsInObjectGroups"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      var rs = RhinoGet.GetOneObject(
        "Select object", false, ObjectType.AnyObject, out obj_ref);
      if (rs != Result.Success)
        return rs;
      var rhino_object = obj_ref.Object();
      if (rhino_object == null)
        return Result.Failure;

      var rhino_object_groups = rhino_object.Attributes.GetGroupList().DefaultIfEmpty(-1);

      var selectable_objects= from obj in doc.Objects.GetObjectList(ObjectType.AnyObject)
                              where obj.IsSelectable(true, false, false, false)
                              select obj;

      foreach (var selectable_object in selectable_objects)
      {
        foreach (var group in selectable_object.Attributes.GetGroupList())
        {
          if (rhino_object_groups.Contains(group))
          {
              selectable_object.Select(true);
              continue;
          }
        }
      }
      doc.Views.Redraw();
      return Result.Success;
    }
  }
}