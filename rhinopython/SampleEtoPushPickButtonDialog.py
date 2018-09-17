################################################################################
# SampleEtoPushPickButtonDialog.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
# Imports
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

class SampleEtoPushPickButtonDialog(forms.Dialog):
    
    def __init__(self):
        self.Title = "Sample Eto PushPickButton Dialog"
        self.ClientSize = drawing.Size(200, 200)
        self.Padding = drawing.Padding(5)
        self.Resizable = False

        button = forms.Button()
        button.Text = "Click Me!"
        button.Click += self.OnPushPickButton
        
        self.Content = button
        
    def OnPickPoint(self, sender, e):
        Rhino.Input.RhinoGet.GetPoint("Pick a point", True)
        
    def OnPushPickButton(self, sender, e):
        Rhino.UI.EtoExtensions.PushPickButton(self, self.OnPickPoint)
        

################################################################################
# Creating a dialog instance and displaying the dialog.
################################################################################
def TestSampleEtoPushPickButtonDialog():
    dialog = SampleEtoPushPickButtonDialog()
    dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestSampleEtoPushPickButtonDialog()    