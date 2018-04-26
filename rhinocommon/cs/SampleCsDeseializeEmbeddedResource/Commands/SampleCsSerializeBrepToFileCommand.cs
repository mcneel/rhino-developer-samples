using System.IO;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using Rhino.Input.Custom;
using Rhino.UI;

namespace SampleCsDeserializeEmbeddedResource.Commands
{
  /// <summary>
  /// SampleCsSerializeBrepToFile test command (e.g. hidden)
  /// </summary>
  //[CommandStyle(Style.Hidden)]
  public class SampleCsSerializeBrepToFileCommand : Command
  {
    public override string EnglishName => "SampleCsSerializeBrepToFile";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Selet surface or polysurface to serialize to a file.");
      go.GeometryFilter = ObjectType.Surface | ObjectType.PolysrfFilter;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var brep = go.Object(0).Brep();
      if (null == brep)
        return Result.Failure;

      string path = null;
      if (mode == RunMode.Interactive)
      {
        var dialog = new SaveFileDialog
        {
          Title = EnglishName,
          Filter = @"Bin Files (*.bin)|*.bin||",
          DefaultExt = "bin"
        };

        if (!dialog.ShowSaveDialog())
          return Result.Cancel;

        path = dialog.FileName;
      }
      else
      {
        var result = RhinoGet.GetString("Save file name", false, ref path);
        if (result != Result.Success)
          return result;
      }

      path = path.Trim();
      if (string.IsNullOrEmpty(path))
        return Result.Nothing;

      if (!Path.HasExtension(path))
        path = Path.ChangeExtension(path, ".bin");

      var rc = SampleCsGeometryHelper.WriteToFile(path, brep);

      return rc ? Result.Success : Result.Failure;
    }
  }
}