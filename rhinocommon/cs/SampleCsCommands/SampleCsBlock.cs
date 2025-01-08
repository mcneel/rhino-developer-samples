using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System.Collections.Generic;

namespace SampleCsCommands
{
  public class SampleCsBlock : Command
  {
    public override string EnglishName => "SampleCsBlock";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select objects to define block
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select objects to define block");
      go.ReferenceObjectSelect = false;
      go.SubObjectSelect = false;
      go.GroupSelect = true;

      // Phantoms, grips, lights, etc., cannot be in blocks.
      ObjectType forbidden_geometry_filter = ObjectType.Light | ObjectType.Grip | ObjectType.Phantom;
      ObjectType geometry_filter = forbidden_geometry_filter ^ ObjectType.AnyObject;
      go.GeometryFilter = geometry_filter;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      // Block base point
      Point3d base_point;
      Result rc = RhinoGet.GetPoint("Block base point", false, out base_point);
      if (rc != Result.Success)
        return rc;

      // Block definition name
      string idef_name = "";
      rc = RhinoGet.GetString("Block definition name", false, ref idef_name);
      if (rc != Result.Success)
        return rc;

      // Validate block name
      idef_name = idef_name.Trim();
      if (string.IsNullOrEmpty(idef_name))
        return Result.Nothing;

      // See if block name already exists
      InstanceDefinition existing_idef = doc.InstanceDefinitions.Find(idef_name);
      if (existing_idef != null)
      {
        RhinoApp.WriteLine("Block definition {0} already exists", idef_name);
        return Result.Nothing;
      }

      // Gather all of the selected objects
      List<GeometryBase> geometry = new List<GeometryBase>();
      List<ObjectAttributes> attributes = new List<ObjectAttributes>();
      for (int i = 0; i < go.ObjectCount; i++)
      {
        RhinoObject rh_object = go.Object(i).Object();
        if (rh_object != null)
        {
          geometry.Add(rh_object.Geometry);
          attributes.Add(rh_object.Attributes);
        }
      }

      // Gather all of the selected objects
      int idef_index = doc.InstanceDefinitions.Add(idef_name, string.Empty, base_point, geometry, attributes);
      if (idef_index < 0)
      {
        RhinoApp.WriteLine("Unable to create block definition", idef_name);
        return Result.Failure;
      }

      return Result.Success;
    }
  }
}
