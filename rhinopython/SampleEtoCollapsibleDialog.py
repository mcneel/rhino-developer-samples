################################################################################
# SampleEtoCollapsibleDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import scriptcontext
import rhinoscriptsyntax as rs
import clr
import sys

from Rhino.UI import *
from Eto.Forms import Form, Dialog, Label, TextBox, StackLayout, Orientation, Button, HorizontalAlignment, VerticalAlignment, TextAlignment, NumericUpDown, MessageBox, DropDown, CheckBox, TableLayout, StackLayoutItem, DynamicLayout, Control, Panel, Form
from Eto.Drawing import *

# Custom label helper to set alignment
def L(text):
	return Label(Text = text, VerticalAlignment = VerticalAlignment.Center, TextAlignment = TextAlignment.Right)

form = Dialog[bool]() # return True or False from ShowModal()
form.Title = "My Collapsible Eto Form"
form.Resizable = False
form.Padding = Padding(5)

# set content of the collapsed section
collapsePanel = DynamicLayout(Visible = False, Padding = Padding(40, 10), DefaultSpacing = Size(5, 5))
collapsePanel.BeginVertical()
collapsePanel.AddRow(None, L("Density:"), NumericUpDown())
collapsePanel.AddRow(None, L("Maximum angle:"), NumericUpDown())
collapsePanel.AddRow(None, L("Maximum aspect ratio:"), NumericUpDown())
collapsePanel.AddRow(None, L("Minimum edge length:"), NumericUpDown())
collapsePanel.AddRow(None, L("Maximum edge length:"), NumericUpDown())
collapsePanel.AddRow(None, L("Maximum distance, edge to surface:"), NumericUpDown())
collapsePanel.AddRow(None, L("Minimum initial grid quads:"), NumericUpDown())
collapsePanel.EndVertical()
collapsePanel.BeginVertical()
collapsePanel.AddRow(None, CheckBox(Text = "Refine mesh"))
collapsePanel.AddRow(None, CheckBox(Text = "Jagged seams"), CheckBox(Text = "Pack textures"), None)
collapsePanel.AddRow(None, CheckBox(Text = "Simple planes"))
collapsePanel.EndVertical()

# button to toggle collapsing
collapseButton = Button(Text = "v", MinimumSize = Size.Empty)
def collapseButton_Click(sender, e):
	try:
		if collapsePanel.Visible:
			form.ClientSize = Size(form.ClientSize.Width, form.ClientSize.Height - collapsePanel.Height)
			collapsePanel.Visible = False
			collapseButton.Text = "^"
		else:
			collapsePanel.Visible = True
			collapseButton.Text = "v"
			form.ClientSize = Size(max(form.ClientSize.Width, collapsePanel.Width), form.ClientSize.Height + collapsePanel.Height)
	except:
		print "Unexpected error:", sys.exc_info()[0]
		pass # so we don't bring down rhino if there's a bug in the script

collapseButton.Click += collapseButton_Click


# a few buttons always shown at the bottom
previewButton = Button(Text = "Preview")

cancelButton = Button(Text = "Cancel")
def cancelButton_Click (sender, e):
	try:
		form.Close(False)
	except:
		print "Unexpected error:", sys.exc_info()[0]
		pass # so we don't bring down rhino if there's a bug in the script
cancelButton.Click += cancelButton_Click;

okButton = Button(Text = "OK")
def okButton_Click (sender, e):
	try:
		form.Close(True)
	except:
		print "Unexpected error:", sys.exc_info()[0]
		pass # so we don't bring down rhino if there's a bug in the script
okButton.Click += okButton_Click

# set default buttons when user presses enter or escape anywhere on the form
form.DefaultButton = okButton
form.AbortButton = cancelButton
toleranceUpDown = NumericUpDown()

# our main layout
layout = DynamicLayout(DefaultSpacing = Size(2,2))
layout.AddSeparateRow(None, L("Tolerance"), toleranceUpDown, L("millimeters"), collapseButton)
layout.AddCentered(collapsePanel) # we need this auto-sized so we can get its width to adjust form height
layout.Add(None); # expanding space, in case you want the form re-sizable
layout.AddSeparateRow(None, previewButton, cancelButton, okButton);
form.Content = layout;
	
	
if form.ShowModal():
	print "Do something, user clicked OK"
	