################################################################################
# SampleSynchronizeRenderColors.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import rhinoscriptsyntax as rs

# Synchronize Render Colors
def SampleSynchronizeRenderColors():
    
    objects = rs.AllObjects()
    if objects is None: return
    
    for obj in objects:
        
        color = rs.ObjectColor(obj)
        source = rs.ObjectColorSource(obj)
        material = -1
        
        if source == 0:
            layer = rs.ObjectLayer(obj)
            material = rs.LayerMaterialIndex(layer)
            if material < 0:
                material = rs.AddMaterialToLayer(layer)
        
        elif source == 1:
            material = rs.ObjectMaterialIndex(obj)
            if material < 0:
                material = rs.AddMaterialToObject(obj)
        
        if material >= 0:
            if color != rs.MaterialColor(material):
                rs.MaterialColor(material, color)
                
    rs.Redraw()
    
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleSynchronizeRenderColors() # Call the function defined above