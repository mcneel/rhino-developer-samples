################################################################################
# SampleEtoColorDropDown.py
# Copyright (c) 2013-2026, Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
# ! python3
import System
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# SampleEtoColorDropDown class
################################################################################
class SampleEtoColorDropDown(forms.DropDown):
    
    def __init__(self, colors):
        super().__init__()
        for c in colors:
            self.Items.Add(self.CreateItem(c.ToArgb(), c.Name))
        
    def CreateItem(self, argb, text):
        item = forms.ImageListItem()
        item.Text = text
        size = drawing.Size(20, 14) * int(forms.Screen.PrimaryScreen.LogicalPixelSize)
        bitmap = drawing.Bitmap(size, drawing.PixelFormat.Format32bppRgb)
        # Graphics is not a context manager under Rhino 9's CPython runtime,
        # so create and dispose it explicitly instead of using 'with'.
        g = drawing.Graphics(bitmap)
        try:
            g.Clear(drawing.Color.FromArgb(argb))
        finally:
            g.Dispose()
        item.Image = bitmap
        return item
    
################################################################################
# SampleEtoColorDropDownDialog dialog
################################################################################
class SampleEtoColorDropDownDialog(forms.Dialog):

    def __init__(self, colors):
        super().__init__()
        self.Title = "Sample Color Dropdown Dialog"
        self.ClientSize = drawing.Size(400, 200)
        self.Padding = drawing.Padding(5)
        self.Resizable = False
        
        self.m_dropdown = SampleEtoColorDropDown(colors)
        self.m_dropdown.SelectedIndex = 0
        self.m_dropdown.SelectedIndexChanged += self.OnSelectedIndexChanged
        
        self.m_label = forms.Label()
        self.m_label.Text = self.m_dropdown.Items[0].Text
        
        # Labels must be Label controls; a bare string is no longer auto-converted.
        name_label = forms.Label()
        name_label.Text = "Name:"
        color_label = forms.Label()
        color_label.Text = "Color:"

        layout = forms.DynamicLayout()
        layout.Padding = drawing.Padding(10)
        layout.Spacing = drawing.Size(5, 5)
        layout.AddRow(name_label, self.m_label, None)
        layout.AddRow(color_label, self.m_dropdown, None)
        layout.Add(None, None, True)
        
        self.Content = layout
        
    def OnSelectedIndexChanged(self, sender, e):
        idx = self.m_dropdown.SelectedIndex
        self.m_label.Text = self.m_dropdown.Items[idx].Text
        msg = "DropDown.SelectedIndexChanged, Value: {0}".format(idx)
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
