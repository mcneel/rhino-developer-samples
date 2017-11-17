################################################################################
# SampleShowToolbarGroup.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import System

# Demonstrates how to programatically show a toolbar group.
def SampleShowToolbarGroup():
    
    # Get loaded toolbar collections (files)
    tb_files = Rhino.RhinoApp.ToolbarFiles
    
    # See if 'Default' toolbar collection is loaded
    tb = tb_files.FindByName('Default', True)
    
    if not tb:
        # Get path to user profile
        path = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)
        # Try loading 'Default" toolbar collection
        tb = tb_files.Open(path + '\McNeel\Rhinoceros\5.0\UI\Default.rui')
    
    if not tb:
        # Something wrong...
        RhinoApp.WriteLine('Unable to open Default.rui')
        return
    
    # Find the 'Polygon' toolbar group
    grp = tb.GetGroup('Polygon');
    if grp:
        # Show toolbar group
        grp.Visible = True
        
        # Pause for user input
        str = ''
        rc, str = Rhino.Input.RhinoGet.GetString('Press <Enter> to continue', True, str)
        
        # Hide toolbar group
        grp.Visible = False
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleShowToolbarGroup()