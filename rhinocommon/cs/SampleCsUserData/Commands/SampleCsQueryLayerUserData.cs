using Rhino;
using Rhino.Commands;

namespace SampleCsUserData.Commands
{
  public class SampleCsQueryLayerUserData : Command
  {
    public override string EnglishName => "SampleCsQueryLayerUserData";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var layer_index = doc.Layers.FindByFullPath("Default", RhinoMath.UnsetIntIndex);
      if (layer_index < 0)
      {
        RhinoApp.WriteLine("Default layer not found.");
        return Result.Nothing;
      }

      var layer = doc.Layers[layer_index];
      if (null == layer)
        return Result.Failure;

      if (layer.UserData.Find(typeof(SampleCsUserDataObject)) is SampleCsUserDataObject ud)
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      else
        RhinoApp.WriteLine("Layer user data not found.");

      return Result.Success;
    }
  }
}
