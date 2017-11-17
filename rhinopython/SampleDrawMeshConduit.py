################################################################################
# SampleDrawMeshConduit.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

import System
import Rhino
import scriptcontext as sc

################################################################################
# SampleDrawMeshConduit display conduit class
################################################################################
class SampleDrawMeshConduit(Rhino.Display.DisplayConduit):
    
    # Initializer
    def __init__(self):
        self.m_mesh = None
    
    # CalculateBoundingBox override
    def CalculateBoundingBox(self, e):
        if self.m_mesh:
            e.IncludeBoundingBox(self.m_mesh.GetBoundingBox(False))
    
    # PostDrawObjects override
    def PostDrawObjects(self, e):
        if self.m_mesh:
            material = Rhino.Display.DisplayMaterial()
            material.IsTwoSided = True
            material.Diffuse = System.Drawing.Color.Blue
            material.BackDiffuse = System.Drawing.Color.Red
            e.Display.EnableLighting(True)
            e.Display.DrawMeshShaded(self.m_mesh, material)
            e.Display.DrawMeshWires(self.m_mesh, System.Drawing.Color.Black)
    
    @property
    def Mesh(self):
        return self.m_mesh
    
    @Mesh.setter
    def Mesh(self, val):
        self.m_mesh = val
    
################################################################################
# TestSampleDrawMeshConduit function
################################################################################
def TestSampleDrawMeshConduit():
          
    rc, corners = Rhino.Input.RhinoGet.GetRectangle()
    if rc != Rhino.Commands.Result.Success:
        return rc
    
    plane = Rhino.Geometry.Plane(corners[0], corners[1], corners[2])
    x_interval = Rhino.Geometry.Interval(0, corners[0].DistanceTo(corners[1]))
    y_interval = Rhino.Geometry.Interval(0, corners[1].DistanceTo(corners[2]))
    mesh = Rhino.Geometry.Mesh.CreateFromPlane(plane, x_interval, y_interval, 10, 10)
    
    conduit = SampleDrawMeshConduit()
    conduit.Mesh = mesh
    conduit.Enabled = True
    sc.doc.Views.Redraw()
    
    out_str = ""
    rc = Rhino.Input.RhinoGet.GetString("Press <Enter> to continue", True, out_str);
    conduit.Enabled = False
    sc.doc.Views.Redraw()
    
################################################################################
# Check to see if this file is being executed as the 'main' python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == '__main__':
    TestSampleDrawMeshConduit()