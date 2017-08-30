################################################################################
# SampleEtoCollapsibleDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
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


        # set content of the collapsed section
        self.collapsePanel = forms.DynamicLayout(Visible = False, Padding = drawing.Padding(40, 10), DefaultSpacing = drawing.Size(5, 5))
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
        self.collapsePanel.AddRow(None, forms.CheckBox(Text = "Refine mesh"))
        self.collapsePanel.AddRow(None, forms.CheckBox(Text = "Jagged seams"), forms.CheckBox(Text = "Pack textures"), None)
        self.collapsePanel.AddRow(None, forms.CheckBox(Text = "Simple planes"))
        self.collapsePanel.EndVertical()

        # button to toggle collapsing
        self.collapseButton = forms.Button(Text = "v", MinimumSize = drawing.Size.Empty)
        self.collapseButton.Click += self.collapseButton_Click
        
        # a few buttons always shown at the bottom
        self.previewButton = forms.Button(Text = "Preview")


        self.cancelButton = forms.Button(Text = "Cancel")
        self.cancelButton.Click += self.cancelButton_Click;

        self.okButton = forms.Button(Text = "OK")
        self.okButton.Click += self.okButton_Click
        
        # set default buttons when user presses enter or escape anywhere on the form
        self.DefaultButton = self.okButton
        self.AbortButton = self.cancelButton

        toleranceUpDown = forms.NumericUpDown()

    # our main layout
        layout = forms.DynamicLayout(DefaultSpacing = drawing.Size(2,2))
        layout.AddSeparateRow(None, L("Tolerance"), toleranceUpDown, L("millimeters"), self.collapseButton)
        layout.AddCentered(self.collapsePanel) # we need this auto-sized so we can get its width to adjust form height
        layout.Add(None); # expanding space, in case you want the form re-sizable
        layout.AddSeparateRow(None, self.previewButton, self.cancelButton, self.okButton);

        self.Content = layout;

    def collapseButton_Click(self, sender, e):
        if self.collapsePanel.Visible:
           self.ClientSize = drawing.Size(self.ClientSize.Width, self.ClientSize.Height - self.collapsePanel.Height)
           self.collapsePanel.Visible = False
           self.collapseButton.Text = "^"
        else:
           self.collapsePanel.Visible = True
           self.collapseButton.Text = "v"
           self.ClientSize = drawing.Size(max(self.ClientSize.Width, self.collapsePanel.Width), self.ClientSize.Height + self.collapsePanel.Height)
#            except:
#             print "Unexpected error:", sys.exc_info()[0]
#             pass # so we don't bring down rhino if there's a bug in the script
    def cancelButton_Click (self, sender, e):
        self.Close(False)


    def okButton_Click (self, sender, e):
        self.Close(True)


        if self.ShowModal():
            print "Do something, user clicked OK"

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