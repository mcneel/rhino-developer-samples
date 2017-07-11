################################################################################
# SampleEtoViewCaptureDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

# Imports from
from Rhino.UI import *
from Eto.Forms import *
from Eto.Drawing import *

# Imports
import Rhino
import Rhino.UI
import Eto
import scriptcontext
import System

################################################################################
# SampleEtoViewCaptureDialog dialog class
################################################################################
class SampleEtoViewCaptureDialog(Eto.Forms.Dialog[bool]):
    
    # Class initializer
    def __init__(self):
        # Initialize dialog box
        self.Title = 'SampleEtoViewCaptureDialog'
        self.Padding = Padding(10)
        # Create a table layout and add all the controls
        layout = TableLayout()
        layout.Padding = Padding(10)
        layout.Spacing = Size(5, 5)
        layout.Rows.Add(self.CreateLabel())
        layout.Rows.Add(self.CreateImageView())
        layout.Rows.Add(None) # spacer
        layout.Rows.Add(self.CreateButtons())
        # Set the dialog content
        self.Content = layout
    
    # Create the dialog label
    def CreateLabel(self):
        self.m_label = Label(Text = 'Click the "Capture" button...')
        return self.m_label
    
    # Create the dialog image list
    def CreateImageView(self):
        self.m_image_view = ImageView()
        self.m_image_view.Size = Size(300, 200)
        self.m_image_view.Image = None
        return self.m_image_view
    
    # Close button click handler
    def OnCloseButtonClick(self, sender, e):
        rc = self.m_image_view.Image is not None
        if rc:
            self.Close(True)
        else:
            self.Close(False)
        
    # Capture button click handler 
    def OnCaptureButtonClick(self, sender, e): 
        # Capture the active view to a System.Drawing.Bitmap
        view = scriptcontext.doc.Views.ActiveView
        bitmap = view.CaptureToBitmap()
        # Convert the System.Drawing.Bitmap to an Eto.Drawing.Bitmap
        # which is required for an Eto image view control
        stream = System.IO.MemoryStream()
        format = System.Drawing.Imaging.ImageFormat.Png
        System.Drawing.Bitmap.Save(bitmap, stream, format)
        if stream.Length != 0:
          self.m_image_view.Image = Bitmap(stream)
        stream.Dispose()
        # Update the text label
        self.m_label.Text = 'Captured view: {}'.format(view.ActiveViewport.Name)
        # Disable the default button
        self.DefaultButton.Enabled = False

    # Create the dialog buttons
    def CreateButtons(self):
        # Create the default button
        self.DefaultButton = Button(Text = 'Capture')
        self.DefaultButton.Click += self.OnCaptureButtonClick
        # Create the abort button
        self.AbortButton = Button(Text = 'Close')
        self.AbortButton.Click += self.OnCloseButtonClick
        # Create button layout
        button_layout = TableLayout()
        button_layout.Spacing = Size(5, 5)
        if Rhino.Runtime.HostUtils.RunningOnWindows:
            button_layout.Rows.Add(TableRow(None, self.DefaultButton, self.AbortButton))
        else:
            button_layout.Rows.Add(TableRow(None, self.AbortButton, self.DefaultButton))
        return button_layout
        
    # Returns the captured image
    def Image(self):
        return self.m_image_view.Image
    
################################################################################
# Function to test the capture dialog
################################################################################
def TestSampleEtoViewCaptureDialog():
    dialog = SampleEtoViewCaptureDialog();
    rc = dialog.ShowModal(RhinoEtoApp.MainWindow)
    if (rc):
        print 'have image'
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestSampleEtoViewCaptureDialog()