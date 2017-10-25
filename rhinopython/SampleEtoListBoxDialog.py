################################################################################
# SampleEtoListBoxDialog.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

# Imports
import System
from System.Collections.ObjectModel import ObservableCollection
import Rhino
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms

################################################################################
# Fruit class
################################################################################
class Fruit(object):
   
    def __init__(self, name, color):
        self.m_name = name
        self.m_color = color
        
    @property
    def Name(self):
        return self.m_name
  
    @property
    def Color(self):
        return self.m_color

################################################################################
# SampleEtoListBoxDialog class
################################################################################
class SampleEtoListBoxDialog(forms.Dialog[bool]):
    
    # Initializer
    def __init__(self, collection):
        self.m_collection = collection
        # Initialize dialog box
        self.Title = 'Sample Eto ListBox'
        self.Padding = drawing.Padding(5)
        self.Resizable = True
        # Create a label control
        label = forms.Label()
        label.Text = 'Select a fruit:'
        # Create dynamic layout control
        layout = forms.DynamicLayout()
        layout.Padding = drawing.Padding(5)
        layout.Spacing = drawing.Size(5, 5)
        # Add controls to layout
        layout.AddRow(label)
        layout.AddRow(self.CreateListBox())
        layout.AddRow(None) # spacer
        layout.AddRow(self.CreateButtons())
        # Set the dialog content
        self.Content = layout
    
    # Delegate function to retrieve the name of a Fruit object
    def FruitDelegate(self, fruit):
        return fruit.Name
        
   # Creates a ListBox control
    def CreateListBox(self):
        # Create labels
        listbox = forms.ListBox()
        listbox.Size = drawing.Size(100, 150)
        listbox.ItemTextBinding = forms.Binding.Delegate[Fruit, System.String](self.FruitDelegate)
        listbox.DataStore = self.m_collection
        listbox.SelectedIndex = 0
        self.m_listbox = listbox
        return self.m_listbox
        
    # OK button click handler
    def OnOkButtonClick(self, sender, e):
        self.m_selected_index = self.m_listbox.SelectedIndex
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
        button_layout = forms.DynamicLayout()
        button_layout.Spacing = drawing.Size(5, 5)
        button_layout.AddRow(None, self.DefaultButton, self.AbortButton, None)
        return button_layout
        
    # Returns the selected index
    @property
    def SelectedIndex(self):
        return self.m_selected_index

################################################################################
# TestSampleEtoListBoxDialog function
################################################################################
def TestSampleEtoListBoxDialog():
    
    # Create and initialize collection
    collection = ObservableCollection[Fruit]()
    collection.Add(Fruit('Apple', 'Red'))
    collection.Add(Fruit('Banana', 'Yellow'))
    collection.Add(Fruit('Grape', 'Purple'))
    collection.Add(Fruit('Orange', 'Orange'))
    collection.Add(Fruit('Strawberry', 'Red'))
    
    # Create and display dialog
    dlg = SampleEtoListBoxDialog(collection)
    rc = dlg.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    if (rc):
        # Print results
        fruit = collection[dlg.SelectedIndex]
        print fruit.Name
        print fruit.Color

################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    TestSampleEtoListBoxDialog()