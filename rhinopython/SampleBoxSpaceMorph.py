################################################################################
# SampleBoxSpaceMorph.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax as rs
import scriptcontext

 # Rhino.Geometry.SpaceMorph-inherited class
class BoxMorph(Rhino.Geometry.SpaceMorph):
    
    def __init__(self, box, corners):
        if 8 == len(box): self.box = box[:]
        if 8 == len(corners): self.corners = corners[:]
        if 8 == len(self.box):
            self.ref_x = Rhino.Geometry.Line(self.box[0], self.box[1])
            self.ref_y = Rhino.Geometry.Line(self.box[0], self.box[3])
            self.ref_z = Rhino.Geometry.Line(self.box[0], self.box[4])
            
    def IsValid(self):
        if 8 == len(self.box) and 8 == len(self.corners): return True
        return False
        
    def MorphPoint(self, point):
        if not self.IsValid(): return Rhino.Geometry.Point3d.Unset
        u = self.ref_x.ClosestParameter(point)
        v = self.ref_y.ClosestParameter(point)
        w = self.ref_z.ClosestParameter(point)
        ab = self.corners[0] + (self.corners[1] - self.corners[0]) * u
        dc = self.corners[3] + (self.corners[2] - self.corners[3]) * u
        ef = self.corners[4] + (self.corners[5] - self.corners[4]) * u
        hg = self.corners[7] + (self.corners[6] - self.corners[7]) * u
        abdc = ab + (dc - ab) * v
        efhg = ef + (hg - ef) * v
        return abdc + (efhg - abdc) * w
        
def SampleBoxSpaceMorph():
    obj_id = rs.GetObject("Select surface or polysurface", rs.filter.surface and rs.filter.polysurface)
    if None == obj_id: return
    
    brep = rs.coercebrep(obj_id)
    if brep is None: return
    
    box = rs.BoundingBox(obj_id)
    corners = box[:]
    corners[6] = box[6] * 2
    
    morph = BoxMorph(box, corners)
    if morph.Morph(brep):
        scriptcontext.doc.Objects.AddBrep(brep)
        scriptcontext.doc.Views.Redraw()
        
        
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == "__main__" ):
    SampleBoxSpaceMorph()

