using System.Collections.Generic;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  [CommandStyle(Style.ScriptRunner)]
  public class SampleCsLastCreatedObjects : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsLastCreatedObjects"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // This example demonstrates how to retrieve the objects that
      // were created by scripting a Rhino command.

      // 1.) Get the runtime serial number that will be assigned to the
      // next Rhino Object that is created.
      var sn_start = RhinoObject.NextRuntimeSerialNumber;

      // 2.) Script the Rhino command here using Rhino.RhinoApp.RunScript.
      // http://developer.rhino3d.com/guides/rhinocommon/run_rhino_command_from_plugin/

      // 3.) Get the runtime serial number that will be assigned to the
      // next Rhino Object that is created.
      var sn_end = RhinoObject.NextRuntimeSerialNumber;

      // 4.) If the scripted command completed successfully and new objects were
      // added to the document, sn_end will be greater than sn_start. So all
      // that's left to do is find the newly added objects.

      var objects = new List<RhinoObject>();
      for (var sn = sn_start; sn < sn_end; sn++)
      {
        var obj = doc.Objects.Find(sn);
        if (null != obj)
          objects.Add(obj);
      }

      // 5.) TODO: do something with the objects...

      return Result.Success;
    }
  }
}
