using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("4628427b-eb6c-4eb0-93b6-9139626ad232")]
  public class SampleCsBlock : Command
  {
    public SampleCsBlock()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsBlock"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select objects to define block
      var go = new Rhino.Input.Custom.GetObject();
      go.SetCommandPrompt("Select objects to define block");
      go.ReferenceObjectSelect = false;
      go.SubObjectSelect = false;
      go.GroupSelect = true;

      // Phantoms, grips, lights, etc., cannot be in blocks.
      var forbidden_geometry_filter = Rhino.DocObjects.ObjectType.Light |
        Rhino.DocObjects.ObjectType.Grip | Rhino.DocObjects.ObjectType.Phantom;
      var geometry_filter = forbidden_geometry_filter ^ Rhino.DocObjects.ObjectType.AnyObject;
      go.GeometryFilter = geometry_filter;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Rhino.Commands.Result.Success)
        return go.CommandResult();

      // Block base point
      Rhino.Geometry.Point3d base_point;
      var rc = Rhino.Input.RhinoGet.GetPoint("Block base point", false, out base_point);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      // Block definition name
      string idef_name = "";
      rc = Rhino.Input.RhinoGet.GetString("Block definition name", false, ref idef_name);
      if (rc != Rhino.Commands.Result.Success)
        return rc;
      // Validate block name
      idef_name = idef_name.Trim();
      if (string.IsNullOrEmpty(idef_name))
        return Rhino.Commands.Result.Nothing;

      // See if block name already exists
      Rhino.DocObjects.InstanceDefinition existing_idef = doc.InstanceDefinitions.Find(idef_name, true);
      if (existing_idef != null)
      {
        Rhino.RhinoApp.WriteLine("Block definition {0} already exists", idef_name);
        return Rhino.Commands.Result.Nothing;
      }

      // Gather all of the selected objects
      var geometry = new System.Collections.Generic.List<Rhino.Geometry.GeometryBase>();
      var attributes = new System.Collections.Generic.List<Rhino.DocObjects.ObjectAttributes>();
      for (int i = 0; i < go.ObjectCount; i++)
      {
        var rhinoObject = go.Object(i).Object();
        if (rhinoObject != null)
        {
          geometry.Add(rhinoObject.Geometry);
          attributes.Add(rhinoObject.Attributes);
        }
      }

      // Gather all of the selected objects
      int idef_index = doc.InstanceDefinitions.Add(idef_name, string.Empty, base_point, geometry, attributes);

      if (idef_index < 0)
      {
        Rhino.RhinoApp.WriteLine("Unable to create block definition", idef_name);
        return Rhino.Commands.Result.Failure;
      }

      return Rhino.Commands.Result.Success;
    }
  }
}
