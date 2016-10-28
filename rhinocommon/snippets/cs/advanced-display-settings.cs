partial class Examples
{
  // The following example code demonstrates how to modify advanced display settings using
  // the Rhino SDK. In this example, a display mode's mesh wireframe thickness (in pixels)
  // will be modified.
  public static Rhino.Commands.Result AdvancedDisplay(Rhino.RhinoDoc doc)
  {
    // Use the display attributes manager to build a list of display modes.
    // Note, these are copies of the originals...
    DisplayModeDescription[] display_modes = DisplayModeDescription.GetDisplayModes();
    if( display_modes==null || display_modes.Length<1 )
      return Rhino.Commands.Result.Failure;

    // Construct an options picker so the user can pick which
    // display mode they want modified
    Rhino.Input.Custom.GetOption go = new Rhino.Input.Custom.GetOption();
    go.SetCommandPrompt("Display mode to modify mesh thickness");
    List<int> opt_list = new List<int>();

    for( int i=0; i<display_modes.Length; i++ )
    {
      string english_name = display_modes[i].EnglishName;
      english_name = english_name.Replace("_", "");
      english_name = english_name.Replace(" ", "");
      english_name = english_name.Replace("-", "");
      english_name = english_name.Replace(",", "");
      english_name = english_name.Replace(".", "");
      int index = go.AddOption(english_name);
      opt_list.Add(index);
    }

    // Get the command option
    go.Get();
    if( go.CommandResult() != Rhino.Commands.Result.Success )
      return go.CommandResult();

    int selected_index = go.Option().Index;
    DisplayModeDescription selected_description = null;
    for( int i=0; i<opt_list.Count; i++ )
    {
      if( opt_list[i]==selected_index )
      {
        selected_description = display_modes[i];
        break;
      }
    }

    // Validate...
    if( selected_description==null )
      return Rhino.Commands.Result.Failure;

    // Modify the desired display mode. In this case, we
    // will just set the mesh wireframe thickness to zero.
    selected_description.DisplayAttributes.MeshSpecificAttributes.MeshWireThickness = 0;
    // Use the display attributes manager to update the display mode.
    DisplayModeDescription.UpdateDisplayMode(selected_description);

    // Force the document to regenerate.
    doc.Views.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
