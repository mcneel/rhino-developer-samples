partial class Examples
{
  public static Result PrePostPick(RhinoDoc doc)
  {
    var go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select objects");
    go.EnablePreSelect(true, true);
    go.EnablePostSelect(true);
    go.GetMultiple(0, 0);
    if (go.CommandResult() != Result.Success)
      return go.CommandResult();

    var selected_objects = go.Objects().ToList();

    if (go.ObjectsWerePreselected)
    {
      go.EnablePreSelect(false, true);
      go.DeselectAllBeforePostSelect = false;
      go.EnableUnselectObjectsOnExit(false);
      go.GetMultiple(0, 0);
      if (go.CommandResult() == Result.Success)
      {
        foreach (var obj in go.Objects())
        {
          selected_objects.Add(obj);
          // The normal behavior of commands is that when they finish,
          // objects that were pre-selected remain selected and objects
          // that were post-selected will not be selected. Because we
          // potentially could have both, we'll try to do something
          // consistent and make sure post-selected objects also stay selected
          obj.Object().Select(true);
        }
      }
    }
    return selected_objects.Count > 0 ? Result.Success : Result.Nothing;
  }
}
