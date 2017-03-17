################################################################################
# SampleSetDetailsToWireframe.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Demonstrates how to set all detail views to wireframe display
def SampleSetDetailsToWireframe():
    # Find the wireframe display mode object
    display_mode = Rhino.Display.DisplayModeDescription.FindByName("Wireframe")
    if display_mode:
        # Get all of the document's page views
        page_views = sc.doc.Views.GetPageViews()
        if page_views:
            # Process each page view
            for page_view in page_views:
                # Get all of the page view's details
                details = page_view.GetDetailViews()
                if details:
                    # Process each page view detail
                    for detail in details:
                        # If the detail's display mode is not wireframe...
                        if detail.Viewport.DisplayMode.Id != display_mode.Id:
                            # ...set it to wireframe.
                            detail.Viewport.DisplayMode = display_mode
                            detail.CommitViewportChanges()
                    # Redraw the page view
                    page_view.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleSetDetailsToWireframe()