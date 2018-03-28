using System;
using System.Drawing;
using System.IO;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.FileIO;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsWrite3dmFile : Command
  {
    public override string EnglishName => "SampleCsWrite3dmFile";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var path = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
      var filename = Path.Combine(path, "SampleCsWrite3dmFile.3dm");

      Result rc;

      using (var file = new File3dm())
      {
        var layer = new Layer { Name = "Default", Color = Color.Black };
        file.AllLayers.Add(layer);

        var layer_index = file.AllLayers.Count - 1;
        var attributes = new ObjectAttributes { LayerIndex = layer_index };

        for (var x = 0; x < 100; x++)
        {
          var line_curve = new LineCurve(new Point3d(x, 0, 0), new Point3d(x, 1, 0));
          if (line_curve.IsValid)
            file.Objects.AddCurve(line_curve, attributes);
        }

        rc = file.Write(filename, 6) ? Result.Success : Result.Failure;
      }

      return rc;
    }
  }
}