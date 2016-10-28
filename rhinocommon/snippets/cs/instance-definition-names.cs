partial class Examples
{
  public static Result InstanceDefinitionNames(RhinoDoc doc)
  {
    var instance_definition_names = (from instance_definition in doc.InstanceDefinitions
                                     where instance_definition != null && !instance_definition.IsDeleted
                                     select instance_definition.Name);

    foreach (var n in instance_definition_names)
      RhinoApp.WriteLine("Instance definition = {0}", n);

    return Result.Success;
  }
}
