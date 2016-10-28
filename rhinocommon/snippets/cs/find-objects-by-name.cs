partial class Examples
{
  public static Rhino.Commands.Result FindObjectsByName(Rhino.RhinoDoc doc)
  {
    const string name = "abc";
    Rhino.DocObjects.ObjectEnumeratorSettings settings = new Rhino.DocObjects.ObjectEnumeratorSettings();
    settings.NameFilter = name;
    System.Collections.Generic.List<Guid> ids = new System.Collections.Generic.List<Guid>();
    foreach (Rhino.DocObjects.RhinoObject rhObj in doc.Objects.GetObjectList(settings))
      ids.Add(rhObj.Id);

    if (ids.Count == 0)
    {
      Rhino.RhinoApp.WriteLine("No objects with the name " + name);
      return Rhino.Commands.Result.Failure;
    }

    Rhino.RhinoApp.WriteLine("Found {0} objects", ids.Count);
    foreach (Guid id in ids)
      Rhino.RhinoApp.WriteLine("  {0}", id);

    return Rhino.Commands.Result.Success;
  }
}
