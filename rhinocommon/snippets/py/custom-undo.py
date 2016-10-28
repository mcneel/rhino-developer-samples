import Rhino
import scriptcontext


def OnUndoFavoriteNumber(sender, e):
    """!!!!!!!!!!
    NEVER change any setting in the Rhino document or application.  Rhino
    handles ALL changes to the application and document and you will break
    the Undo/Redo commands if you make any changes to the application or
    document. This is meant only for your own private plugin data
    !!!!!!!!!!

    This function can be called either by undo or redo
    In order to get redo to work, add another custom undo event with the
    current value.  If you don't want redo to work, just skip adding
    a custom undo event here
    """
    current_value = scriptcontext.sticky["FavoriteNumber"]
    e.Document.AddCustomUndoEvent("Favorite Number", OnUndoFavoriteNumber, current_value)

    old_value = e.Tag
    print "Going back to your favorite =", old_value
    scriptcontext.sticky["FavoriteNumber"]= old_value;


def TestCustomUndo():
    """Rhino automatically sets up an undo record when a command is run,
       but... the undo record is not saved if nothing changes in the
       document (objects added/deleted, layers changed,...)

       If we have a command that doesn't change things in the document,
       but we want to have our own custom undo called then we need to do
       a little extra work
    """
    current_value = 0
    if scriptcontext.sticky.has_key("FavoriteNumber"):
        current_value = scriptcontext.sticky["FavoriteNumber"]
    rc, new_value = Rhino.Input.RhinoGet.GetNumber("Favorite number", True, current_value)
    if rc!=Rhino.Commands.Result.Success: return

    scriptcontext.doc.AddCustomUndoEvent("Favorite Number", OnUndoFavoriteNumber, current_value);
    scriptcontext.sticky["FavoriteNumber"] = new_value

if __name__=="__main__":
    TestCustomUndo()
