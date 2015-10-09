import Rhino
import scriptcontext

# Generate a layout with a single detail view that zooms
# to a list of objects
def AddLayout():
    scriptcontext.doc.PageUnitSystem = Rhino.UnitSystem.Millimeters
    page_views = scriptcontext.doc.Views.GetPageViews()
    page_number = 1
    if page_views: page_number = len(page_views) + 1
    pageview = scriptcontext.doc.Views.AddPageView("A0_{0}".format(page_number), 1189, 841)
    if pageview:
        top_left = Rhino.Geometry.Point2d(20,821)
        bottom_right = Rhino.Geometry.Point2d(1169, 20)
        detail = pageview.AddDetailView("ModelView", top_left, bottom_right, Rhino.Display.DefinedViewportProjection.Top)
        if detail:
            pageview.SetActiveDetail(detail.Id)
            detail.Viewport.ZoomExtents()
            detail.DetailGeometry.IsProjectionLocked = True
            detail.DetailGeometry.SetScale(1, scriptcontext.doc.ModelUnitSystem, 10, scriptcontext.doc.PageUnitSystem)
            # Commit changes tells the document to replace the document's detail object
            # with the modified one that we just adjusted
            detail.CommitChanges()
        pageview.SetPageAsActive()
        scriptcontext.doc.Views.ActiveView = pageview
        scriptcontext.doc.Views.Redraw()

if __name__=="__main__":
    AddLayout()