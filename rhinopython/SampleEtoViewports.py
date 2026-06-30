################################################################################
# SampleEtoViewports.py
# Copyright (c) 2013-2026, Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
# ! python3
import Rhino
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# Viewports dialog class
################################################################################
class ViewportsDialog(forms.Dialog[bool]):
    
    # Initializer
    def __init__(self):
        super().__init__()
        # Initialize dialog box
        self.Title = 'Rhino Viewport in an Eto Control'
        self.Padding = drawing.Padding(5)
        self.Resizable = True
        # Give the dialog an explicit size so the two 400x200 viewports are not
        # squished: the ViewportControl's own Size does not drive the window width.
        self.ClientSize = drawing.Size(420, 470)
        # Create viewport controls
        viewport0 = Rhino.UI.Controls.ViewportControl()
        viewport0.Size = drawing.Size(400, 200)
        viewport1 = Rhino.UI.Controls.ViewportControl()
        viewport1.Size = drawing.Size(400, 200)
        # Create layout
        layout = forms.DynamicLayout()
        layout.Padding = drawing.Padding(5)
        layout.Spacing = drawing.Size(5, 5)
        layout.AddRow(viewport0)
        layout.AddRow(viewport1)
        layout.AddRow(None)
        layout.AddRow(self.CloseButton())
        # Set the dialog content
        self.Content = layout
   
    # Close button click handler
    def OnCloseButtonClick(self, sender, e):
        self.Close(True)
   
    # Create button control
    def CloseButton(self):
        # Create the default button
        self.DefaultButton = forms.Button()
        self.DefaultButton.Text = 'Close'
        self.DefaultButton.Click += self.OnCloseButtonClick
        return self.DefaultButton
    
################################################################################
# Function to test the viewport dialog
################################################################################
def TestViewportsDialog():
    dlg = ViewportsDialog()
    dlg.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
     
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestViewportsDialog()
