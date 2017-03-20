using System;
using System.IO;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.FileIO;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("62b8f03f-2462-4b5a-b284-31733cf4235c")]
  public class SampleCsLayerPathName : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsLayerPathName"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var dialog = new OpenFileDialog
      {
        Filter = @"Rhino 3D Models (*.3dm)|*.3dm",
        DefaultExt = "3dm"
      };

      var rc = dialog.ShowDialog();
      if (rc != DialogResult.OK)
        return Result.Cancel;

      var filename = dialog.FileName;
      if (string.IsNullOrEmpty(filename) || !File.Exists(filename))
        return Result.Failure;

      var file = File3dm.Read(filename);
      if (null == file)
        return Result.Failure;

      // Layers do not store their full path name. Full layer path names are generated, 
      // as needed, by a function the document's layer table. Basically, the function 
      // check to see if the layer has a parent and, if so, prepends the parent's name
      // to the layer name. And then the parent layer check to see if it has a parent, 
      // etc., until no parent is found.

      // The layer's collection in File3dm is just an ignorant collection. So if you want 
      // to generate full layer path names from layers in in File3dm, you need to write your
      // own function.

      for (var i = 0; i < file.Layers.Count; i++)
      {
        string full_name = null;
        if (GetLayerPathName(file, i, ref full_name))
          RhinoApp.WriteLine(full_name);
      }

      return Result.Success;
    }

    /// <summary>
    /// Get the layer path name.
    /// </summary>
    private bool GetLayerPathName(File3dm file, int layerIndex, ref string fullName)
    {
      return GetLayerPathName(file, layerIndex, @"::", ref fullName);
    }

    /// <summary>
    /// Get the layer path name.
    /// </summary>
    private bool GetLayerPathName(File3dm file, int layerIndex, string delimeter, ref string fullName)
    {
      if (null == file || layerIndex < 0 || layerIndex >= file.Layers.Count)
        return false;

      string name = null;
      for (var i = 0; i < file.Layers.Count; i++)
      {
        var layer = file.Layers[layerIndex];
        var layer_name = layer.Name;
        if (string.IsNullOrEmpty(name))
        {
          name = layer_name;
        }
        else
        {
          var child_name = name;
          name = layer_name;
          if (!string.IsNullOrEmpty(delimeter))
            name += delimeter;
          name += child_name;
        }

        if (layer.ParentLayerId == Guid.Empty)
          break;

        layerIndex = FindLayerFromId(file, layer.ParentLayerId);
        if (layerIndex < 0)
          break;
      }

      fullName = name;

      return true;
    }

    /// <summary>
    /// Search for a layer with a matching id.
    /// </summary>
    private int FindLayerFromId(File3dm file, Guid id)
    {
      if (null != file && id != Guid.Empty)
      {
        for (var i = 0; i < file.Layers.Count; i++)
        {
          if (file.Layers[i].Id == id)
            return i;
        }
      }
      return -1;
    }
  }
}
