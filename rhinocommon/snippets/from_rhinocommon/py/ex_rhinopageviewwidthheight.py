from Rhino import *
from Rhino.Commands import *
from Rhino.Input import *
from scriptcontext import doc

def RunCommand():
  width = 1189
  height = 841
  page_views = doc.Views.GetPageViews()
  page_number = 1 if page_views==None else page_views.Length + 1
  pageview = doc.Views.AddPageView("A0_{0}".format(page_number), width, height)

  new_width = width
  rc, new_width = RhinoGet.GetInteger("new width", False, new_width)
  if rc != Result.Success or new_width <= 0: return rc

  new_height = height
  rc, new_height = RhinoGet.GetInteger("new height", False, new_height)
  if rc != Result.Success or new_height <= 0: return rc

  pageview.PageWidth = new_width
  pageview.PageHeight = new_height
  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()