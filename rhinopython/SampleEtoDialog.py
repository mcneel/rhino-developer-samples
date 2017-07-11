################################################################################
# SampleEtoDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
from Rhino.UI import *
from Eto.Forms import *
from Eto.Drawing import *

################################################################################
# Sample dialog
################################################################################
def TestSampleEtoDialog():
    dialog = Dialog()
    dialog.Title = "Sample Eto Dialog"
    dialog.ClientSize = Size(200, 200)
    dialog.Padding = Padding(5)
    dialog.Resizable = False

    label = Label()
    label.Text = "Hello Rhino.Python!"
    dialog.Content = label

    dialog.ShowModal(RhinoEtoApp.MainWindow)
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestSampleEtoDialog()    