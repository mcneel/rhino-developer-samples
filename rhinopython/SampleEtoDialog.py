################################################################################
# SampleEtoDialog.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# Sample dialog class extending the Eto Dialog()
# A guide for Rhino.Python Eto Dialog can be found at:
# http://developer.rhino3d.com/guides/rhinopython/eto-controls-python/
################################################################################
class SimpleEtoDialog(forms.Dialog):

    def __init__(self):
        self.Title = "Sample Eto Dialog"
        self.ClientSize = drawing.Size(200, 200)
        self.Padding = drawing.Padding(5)
        self.Resizable = False

        label = forms.Label()
        label.Text = "Hello Rhino.Python!"
        self.Content = label


################################################################################
# Creating a dialog instance and displaying the dialog.
################################################################################

def TestSampleEtoDialog():
    dialog = SimpleEtoDialog()
    dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestSampleEtoDialog()    