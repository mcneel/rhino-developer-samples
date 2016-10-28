import Rhino
import scriptcontext
import System.Guid, System.Drawing.Color

def AddLayer():
    # Cook up an unused layer name
    unused_name = scriptcontext.doc.Layers.GetUnusedLayerName(False)

    # Prompt the user to enter a layer name
    gs = Rhino.Input.Custom.GetString()
    gs.SetCommandPrompt("Name of layer to add")
    gs.SetDefaultString(unused_name)
    gs.AcceptNothing(True)
    gs.Get()
    if gs.CommandResult()!=Rhino.Commands.Result.Success:
        return gs.CommandResult()

    # Was a layer named entered?
    layer_name = gs.StringResult().Trim()
    if not layer_name:
        print "Layer name cannot be blank."
        return Rhino.Commands.Result.Cancel

    # Is the layer name valid?
    if not Rhino.DocObjects.Layer.IsValidName(layer_name):
        print layer_name, "is not a valid layer name."
        return Rhino.Commands.Result.Cancel

    # Does a layer with the same name already exist?
    layer_index = scriptcontext.doc.Layers.Find(layer_name, True)
    if layer_index>=0:
        print "A layer with the name", layer_name, "already exists."
        return Rhino.Commands.Result.Cancel

    # Add a new layer to the document
    layer_index = scriptcontext.doc.Layers.Add(layer_name, System.Drawing.Color.Black)
    if layer_index<0:
        print "Unable to add", layer_name, "layer."
        return Rhino.Commands.Result.Failure

    return Rhino.Commands.Result.Success


if __name__=="__main__":
    AddLayer()
