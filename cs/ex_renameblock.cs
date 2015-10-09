using Rhino;
using Rhino.Input;
using Rhino.Commands;

namespace examples_cs
{
  public class RenameBlockCommand : Command
  {
    public override string EnglishName { get { return "csRenameBlock"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Get the name of the insance definition to rename
      var instance_definition_name = "";
      var rc = RhinoGet.GetString("Name of block to rename", true, ref instance_definition_name);
      if (rc != Result.Success)
        return rc;
      if (string.IsNullOrWhiteSpace(instance_definition_name))
        return Result.Nothing;
     
      // Verify instance definition exists
      var instance_definition = doc.InstanceDefinitions.Find(instance_definition_name, true);
      if (instance_definition == null) {
        RhinoApp.WriteLine("Block \"{0}\" not found.", instance_definition_name);
        return Result.Nothing;
      }

      // Verify instance definition is rename-able
      if (instance_definition.IsDeleted || instance_definition.IsReference) {
        RhinoApp.WriteLine("Unable to rename block \"{0}\".", instance_definition_name);
        return Result.Nothing;
      }
     
      // Get the new instance definition name
      string instance_definition_new_name = "";
      rc = RhinoGet.GetString("Name of block to rename", true, ref instance_definition_new_name);
      if (rc != Result.Success)
        return rc;
      if (string.IsNullOrWhiteSpace(instance_definition_new_name))
        return Result.Nothing;

      // Verify the new instance definition name is not already in use
      var existing_instance_definition = doc.InstanceDefinitions.Find(instance_definition_new_name, true);
      if (existing_instance_definition != null && !existing_instance_definition.IsDeleted) {
        RhinoApp.WriteLine("Block \"{0}\" already exists.", existing_instance_definition);
        return Result.Nothing;
      }
     
      // change the block name
      if (!doc.InstanceDefinitions.Modify(instance_definition.Index, instance_definition_new_name, instance_definition.Description, true)) {
        RhinoApp.WriteLine("Could not rename {0} to {1}", instance_definition.Name, instance_definition_new_name);
        return Result.Failure;
      }

      return Result.Success;
    }
  }
}