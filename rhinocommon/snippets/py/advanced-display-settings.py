import Rhino
import scriptcontext

# The following example demonstrates how to modify advanced display settings
# using RhinoCommon. In this example, a display mode's mesh wireframe thickness
# (in pixels) will be modified.
def AdvancedDisplay():
    # Use the display attributes manager to build a list of display modes.
    # Note, these are copies of the originals...
    display_modes = Rhino.Display.DisplayModeDescription.GetDisplayModes()
    if not display_modes: return Rhino.Commands.Result.Failure

    # Construct an options picker so the user can pick which
    # display mode they want modified
    go = Rhino.Input.Custom.GetOption()
    go.SetCommandPrompt("Display mode to modify mesh thickness")
    opt_list = []
    for i, mode in enumerate(display_modes):
        english_name = mode.EnglishName
        english_name = english_name.translate(None, "_ -,.")
        opt_list.append( go.AddOption(english_name) )

    # Get the command option
    go.Get()
    if go.CommandResult()!=Rhino.Commands.Result.Success:
      return go.CommandResult();

    selected_index = go.Option().Index
    selected_description = None
    for i,option in enumerate(opt_list):
        if option==selected_index:
            selected_description = display_modes[i]
            break
    # Validate...
    if not selected_description: return Rhino.Commands.Result.Failure

    # Modify the desired display mode. In this case, we
    # will just set the mesh wireframe thickness to zero.
    selected_description.DisplayAttributes.MeshSpecificAttributes.MeshWireThickness = 0
    # Use the display attributes manager to update the display mode.
    Rhino.Display.DisplayModeDescription.UpdateDisplayMode(selected_description)
    # Force the document to regenerate.
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    AdvancedDisplay()
