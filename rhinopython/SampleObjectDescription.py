################################################################################
# SampleObjectDescription.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

import Rhino
import rhinoscriptsyntax as rs
import scriptcontext as sc

################################################################################
# SampleObjectDescription function
################################################################################
def SampleObjectDescription(obj_id):
    if obj_id:
        obj = rs.coercerhinoobject(obj_id)
        if obj:
            log = Rhino.FileIO.TextLog()
            log.Print(obj.ShortDescription(False))
            obj.Description(log)
            Rhino.UI.Dialogs.ShowTextDialog(log.ToString(), 'Object Description')
    
################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    obj_id = rs.GetObject()
    if obj_id:
        SampleObjectDescription(obj_id)