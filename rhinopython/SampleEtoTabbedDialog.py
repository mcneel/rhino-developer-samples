################################################################################
# SampleEtoTabbedDialog.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

# Imports
import System
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# Creates a panel that displays a text label
################################################################################
class LabelPanel(forms.Panel):
    # Initializer
    def __init__(self):
        # create a control
        label = forms.Label()
        label.Text = "Text Label"
        # create a layout
        layout = forms.DynamicLayout()
        layout.DefaultSpacing = drawing.Size(5, 5)
        layout.Padding = drawing.Padding(10)
        # add the control to the layout
        layout.Add(label)
        # set the panel content
        self.Content = layout

################################################################################
# Creates a panel that displays a text area control
################################################################################
class TextAreaPanel(forms.Scrollable):
    # Initializer
    def __init__(self):
        # create a control
        text = forms.TextArea()
        text.Text = "Every Good Boy Deserves Fudge."
        # create a layout
        layout = forms.TableLayout()
        layout.Padding = drawing.Padding(10)
        layout.Spacing = drawing.Size(5, 5)
        layout.Rows.Add(text)
        # set the panel content
        self.Content = layout

################################################################################
# SampleEtoTabbedDialog dialog class
################################################################################
class SampleEtoTabbedDialog(forms.Dialog):
    
    # Initializer
    def __init__(self):
        self.Rnd = System.Random()
        self.Title = "Sample Eto Tabbed Dialog"
        self.Padding = drawing.Padding(10)
        self.Resizable = True
        self.Content = self.Create()
    
    # Create the dialog content
    def Create(self):
        # create default tabs
        self.TabControl = self.DefaultTabs()
        # create stack layout item for tabs
        tab_items = forms.StackLayoutItem(self.TabControl, True)
        # create layout for buttons
        button_layout = forms.StackLayout()
        button_layout.Orientation = forms.Orientation.Horizontal
        button_layout.Items.Add(None)
        button_layout.Items.Add(self.AddTab())
        button_layout.Items.Add(self.RemoveTab())
        button_layout.Items.Add(self.SelectTab())
        button_layout.Items.Add(None)
        # create stack layout for content
        layout = forms.StackLayout()
        layout.Spacing = 5
        layout.HorizontalContentAlignment = forms.HorizontalAlignment.Stretch
        # add the stuff above to this layout
        layout.Items.Add(button_layout)
        layout.Items.Add(tab_items)
        return layout
    
    # Create the default tabs
    def DefaultTabs(self):
        # creates a tab control
        control = self.CreateTabControl()
        # create and add a tab page 1
        tab1 = forms.TabPage()
        tab1.Text = "Tab 1"
        tab1.Content = self.TabOne()
        control.Pages.Add(tab1)
        # create and add a tab page 2
        tab2 = forms.TabPage()
        tab2.Text = "Tab 2"
        tab2.Content = self.TabTwo()
        control.Pages.Add(tab2)
        # return
        return control
    
    # AddTab button click handler
    def AddTabClick(self, sender, e):
        tab = forms.TabPage()
        tab.Text = "Tab" + str(self.TabControl.Pages.Count + 1)
        if (self.TabControl.Pages.Count % 2 == 0):
            tab.Content = self.TabOne()
        else:
            tab.Content = self.TabTwo()
        self.TabControl.Pages.Add(tab)
    
    # RemoveTab button click handler
    def RemoveTabClick(self, sender, e):
        if (self.TabControl.SelectedIndex >= 0 and self.TabControl.Pages.Count > 0):
            self.TabControl.Pages.RemoveAt(self.TabControl.SelectedIndex)
    
    # SelectTab button click handler
    def SelectTabClick(self, sender, e):
        if (self.TabControl.Pages.Count > 0):
            self.TabControl.SelectedIndex = self.Rnd.Next(self.TabControl.Pages.Count)
    
    # Creates an add tab button
    def AddTab(self):
        button = forms.Button()
        button.Text = "Add Tab"
        button.Click += self.AddTabClick
        return button
    
    # Creates a remove tab button
    def RemoveTab(self):
        button = forms.Button()
        button.Text = "Remove Tab"
        button.Click += self.RemoveTabClick
        return button
    
    # Creates a tab selection button
    def SelectTab(self):
        button = forms.Button()
        button.Text = "Select Tab"
        button.Click += self.SelectTabClick
        return button
    
    # Delegate function to tab position control
    def DockPositionDelegate(self, position):
        self.TabControl = position
    
    # Creates the one and only tab control
    def CreateTabControl(self):
        tab = forms.TabControl()
        # Orient the tabs at the top
        tab.TabPosition = forms.DockPosition.Top
        return tab
    
    # Creates a tab page's content
    def TabOne(self):
        control = forms.Panel()
        control.Content = LabelPanel()
        return control
    
    # Creates a tab page's content
    def TabTwo(self):
        control = forms.Panel()
        control.Content = TextAreaPanel()
        return control

################################################################################
# TestSampleEtoTabbedDialog function
################################################################################
def TestSampleEtoTabbedDialog():
    dialog = SampleEtoTabbedDialog()
    dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)

################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    TestSampleEtoTabbedDialog()    