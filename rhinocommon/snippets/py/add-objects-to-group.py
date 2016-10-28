import Rhino
import scriptcontext

def AddObjectsToGroup():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects to group")
    go.GroupSelect = True
    go.GetMultiple(1, 0)
    if go.CommandResult()!=Rhino.Commands.Result.Success:
        return go.CommandResult()

    ids = [go.Object(i).ObjectId for i in range(go.ObjectCount)]
    index = scriptcontext.doc.Groups.Add(ids)
    scriptcontext.doc.Views.Redraw()
    if index>=0: return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure


if __name__ == "__main__":
    AddObjectsToGroup()
