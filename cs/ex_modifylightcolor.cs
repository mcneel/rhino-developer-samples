using Rhino;
using Rhino.DocObjects;
using Rhino.Commands;
using Rhino.Input;
using Rhino.UI;

namespace examples_cs
{
  public class ChangeLightColorCommand : Rhino.Commands.Command
  {
    public override string EnglishName
    {
      get { return "csLightColor"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      ObjRef obj_ref;
      var rc = RhinoGet.GetOneObject("Select light to change color", true,
        ObjectType.Light, out obj_ref);
      if (rc != Result.Success)
        return rc;
      var light = obj_ref.Light();
      if (light == null)
        return Result.Failure;

      var diffuse_color = light.Diffuse;
      if (Dialogs.ShowColorDialog(ref diffuse_color))
      {
        light.Diffuse = diffuse_color;
      }

      doc.Lights.Modify(obj_ref.ObjectId, light);
      return Result.Success;
    }
  }
}