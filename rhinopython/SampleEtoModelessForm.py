################################################################################
# SampleEtoModelessForm.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

import System
import Rhino
import Rhino.UI
import Eto.Drawing as drawing
import Eto.Forms as forms
import scriptcontext as sc

################################################################################
# SampleEtoModelessForm class
################################################################################
class SampleEtoModelessForm(forms.Form):
    
    # Initializer
    def __init__(self):
        self.m_selecting = False
        # Basic form initialization
        self.Initialize()
        # Create the form's controls
        self.CreateFormControls()
        # Fill the form's listbox
        self.FillListBox()
        # Create Rhino event handlers
        self.CreateEvents()
    
    # Basic form initialization
    def Initialize(self):
        self.Title = 'Sample Modeless Form'
        self.Padding = drawing.Padding(5)
        self.Resizable = True
        self.Maximizable = False
        self.Minimizable = False
        self.ShowInTaskbar = False
        self.MinimumSize = drawing.Size(200, 150)
        # FormClosed event handler
        self.Closed += self.OnFormClosed
    
    # Adds a listitem to the listbox
    def AddObject(self, obj):
        item = forms.ListItem()
        item.Text = obj.ShortDescription(False)
        if obj.Name:
            item.Text += " - " + obj.Name
        item.Tag = obj.Id
        self.m_listbox.Items.Add(item)
        
    # Fills the listbox with document objects
    def FillListBox(self):
        iter = Rhino.DocObjects.ObjectEnumeratorSettings()
        iter.NormalObjects = True
        iter.LockedObjects = False
        iter.IncludeLights = True
        iter.IncludeGrips = False
        objects = sc.doc.Objects.GetObjectList(iter)
        for obj in objects:
            self.AddObject(obj)
     
    # CloseDocument event handler
    def OnCloseDocument(self, sender, e):
        self.m_listbox.Items.Clear()
        
    # NewDocument event handler
    def OnNewDocument(self, sender, e):
        self.FillListBox()
        
    # EndOpenDocument event handler
    def OnEndOpenDocument(self, sender, e):
        self.FillListBox()
        
    # OnAddRhinoObject event handler
    def OnAddRhinoObject(self, sender, e):
        self.AddObject(e.TheObject)
    
    # OnDeleteRhinoObject event handler
    def OnDeleteRhinoObject(self, sender, e):
        for item in self.m_listbox.Items:
            if item.Tag == e.ObjectId:
                self.m_listbox.Items.Remove(item)
                break
    
    # OnSelectObjects event handler
    def OnSelectObjects(self, sender, e):
        if self.m_selecting == True:
            return
        if e.RhinoObjects.Length == 1:
            i = 0
            for item in self.m_listbox.Items:
                if item.Tag == e.RhinoObjects[0].Id:
                    self.m_listbox.SelectedIndex = i
                    break
                else:
                    i += 1
        else:
            self.m_listbox.SelectedIndex = -1
    
    # OnDeselectAllObjects event handler
    def OnDeselectAllObjects(self, sender, e):
        if self.m_selecting == True:
            return
        self.m_listbox.SelectedIndex = -1
    
    # Create Rhino event handlers
    def CreateEvents(self):
        Rhino.RhinoDoc.CloseDocument += self.OnCloseDocument
        Rhino.RhinoDoc.NewDocument += self.OnNewDocument
        Rhino.RhinoDoc.EndOpenDocument += self.OnEndOpenDocument;
        Rhino.RhinoDoc.AddRhinoObject += self.OnAddRhinoObject;
        Rhino.RhinoDoc.DeleteRhinoObject += self.OnDeleteRhinoObject;
        Rhino.RhinoDoc.SelectObjects += self.OnSelectObjects
        Rhino.RhinoDoc.DeselectAllObjects += self.OnDeselectAllObjects;
        
    # Remove Rhino event handlers
    def RemoveEvents(self):        
        Rhino.RhinoDoc.CloseDocument -= self.OnCloseDocument
        Rhino.RhinoDoc.NewDocument -= self.OnNewDocument
        Rhino.RhinoDoc.EndOpenDocument -= self.OnEndOpenDocument;
        Rhino.RhinoDoc.AddRhinoObject -= self.OnAddRhinoObject;
        Rhino.RhinoDoc.DeleteRhinoObject -= self.OnDeleteRhinoObject;
        Rhino.RhinoDoc.SelectObjects -= self.OnSelectObjects
        Rhino.RhinoDoc.DeselectAllObjects -= self.OnDeselectAllObjects;
        
    # Create all of the controls used by the form
    def CreateFormControls(self):
        # Create table layout
        layout = forms.TableLayout()
        layout.Padding = drawing.Padding(10)
        layout.Spacing = drawing.Size(5, 5)
        # Add controls to layout
        layout.Rows.Add(forms.Label(Text = 'Rhino Objects:'))
        layout.Rows.Add(self.CreateListBoxRow())
        layout.Rows.Add(self.CreateButtonRow())
        # Set the content
        self.Content = layout
    
    # Listbox.SelectedIndexChanged event handler
    def OnSelectedIndexChanged(self, sender, e):
        index = self.m_listbox.SelectedIndex
        if index >= 0:
            self.m_selecting = True
            item = self.m_listbox.Items[index]
            Rhino.RhinoApp.RunScript("_SelNone", False)
            Rhino.RhinoApp.RunScript("_SelId " + item.Tag.ToString() + " _Enter", False)
            self.m_selecting = False
    
    # Called by CreateFormControls to creates the
    # table row that contains the listbox
    def CreateListBoxRow(self):
        # Create the listbox
        self.m_listbox = forms.ListBox()
        self.m_listbox.Size = drawing.Size(200, 100)
        self.m_listbox.SelectedIndexChanged += self.OnSelectedIndexChanged
        # Create the table row
        table_row = forms.TableRow()
        table_row.ScaleHeight = True
        table_row.Cells.Add(self.m_listbox)
        return table_row
    
    # 'Select' button click handler
    def OnSelectClick(self, sender, e):
        self.m_selecting = True
        self.m_listbox.SelectedIndex = -1
        Rhino.RhinoApp.RunScript("_SelAll", False)
        self.m_selecting = False
    
    # 'Clear' button click handler
    def OnClearClick(self, sender, e):
        self.m_selecting = True
        self.m_listbox.SelectedIndex = -1
        Rhino.RhinoApp.RunScript("_SelNone", False)
        self.m_selecting = False
    
    # Called by CreateFormControls to creates the
    # table row that contains the button controls.
    def CreateButtonRow(self):
        # Select button
        select_button = forms.Button(Text = 'Select All')
        select_button.Click += self.OnSelectClick
        # Clear button
        clear_button = forms.Button(Text = 'Clear')
        clear_button.Click += self.OnClearClick
        # Create layout
        layout = forms.TableLayout(Spacing = drawing.Size(5, 5))
        layout.Rows.Add(forms.TableRow(None, select_button, clear_button, None))
        return layout
    
    # Form Closed event handler
    def OnFormClosed(self, sender, e):
        # Remove the events added in the initializer
        self.RemoveEvents()
        # Dispose of the form and remove it from the sticky dictionary
        if sc.sticky.has_key('sample_modeless_form'):
            form = sc.sticky['sample_modeless_form']
            if form:
                form.Dispose()
                form = None
            sc.sticky.Remove('sample_modeless_form')
    
################################################################################
# TestSampleEtoModelessForm function
################################################################################
def TestSampleEtoModelessForm():
    
    # See if the form is already visible
    if sc.sticky.has_key('sample_modeless_form'):
        return
    
    # Create and show form
    form = SampleEtoModelessForm()
    form.Owner = Rhino.UI.RhinoEtoApp.MainWindow
    form.Show()
    # Add the form to the sticky dictionary so it
    # survives when the main function ends.
    sc.sticky['sample_modeless_form'] = form
    
################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    TestSampleEtoModelessForm()