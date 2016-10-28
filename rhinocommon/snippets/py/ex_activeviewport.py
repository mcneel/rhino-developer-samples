import Rhino
import scriptcontext

def ActiveViewport():
    view = scriptcontext.doc.Views.ActiveView
    if view is None: return
    if isinstance(view, Rhino.Display.RhinoPageView):
        if view.PageIsActive:
            print "The layout", view.PageName, "is active"
        else:
            detail_name = view.ActiveViewport.Name
            print "The detail", detail_name, "on layout", view.PageName, "is active"
    else:
        print "The viewport", view.MainViewport.Name, "is active"


if __name__ == "__main__":
    ActiveViewport()
