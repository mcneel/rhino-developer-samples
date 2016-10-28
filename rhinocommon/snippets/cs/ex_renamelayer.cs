using Rhino;
using Rhino.Input;
using Rhino.Commands;
using Rhino.DocObjects;
using System;
using System.Linq;

namespace examples_cs
{
  public class RenameLayerCommand : Command
  {
    public override string EnglishName { get { return "csRenameLayer"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string layer_name = "";
      var rc = RhinoGet.GetString("Name of layer to rename", true, ref layer_name);
      if (rc != Result.Success)
        return rc;
      if (String.IsNullOrWhiteSpace(layer_name))
        return Result.Nothing;
     
      // because of sublayers it's possible that more than one layer has the same name
      // so simply calling doc.Layers.Find(layerName) isn't good enough.  If "layerName" returns
      // more than one layer then present them to the user and let him decide.
      var matching_layers = (from layer in doc.Layers
                             where layer.Name == layer_name
                             select layer).ToList<Layer>();

      Layer layer_to_rename = null;
      if (matching_layers.Count == 0)
      {
        RhinoApp.WriteLine("Layer \"{0}\" does not exist.", layer_name);
        return Result.Nothing;
      }
      else if (matching_layers.Count == 1)
      {
        layer_to_rename = matching_layers[0];
      }
      else if (matching_layers.Count > 1)
      {
        for (int i = 0; i < matching_layers.Count; i++)
        {
          RhinoApp.WriteLine("({0}) {1}", i+1, matching_layers[i].FullPath.Replace("::", "->"));
        }
        int selected_layer = -1;
        rc = RhinoGet.GetInteger("which layer?", true, ref selected_layer);
        if (rc != Result.Success)
          return rc;
        if (selected_layer > 0 && selected_layer <= matching_layers.Count)
          layer_to_rename = matching_layers[selected_layer - 1];
        else return Result.Nothing;
      }

      if (layer_to_rename == null)
        return Result.Nothing;

      layer_name = "";
      rc = RhinoGet.GetString("New layer name", true, ref layer_name);
      if (rc != Result.Success)
        return rc;
      if (String.IsNullOrWhiteSpace(layer_name))
        return Result.Nothing;

      layer_to_rename.Name = layer_name;
      if (!layer_to_rename.CommitChanges())
        return Result.Failure;
      return Result.Success;
    }
  }
}