using System;
using System.Collections.Generic;

partial class Examples
{
  public static Rhino.Commands.Result AddObjectsToGroup(Rhino.RhinoDoc doc)
  {
    Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
    go.SetCommandPrompt("Select objects to group");
    go.GroupSelect = true;
    go.GetMultiple(1, 0);
    if (go.CommandResult() != Rhino.Commands.Result.Success)
      return go.CommandResult();

    List<Guid> ids = new List<Guid>();
    for (int i = 0; i < go.ObjectCount; i++)
    {
      ids.Add(go.Object(i).ObjectId);
    }
    int index = doc.Groups.Add(ids);
    doc.Views.Redraw();
    if (index >= 0)
      return Rhino.Commands.Result.Success;
    return Rhino.Commands.Result.Failure;
  }
}
