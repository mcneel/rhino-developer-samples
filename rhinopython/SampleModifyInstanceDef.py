################################################################################
# SampleModifyInstanceDef.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Demonstates how to modify an instance definition's objects
def SampleModifyInstanceDef():
    
    # Get the name of the instance definition to modify
    idef_name = ''
    rc, idef_name = Rhino.Input.RhinoGet.GetString("Name of block to modify", False, idef_name)
    if rc != Rhino.Commands.Result.Success: 
        return
    
    # Find the instance definition
    idef = sc.doc.InstanceDefinitions.Find(idef_name)
    if idef is None:
        print 'Instance definition not found'
        return
    
    # Get the instance definition's objects
    objects = idef.GetObjects()
    
    # Separate the object into geometry and attribute lists
    geometry = []
    attributes = []
    for i, obj in enumerate(objects):
        geometry.append(obj.Geometry)
        attributes.append(obj.Attributes)
    
    # Create a new text dot
    origin = Rhino.Geometry.Point3d.Origin
    dot = Rhino.Geometry.TextDot(idef.Name, origin)
    
    # Append the text dot and default attributes to the lists
    geometry.append(dot)
    attributes.append(sc.doc.CreateDefaultAttributes())
    
    # Modify the instance definition and redraw
    sc.doc.InstanceDefinitions.ModifyGeometry(idef.Index, geometry, attributes)
    sc.doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleModifyInstanceDef()