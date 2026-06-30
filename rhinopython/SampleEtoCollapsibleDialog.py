################################################################################
# SampleEtoCollapsibleDialog.py
# Copyright (c) 2013-2026, Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
# ! python3
import scriptcontext
import rhinoscriptsyntax as rs
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# Sample dialog class extending the Eto Dialog([T])
################################################################################
class CollapsableEtoDialog(forms.Dialog[bool]): # return True or False from ShowModal()

    def __init__(self):
        super().__init__()
        self.Title = "My Collapsible Eto Form"
        self.Resizable = False
        self.Padding = drawing.Padding(5)

       # Custom label helper to set alignment
        def L(text):
            m_label = forms.Label()
            m_label.Text = text
            m_label.VerticalAlignment = forms.VerticalAlignment.Center
            m_label.TextAlignment = forms.TextAlignment.Right
            return m_label

        # Custom checkbox helper (set properties after construction)
        def CB(text):
            m_cb = forms.CheckBox()
            m_cb.Text = text
            return m_cb


        # set content of the collapsed section (shown expanded by default)
        self.collapsePanel = forms.DynamicLayout()
        self.collapsePanel.Visible = True
        self.collapsePanel.Padding = drawing.Padding(40, 10)
        self.collapsePanel.DefaultSpacing = drawing.Size(5, 5)
        self.collapsePanel.BeginVertical()
        self.collapsePanel.AddRow(None, L("Density:"), forms.NumericUpDown())
        self.collapsePanel.AddRow(None, L("Maximum angle:"), forms.NumericUpDown())
        self.collapsePanel.AddRow(None, L("Maximum aspect ratio:"), forms.NumericUpDown())
        self.collapsePanel.AddRow(None, L("Minimum edge length:"), forms.NumericUpDown())
        self.collapsePanel.AddRow(None, L("Maximum edge length:"), forms.NumericUpDown())
        self.collapsePanel.AddRow(None, L("Maximum distance, edge to surface:"), forms.NumericUpDown())
        self.collapsePanel.AddRow(None, L("Minimum initial grid quads:"), forms.NumericUpDown())
        self.collapsePanel.EndVertical()
        self.collapsePanel.BeginVertical()
        self.collapsePanel.AddRow(None, CB("Refine mesh"))
        self.collapsePanel.AddRow(None, CB("Jagged seams"), CB("Pack textures"), None)
        self.collapsePanel.AddRow(None, CB("Simple planes"))
        self.collapsePanel.EndVertical()

        # button to toggle collapsing (starts expanded, so show the collapse arrow)
        self.collapseButton = forms.Button()
        self.collapseButton.Text = "^"
        self.collapseButton.MinimumSize = drawing.Size.Empty
        self.collapseButton.Click += self.collapseButton_Click

        # a few buttons always shown at the bottom
        self.previewButton = forms.Button()
        self.previewButton.Text = "Preview"


        self.cancelButton = forms.Button()
        self.cancelButton.Text = "Cancel"
        self.cancelButton.Click += self.cancelButton_Click;

        self.okButton = forms.Button()
        self.okButton.Text = "OK"
        self.okButton.Click += self.okButton_Click
        
        # set default buttons when user presses enter or escape anywhere on the form
        self.DefaultButton = self.okButton
        self.AbortButton = self.cancelButton

        toleranceUpDown = forms.NumericUpDown()

    # our main layout
        layout = forms.DynamicLayout()
        layout.DefaultSpacing = drawing.Size(2,2)
        layout.AddSeparateRow(None, L("Tolerance"), toleranceUpDown, L("millimeters"), self.collapseButton)
        layout.AddCentered(self.collapsePanel) # we need this auto-sized so we can get its width to adjust form height
        layout.Add(None); # expanding space, in case you want the form re-sizable
        layout.AddSeparateRow(None, self.previewButton, self.cancelButton, self.okButton);

        self.Content = layout;

    def collapseButton_Click(self, sender, e):
        if self.collapsePanel.Visible:
           # Collapse: remember the panel's height while it is still visible
           # (a hidden panel reports 0, which would break re-expanding).
           self.m_panel_height = self.collapsePanel.Height
           # Clamp to >= 0: a negative window height is a fatal error in Rhino 9's WPF/Eto.
           new_height = max(0, self.ClientSize.Height - self.m_panel_height)
           self.ClientSize = drawing.Size(self.ClientSize.Width, new_height)
           self.collapsePanel.Visible = False
           self.collapseButton.Text = "v"
        else:
           # Expand: restore using the remembered panel height.
           self.collapsePanel.Visible = True
           self.collapseButton.Text = "^"
           panel_height = getattr(self, "m_panel_height", self.collapsePanel.Height)
           self.ClientSize = drawing.Size(self.ClientSize.Width, self.ClientSize.Height + panel_height)

    def cancelButton_Click (self, sender, e):
        self.Close(False)


    def okButton_Click (self, sender, e):
        self.Close(True)

################################################################################
# Creating a dialog instance and displaying the dialog.
################################################################################

def TestCollapseEtoDialog():
    dialog = CollapsableEtoDialog()
    dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestCollapseEtoDialog()    
