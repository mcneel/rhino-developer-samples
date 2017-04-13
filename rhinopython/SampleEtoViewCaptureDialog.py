################################################################################
# SampleEtoViewCaptureDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import clr
clr.AddReference("Eto")
clr.AddReference("Rhino.UI")

from Rhino.UI import *
from Eto.Forms import *
from Eto.Drawing import *

import Rhino
import scriptcontext
import System

# Display a "view capture" in an Eto dialog box
def SampleEtoViewCaptureDialog():
    
    # Capture button click handler 
    def OnCaptureButtonClick(sender, e): 
        # Capture the active view to a System.Drawing.Bitmap
        view = scriptcontext.doc.Views.ActiveView
        bitmap = view.CaptureToBitmap()
        
        # Convert the System.Drawing.Bitmap to an Eto.Drawing.Bitmap
        # which is required for an image view control
        stream = System.IO.MemoryStream()
        format = System.Drawing.Imaging.ImageFormat.Png
        System.Drawing.Bitmap.Save(bitmap, stream, format)
        if stream.Length != 0:
          image_view.Image = Bitmap(stream)
        stream.Dispose()
        
        # Update the text label
        label.Text = "Captured view: {}".format(view.ActiveViewport.Name)

        # Disable the default button
        dialog.DefaultButton.Enabled = False
    
    # Close button click handler
    def OnCloseButtonClick(sender, e): 
        dialog.Close(True)
    
    # Create an Eto dialog
    dialog = Dialog[bool]()
    dialog.Title = "SampleEtoViewCaptureDialog"
    dialog.Padding = Padding(10)
    
    # Create a text label
    label = Label(Text = 'Click the "Capture" button...')
    
    # Create an image view
    image_view = ImageView()
    image_view.Size = Size(300, 200)
    image_view.Image = None
    
    # Create the default button
    dialog.DefaultButton = Button(Text = "Capture")
    dialog.DefaultButton.Click += OnCaptureButtonClick
    
    # Create the abort button
    dialog.AbortButton = Button(Text = "Close")
    dialog.AbortButton.Click += OnCloseButtonClick
    
    # Create a table layout for the buttons
    button_layout = TableLayout()
    button_layout.Spacing = Size(5, 5)
    if Rhino.Runtime.HostUtils.RunningOnWindows:
        button_layout.Rows.Add(TableRow(None, dialog.DefaultButton, dialog.AbortButton))
    else:
        button_layout.Rows.Add(TableRow(None, dialog.AbortButton, dialog.DefaultButton))
    
    # Create a table layout and add all the controls
    content_layout = TableLayout()
    content_layout.Padding = Padding(10)
    content_layout.Spacing = Size(5, 5)
    content_layout.Rows.Add(label)
    content_layout.Rows.Add(image_view)
    content_layout.Rows.Add(button_layout)
    
    # Set the dialog content
    dialog.Content = content_layout
    
    # Display the dialog box in a modal fashion
    dialog.ShowModal(RhinoEtoApp.MainWindow)
    
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleEtoViewCaptureDialog()