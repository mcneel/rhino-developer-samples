partial class Examples
{
  public static Rhino.Commands.Result InstanceDefinitionObjects(Rhino.RhinoDoc doc)
  {
    Rhino.DocObjects.ObjRef objref;
    var rc = Rhino.Input.RhinoGet.GetOneObject("Select instance", false, Rhino.DocObjects.ObjectType.InstanceReference, out objref);
    if (rc != Rhino.Commands.Result.Success)
      return rc;

    var iref = objref.Object() as Rhino.DocObjects.InstanceObject;
    if (iref != null)
    {
      var idef = iref.InstanceDefinition;
      if (idef != null)
      {
        var rhino_objects = idef.GetObjects();
        for (int i = 0; i < rhino_objects.Length; i++)
          Rhino.RhinoApp.WriteLine("Object {0} = {1}", i, rhino_objects[i].Id);
      }
    }
    return Rhino.Commands.Result.Success;
  }
}
