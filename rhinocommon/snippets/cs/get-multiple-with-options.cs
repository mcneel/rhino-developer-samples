partial class Examples
{
  public static Rhino.Commands.Result GetMultipleWithOptions(Rhino.RhinoDoc doc)
  {
    const Rhino.DocObjects.ObjectType geometryFilter = Rhino.DocObjects.ObjectType.Surface | 
                                                       Rhino.DocObjects.ObjectType.PolysrfFilter | 
                                                       Rhino.DocObjects.ObjectType.Mesh;
    int integer1 = 300;
    int integer2 = 300;

    Rhino.Input.Custom.OptionInteger optionInteger1 = new Rhino.Input.Custom.OptionInteger(integer1, 200, 900);
    Rhino.Input.Custom.OptionInteger optionInteger2 = new Rhino.Input.Custom.OptionInteger(integer2, 200, 900);

    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select surfaces, polysurfaces, or meshes");
    go.GeometryFilter = geometryFilter;
    go.AddOptionInteger("Option1", ref optionInteger1);
    go.AddOptionInteger("Option2", ref optionInteger2);
    go.GroupSelect = true;
    go.SubObjectSelect = false;
    go.EnableClearObjectsOnEntry(false);
    go.EnableUnselectObjectsOnExit(false);
    go.DeselectAllBeforePostSelect = false;

    bool bHavePreselectedObjects = false;

    for (; ; )
    {
      Rhino.Input.GetResult res = go.GetMultiple(1, 0);

      if (res == Rhino.Input.GetResult.Option)
      {
        go.EnablePreSelect(false, true);
        continue;
      }

      else if (res != Rhino.Input.GetResult.Object)
        return Rhino.Commands.Result.Cancel;

      if (go.ObjectsWerePreselected)
      {
        bHavePreselectedObjects = true;
        go.EnablePreSelect(false, true);
        continue;
      }

      break;
    }

    if (bHavePreselectedObjects)
    {
      // Normally when command finishes, pre-selected objects will remain
      // selected, when and post-selected objects will be unselected.
      // With this sample, it is possible to have a combination of 
      // pre-selected and post-selected objects. To make sure everything
      // "looks the same", unselect everything before finishing the command.
      for (int i = 0; i < go.ObjectCount; i++)
      {
        Rhino.DocObjects.RhinoObject rhinoObject = go.Object(i).Object();
        if (null != rhinoObject)
          rhinoObject.Select(false);
      }
      doc.Views.Redraw();
    }

    int objectCount = go.ObjectCount;
    integer1 = optionInteger1.CurrentValue;
    integer2 = optionInteger2.CurrentValue;

    Rhino.RhinoApp.WriteLine("Select object count = {0}", objectCount);
    Rhino.RhinoApp.WriteLine("Value of integer1 = {0}", integer1);
    Rhino.RhinoApp.WriteLine("Value of integer2 = {0}", integer2);

    return Rhino.Commands.Result.Success;
  }
}
