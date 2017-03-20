using System;
using System.Collections.Generic;
using System.IO;
using Rhino;
using Rhino.Commands;
using Rhino.FileIO;
using Rhino.Geometry;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("3f9ea6fe-77ce-4fdf-bf8e-a0433ba660cb")]
  public class SampleCsImportBlockFromFile : Command
  {
    public override string EnglishName
    {
      get { return "SampleCsImportBlockFromFile"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Name of block to import
      var gs = new GetString();
      gs.SetCommandPrompt("Name of block to import");
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var idef_name = gs.StringResult().Trim();
      if (string.IsNullOrEmpty(idef_name))
        return Result.Nothing;

      if (null != doc.InstanceDefinitions.Find(idef_name, true))
      {
        RhinoApp.WriteLine("Block already exists.");
        return Result.Nothing;
      }

      // Name of file to import block from
      gs.SetCommandPrompt("Name of file to import block from");
      gs.Get();
      if (gs.CommandResult() != Result.Success)
        return gs.CommandResult();

      var idef_filename = gs.StringResult().Trim();
      if (string.IsNullOrEmpty(idef_filename))
        return Result.Nothing;

      if (!File.Exists(idef_filename))
      {
        RhinoApp.WriteLine("File not found.");
        return Result.Failure;
      }

      using (var file = File3dm.Read(idef_filename))
      {
        // Find the instance defintion named "idef_name"
        var idef_index = -1;
        for (var i = 0; i < file.InstanceDefinitions.Count; i++)
        {
          if (string.Equals(idef_name, file.InstanceDefinitions[i].Name, StringComparison.OrdinalIgnoreCase))
          {
            idef_index = i;
            break;
          }
        }

        if (idef_index < 0)
        {
          RhinoApp.WriteLine("Block not found in file.");
          return Result.Failure;
        }

        // Find and duplicate all of the instance definition geometry
        // Note, this sample only imports the geoemtry, not the geometry's
        // attributes (layer, material, etc.).
        var idef = file.InstanceDefinitions[idef_index];
        var iref_objects = idef.GetObjectIds();
        var geometry = new List<GeometryBase>(iref_objects.Length);

        foreach (var obj_id in iref_objects)
        {
          foreach (var obj in  file.Objects)
          {
            if (obj_id == obj.Attributes.ObjectId)
              geometry.Add(obj.Geometry.Duplicate());
          }
        }

        if (geometry.Count == 0)
          return Result.Failure;

        // Add (e.g. import) the instance defintion
        idef_index = doc.InstanceDefinitions.Add(idef_name, idef.Description, Point3d.Origin, geometry);
        if (idef_index < 0)
        {
          RhinoApp.WriteLine("Unable to import block from file.");
          return Result.Failure;
        }
      }

      return Result.Success;
    }
  }
}
