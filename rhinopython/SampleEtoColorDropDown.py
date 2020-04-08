################################################################################
# SampleEtoColorDropDown.py
# Copyright (c) 2020 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import System
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# SampleEtoColorDropDown class
################################################################################
class SampleEtoColorDropDown(forms.DropDown):
    
    def __init__(self, colors):
        for c in colors:
            self.Items.Add(self.CreateItem(c.ToArgb(), c.Name))
        
    def CreateItem(self, argb, text):
        item = forms.ImageListItem()
        item.Text = text
        size = drawing.Size(20, 14) * int(forms.Screen.PrimaryScreen.LogicalPixelSize)
        bitmap = drawing.Bitmap(size, drawing.PixelFormat.Format32bppRgb)
        with drawing.Graphics(bitmap) as g:
            g.Clear(drawing.Color.FromArgb(argb))
        item.Image = bitmap
        return item
    
################################################################################
# SampleEtoColorDropDownDialog dialog
################################################################################
class SampleEtoColorDropDownDialog(forms.Dialog):

    def __init__(self, colors):
        self.Title = "Sample Color Dropdown Dialog"
        self.ClientSize = drawing.Size(400, 200)
        self.Padding = drawing.Padding(5)
        self.Resizable = False
        
        self.m_dropdown = SampleEtoColorDropDown(colors)
        self.m_dropdown.SelectedIndex = 0
        self.m_dropdown.SelectedIndexChanged += self.OnSelectedIndexChanged
        
        layout = forms.DynamicLayout()
        layout.Padding = drawing.Padding(10)
        layout.Spacing = drawing.Size(5, 5)
        layout.AddRow("Color:", self.m_dropdown, None)
        layout.Add(None, None, True)
        
        self.Content = layout
        
    def OnSelectedIndexChanged(self, sender, e):
        msg = "DropDown.SelectedIndexChanged, Value: {0}".format(self.m_dropdown.SelectedIndex)
        print(msg)
        
################################################################################
# Function to test the dialog
################################################################################
def TestSampleEtoColorDropDownDialog():
    
    colors = []
    colors.append(System.Drawing.Color.Black)
    colors.append(System.Drawing.Color.White)
    colors.append(System.Drawing.Color.Red)
    colors.append(System.Drawing.Color.Yellow)
    colors.append(System.Drawing.Color.Green)
    colors.append(System.Drawing.Color.Cyan)
    colors.append(System.Drawing.Color.Blue)
    colors.append(System.Drawing.Color.Magenta)
    
    dialog = SampleEtoColorDropDownDialog(colors);
    rc = dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    
################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestSampleEtoColorDropDownDialog()