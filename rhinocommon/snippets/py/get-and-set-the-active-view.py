from Rhino import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from scriptcontext import doc

def RunCommand():
  # view and view names
  active_view_name = doc.Views.ActiveView.ActiveViewport.Name

  non_active_views = [(view.ActiveViewport.Name, view) for view in doc.Views
                      if view.ActiveViewport.Name != active_view_name]

  # get name of view to set active
  gs = GetString()
  gs.SetCommandPrompt("Name of view to set active")
  gs.AcceptNothing(True)
  gs.SetDefaultString(active_view_name)
  for view_name, _ in non_active_views:
    gs.AddOption(view_name)
  result = gs.Get()
  if gs.CommandResult() != Result.Success:
    return gs.CommandResult()

  selected_view_name = "{0}".format(gs.StringResult())
  if gs.Option() != None:
      selected_view_name = gs.Option().EnglishName

  if selected_view_name != active_view_name:
    if selected_view_name in [seq[0] for seq in non_active_views]:
      doc.Views.ActiveView = [seq[1] for seq in non_active_views
                              if seq[0] == selected_view_name][0]
    else:
      print "\"{0}\" is not a view name".format(selected_view_name)

  return Result.Success

if __name__ == "__main__":
  RunCommand()
