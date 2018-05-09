################################################################################
# SampleGetSpecificObjectGrip.py
# Copyright (c) 2018, Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import System
import Rhino
import rhinoscriptsyntax as rs

# If you want to limit grip object selection to a specific object,
# then you can use a custom GetObject class that behaves 
# similar to this one.
class GetSpecificObjectGrip(Rhino.Input.Custom.GetObject):
    
    def __init__(self, owner_id):
        self.m_owner_id = owner_id
        self.GeometryFilter = Rhino.DocObjects.ObjectType.Grip
        self.SubObjectSelect = False
    
    def CustomGeometryFilter(self, rhino_object, geometry, component_index):
        if isinstance(rhino_object, Rhino.DocObjects.GripObject):
            if self.m_owner_id == rhino_object.OwnerId:
                return True
        return False
    
# Tests the sample class (above)
def SampleGetSpecificObjectGrip():
    # Replace the id (below) with your own...
    object_id = System.Guid('782b7f20-8353-4066-be42-bd3f08791340')
    go = GetSpecificObjectGrip(object_id)
    go.SetCommandPrompt('Select grips')
    go.GetMultiple(1, 0)
    # TODO...
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleGetSpecificObjectGrip()