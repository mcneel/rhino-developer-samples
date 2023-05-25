using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;

namespace SampleCsUserData.Commands
{
  public class SampleCsAddLayerUserData : Command
  {
    public override string EnglishName => "SampleCsAddLayerUserData";

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

      if (!(layer.UserData.Find(typeof(SampleCsUserDataObject)) is SampleCsUserDataObject ud))
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

        var new_layer = new Layer();
        new_layer.CopyAttributesFrom(layer);
        new_layer.UserData.Add(ud);
        doc.Layers.Modify(new_layer, layer.Index, false);
      }
      else
      {
        RhinoApp.WriteLine("{0} = {1}", ud.Description, ud.Notes);
      }

      return Result.Success;
    }
  }
}
