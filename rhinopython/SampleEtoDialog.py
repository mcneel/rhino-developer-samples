################################################################################
# SampleEtoDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import clr
clr.AddReference("Eto")
clr.AddReference("Rhino.UI")

from Rhino.UI import *
from Eto.Forms import *
from Eto.Drawing import *

obj = Dialog()
obj.Title = "Sample Eto Dialog"
obj.ClientSize = Size(200, 200)
obj.Padding = Padding(5)
obj.Resizable = False

label = Label()
label.Text = "Hello Rhino.Python!"
obj.Content = label

obj.ShowModal(RhinoEtoApp.MainWindow)