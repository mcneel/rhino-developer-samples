partial class Examples
{
  public static Rhino.Commands.Result AddChildLayer(Rhino.RhinoDoc doc)
  {
    // Get an existing layer
    string default_name = doc.Layers.CurrentLayer.Name;

    // Prompt the user to enter a layer name
    Rhino.Input.Custom.GetString gs = new Rhino.Input.Custom.GetString();
    gs.SetCommandPrompt("Name of existing layer");
    gs.SetDefaultString(default_name);
    gs.AcceptNothing(true);
    gs.Get();
    if (gs.CommandResult() != Rhino.Commands.Result.Success)
      return gs.CommandResult();

    // Was a layer named entered?
    string layer_name = gs.StringResult().Trim();
    int index = doc.Layers.Find(layer_name, true);
    if (index<0)
      return Rhino.Commands.Result.Cancel;

    Rhino.DocObjects.Layer parent_layer = doc.Layers[index];

    // Create a child layer
    string child_name = parent_layer.Name + "_child";
    Rhino.DocObjects.Layer childlayer = new Rhino.DocObjects.Layer();
    childlayer.ParentLayerId = parent_layer.Id;
    childlayer.Name = child_name;
    childlayer.Color = System.Drawing.Color.Red;

    index = doc.Layers.Add(childlayer);
    if (index < 0)
    {
      Rhino.RhinoApp.WriteLine("Unable to add {0} layer.", child_name);
      return Rhino.Commands.Result.Failure;
    }
    return Rhino.Commands.Result.Success;
  }
}
