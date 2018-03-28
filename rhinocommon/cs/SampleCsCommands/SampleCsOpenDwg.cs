using System;
using System.IO;
using Rhino;
using Rhino.Commands;
using Rhino.Input;

namespace SampleCsCommands
{
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsOpenDwg : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsOpenDwg"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string filename = null;
      var rc = RhinoGet.GetString("Name of AutoCAD DWG file to open", false, ref filename);
      if (rc != Result.Success)
        return rc;

      filename = filename.Trim();
      if (string.IsNullOrEmpty(filename))
        return Result.Nothing;

      if (!File.Exists(filename))
      {
        RhinoApp.WriteLine("File not found");
        return Result.Failure;
      }

      var extension = Path.GetExtension(filename);
      if (string.IsNullOrEmpty(extension))
        return Result.Nothing;

      if (!string.Equals(extension, ".dwg", StringComparison.InvariantCultureIgnoreCase))
      {
        RhinoApp.WriteLine("Not an AutoCAD DWG file.");
        return Result.Failure;
      }

      // Make sure to surround filename string with double-quote characters
      // in case the path contains spaces.
      var script = string.Format("_-Open \"{0}\" _Enter", filename);
      RhinoApp.RunScript(script, false);

      return Result.Success;
    }
  }
}
