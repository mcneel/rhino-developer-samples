using Rhino;
using Rhino.Commands;

namespace SampleCsUserData.Commands
{
  [System.Runtime.InteropServices.Guid("075db042-a92e-40f4-8ca2-5b9c9e243334")]
  public class SampleCsQueryLayerUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsQueryLayerUserData"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var layer_index = doc.Layers.FindByFullPath("Default", Rhino.RhinoMath.UnsetIntIndex);
      if (layer_index < 0)
      {
        RhinoApp.WriteLine("Default layer not found.");
        return Result.Nothing;
      }

      var layer = doc.Layers[layer_index];
      if (null == layer)
        return Result.Failure;

      var ud = layer.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null != ud)
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      else
        RhinoApp.WriteLine("Layer user data not found.");

      return Result.Success;
    }
  }
}
