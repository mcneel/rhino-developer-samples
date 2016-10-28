using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;

namespace examples_cs
{
  public class DuplicateObjectCommand : Command
  {
    public override string EnglishName { get { return "csDuplicateObject"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      var rc = RhinoGet.GetOneObject("Select object to duplicate", false, ObjectType.AnyObject, out obj_ref);
      if (rc != Result.Success)
        return rc;
      var rhino_object = obj_ref.Object();

      var geometry_base = rhino_object.DuplicateGeometry();
      if (geometry_base != null)
        if (doc.Objects.Add(geometry_base) != Guid.Empty)
          doc.Views.Redraw();

      return Result.Success;
    }
  }
}