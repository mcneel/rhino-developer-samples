################################################################################
# SampleDrawExtrudedCurves.py
# Copyright (c) 2019 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import scriptcontext as sc
import Rhino
import System

# GetObject-derived class that limits object picking to planar curves
class GetPlanarCurve(Rhino.Input.Custom.GetObject):
    def CustomGeometryFilter(self, rh_object, geometry, component_index):
        if not rh_object: 
            return False
        if not geometry: 
            return False
        if not isinstance(geometry, Rhino.Geometry.Curve): 
            return False
        tol = rh_object.Document.ModelAbsoluteTolerance
        rc, plane = geometry.TryGetPlane(tol)
        return rc

# DisplayConduit-derived class that draws Breps
class DrawBrepConduit(Rhino.Display.DisplayConduit):
    def __init__(self, breps):
        self.breps = breps
        
    def CalculateBoundingBox(self, e):
        for brep in self.breps:
            e.IncludeBoundingBox(brep.GetBoundingBox(False))
            
    def PreDrawObjects(self, e):
        att = e.Display.DisplayPipelineAttributes
        if att.ShadingEnabled:
            material = Rhino.Display.DisplayMaterial(System.Drawing.Color.DarkGray)
            for brep in self.breps:
                e.Display.DrawBrepShaded(brep, material)
        for brep in self.breps:
            e.Display.DrawBrepWires(brep, System.Drawing.Color.Black)

# Sample function that select planar curve, extrudes them in some fixed
# direction, and then draws them in a custom display conduit.
def SampleDrawExtrudedCurves():
    go = GetPlanarCurve()
    go.SetCommandPrompt("Select planar curves")
    go.SubObjectSelect = False
    go.GetMultiple(1, 0)
    if go.CommandResult() != Rhino.Commands.Result.Success:
        return
    
    tol = sc.doc.ModelAbsoluteTolerance
    breps = []
    
    for objref in go.Objects():
        crv = objref.Curve();
        rc, plane = crv.TryGetPlane(tol)
        dir = plane.ZAxis * 3
        srf = Rhino.Geometry.Surface.CreateExtrusion(crv, dir)
        brep = srf.ToBrep()
        brep.Faces.SplitKinkyFaces(Rhino.RhinoMath.DefaultAngleTolerance, True)
        if crv.IsClosed:
            solid = brep.CapPlanarHoles(tol)
            if Rhino.Geometry.BrepSolidOrientation.Inward == solid.SolidOrientation:
                solid.Flip();
            breps.append(solid)
        else:
            breps.append(brep)
            
        objref.Object().Select(False)
    
    conduit = DrawBrepConduit(breps)
    conduit.Enabled = True
    sc.doc.Views.Redraw()

    rs.GetString('Press <Enter> to continue')

    conduit.Enabled = False
    sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleDrawExtrudedCurves()
