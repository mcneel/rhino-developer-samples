################################################################################
# SampleEtoRebuildCurve.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
# Imports
import Rhino
import scriptcontext as sc
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# Class to hold rebuild arguments
################################################################################
class RebuildCurveArgs():
    
    # Initializer
    def __init__(self):
        self.PointCount = 0
        self.Degree = 0
        self.PreserveTangents = False
        self.DeleteInput = True
    
    # Validator
    def IsValid(self):
        if self.PointCount < 2 or self.PointCount > 32767: return False
        if self.Degree < 1 or self.Degree > 11: return False
        return True
    
################################################################################
# Rebuild curve dialog class
################################################################################
class RebuildCurveDialog(forms.Dialog[bool]):
    
    # Initializer
    def __init__(self, args):
        self.Args = args
        # Initialize dialog box
        self.Title = 'Rebuild'
        self.Padding = drawing.Padding(5)
        # Create layout
        layout = forms.TableLayout()
        layout.Padding = drawing.Padding(5)
        layout.Spacing = drawing.Size(5, 5)
        layout.Rows.Add(self.CreateSteppers())
        layout.Rows.Add(None) # spacer
        layout.Rows.Add(self.CreateCheckBoxes())
        layout.Rows.Add(None) # spacer
        layout.Rows.Add(self.CreateButtons())
        # Set the dialog content
        self.Content = layout
    
    # Creates numeric stepper controls
    def CreateSteppers(self):
        # Create labels
        label0 = forms.Label(Text = 'Point count:')
        label1 = forms.Label(Text = 'Degree:')
        label2 = forms.Label(Text = '({})'.format(self.Args.PointCount))
        label3 = forms.Label(Text = '({})'.format(self.Args.Degree))
        # Create numeric steppers
        self.PointCount = forms.NumericStepper(
            Value = self.Args.PointCount,
            MinValue = 2,
            MaxValue = 32767
            )
        self.Degree = forms.NumericStepper(
            Value = self.Args.Degree,
            MinValue = 1,
            MaxValue = 11
            )
        # Create table layout
        layout = forms.TableLayout()
        layout.Spacing = drawing.Size(5, 5)
        layout.Rows.Add(forms.TableRow(label0, label2, self.PointCount))
        layout.Rows.Add(forms.TableRow(label1, label3, self.Degree))
        return layout
    
    # Creates checkbox controls
    def CreateCheckBoxes(self):
        # Create checkboxes
        self.DeleteInput = forms.CheckBox(
            Text = 'Delete input', 
            Checked = self.Args.DeleteInput,
            ThreeState = False
            )
        self.PreserveTangents = forms.CheckBox(
            Text = 'Preserve tangent end directions', 
            Checked = self.Args.PreserveTangents,
            ThreeState = False
            )
        # Create table layout
        layout = forms.TableLayout()
        layout.Spacing = drawing.Size(5, 5)
        layout.Rows.Add(forms.TableRow(self.DeleteInput))
        layout.Rows.Add(forms.TableRow(self.PreserveTangents))
        return layout
    
    # OK button click handler
    def OnOkButtonClick(self, sender, e):
        # Harvest control values before closing
        self.Args.PointCount = self.PointCount.Value
        self.Args.Degree = self.Degree.Value
        self.Args.PreserveTangents = self.PreserveTangents.Checked
        self.Args.DeleteInput = self.DeleteInput.Checked
        self.Close(True)
    
    # Cancel button click handler
    def OnCancelButtonClick(self, sender, e):
        self.Close(False)
    
    # Create button controls
    def CreateButtons(self):
        # Create the default button
        self.DefaultButton = forms.Button(Text = 'OK')
        self.DefaultButton.Click += self.OnOkButtonClick
        # Create the abort button
        self.AbortButton = forms.Button(Text = 'Cancel')
        self.AbortButton.Click += self.OnCancelButtonClick
        # Create button layout
        button_layout = forms.TableLayout()
        button_layout.Spacing = drawing.Size(5, 5)
        button_layout.Rows.Add(forms.TableRow(None, self.DefaultButton, self.AbortButton, None))
        return button_layout
    
################################################################################
# Function to test the rebuild curve dialog
################################################################################
def TestRebuildCurve():
    
    res, objref = Rhino.Input.RhinoGet.GetOneObject(
        "Select curve to rebuild", 
        True, 
        Rhino.DocObjects.ObjectType.Curve
        )
    if res != Rhino.Commands.Result.Success: return
    
    curve = objref.Curve()
    if curve is None: return
        
    nurb = curve.ToNurbsCurve()
    if curve is None: return    
    
    args = RebuildCurveArgs()
    args.PointCount = nurb.Points.Count
    args.Degree = nurb.Degree
    
    dlg = RebuildCurveDialog(args)
    rc = dlg.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    if rc and args.IsValid():
        nurb = curve.Rebuild(args.PointCount, args.Degree, args.PreserveTangents)
        if nurb:
            if args.DeleteInput:
                sc.doc.Objects.Replace(objref, nurb)
            else:
                sc.doc.Objects.Add(nurb, objref.Object().Attributes)
            sc.doc.Views.Redraw()
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestRebuildCurve()