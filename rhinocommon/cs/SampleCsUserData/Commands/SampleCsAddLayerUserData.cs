using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  public class SampleCsAddLayerUserData : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsAddLayerUserData"; }
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
      if (null == ud)
      {
        var gs = new GetString();
        gs.SetCommandPrompt("Layer notes");
        gs.GetLiteralString();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        ud = new SampleCsUserDataObject
        {
          Notes = gs.StringResult()
        };

        layer.UserData.Add(ud);
      }
      else
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      }

      return Result.Success;
    }
  }
}
