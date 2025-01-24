using Rhino;
using Rhino.Commands;

namespace SampleCsUserData.Commands
{
  public class SampleCsQueryLayerUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsQueryLayerUserData"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      int layer_index = doc.Layers.FindByFullPath("Default", Rhino.RhinoMath.UnsetIntIndex);
      if (layer_index < 0)
      {
        RhinoApp.WriteLine("Default layer not found.");
        return Result.Nothing;
      }

      Rhino.DocObjects.Layer layer = doc.Layers[layer_index];
      if (null == layer)
        return Result.Failure;

      SampleCsUserDataObject ud = layer.UserData.Find(typeof(SampleCsUserDataObject)) as SampleCsUserDataObject;
      if (null != ud)
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      else
        RhinoApp.WriteLine("Layer user data not found.");

      return Result.Success;
    }
  }
}
