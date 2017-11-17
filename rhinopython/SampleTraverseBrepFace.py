################################################################################
# SampleTraverseBrepFace.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino

# Demonstrates how to traverse a Brep face
def SampleTraverseBrepFace(brep, fi):
    
    # Valiate Brep
    if not brep:
        Rhino.RhinoApp.WriteLine("ERROR: Brep is null")
        return
    
    # Validate face index
    if fi < 0 or fi >= brep.Faces.Count:
        Rhino.RhinoApp.WriteLine("ERROR: Invalid face index")
        return
    
    Rhino.RhinoApp.WriteLine("Traversing Brep face {0}", fi)
    
    # Get Brep face
    face = brep.Faces[fi]
    
    # Each face has an underlying untrimmed surface
    si = face.SurfaceIndex
    if si < 0 or si >= brep.Surfaces.Count:
        Rhino.RhinoApp.WriteLine("ERROR: invalid Brep.Faces[{0}].SurfaceIndex", fi)
    else:
        surface = brep.Surfaces[si]
        if not surface:
          Rhino.RhinoApp.WriteLine("ERROR: invalid Brep.Surfaces[{0}] is null", si);
    
    # The face is trimmed with one or more trimming loops. All the 2d trimming 
    # curves are oriented so that the active region of the trimmed surface lies
    # to the left of the 2d trimming curve.  
    #
    # If face.OrientationIsReversed is true, the orientations of the face in
    # the Brep is opposited the natural parameteric orientation of the surface.

    # Number of trimming loops on this face (>=1)
    loop_count = face.Loops.Count
    
    li = 0
    while li < loop_count:
        
        # Get face loop
        loop = face.Loops[li]
        
        # Number of trimming edges in this loop
        loop_trim_count = loop.Trims.Count
        
        lti = 0
        while lti < loop_trim_count:
            
            # Get loop trim
            trim = loop.Trims[lti];
          
            # 2d trimming information
            
            # Each trim has a 2d parameter space curve
            c2i = trim.TrimCurveIndex;
            if c2i < 0 or c2i >= brep.Curves2D.Count:
                Rhino.RhinoApp.WriteLine("ERROR: invalid Brep.Trims[{0}].TrimCurveIndex", c2i)
            else:
                crv2d = brep.Curves2D[c2i]
                if not crv2d:
                  Rhino.RhinoApp.WriteLine("ERROR: invalid Brep.Curves2D[{0}] is null", c2i);
                  return
                   
            # Topology and 3d geometry information
            
            # Trim starts at vertex0 and ends at vertex1. When the trim is a loop
            # or on a singular surface side, vertex0 and vertex1 will be equal.
            #
            # vertex0 = trim.StartVertex
            # vertex1 = trim.EndVertex
            #
            # The vertexX.EdgeIndices array contains the brep.Edges indices of
            # the edges that begin or end at vertexX.
            
            edge = trim.Edge
            
            # If None == crv2d, then this trim lies on a portion of a singular 
            # surface side. The vertex indices are still valid and will be equal.
            
            if edge:
                
                # If trim.IsReversed() is false, the orientations of the 3d edge
                # and the 3d curve obtained by composing the surface and 2d curve 
                # agree.
                # 
                # If trim.IsReversed() is true, the orientations of the 3d edge
                # and the 3d curve obtained by composing the surface and 2d curve
                # are opposite.
                
                c3i = edge.EdgeCurveIndex
                if c3i < 0 or c3i >= brep.Curves3D.Count:
                  Rhino.RhinoApp.WriteLine("ERROR: invalid brep.Edges[{0}].EdgeCurveIndex", c3i)
                else:
                  crv3d = brep.Edges[c3i]
                  if not crv3d:
                    Rhino.RhinoApp.WriteLine("ERROR: invalid brep.Curves3D[{0}] is null", c3i)
                
                # The edge.TrimIndices array contains the brep.Trims indices
                # for the other trims that are joined to this edge.
            
            lti += 1 # Next trim
            
        # Next loop
        li += 1

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    filter = Rhino.DocObjects.ObjectType.Brep
    rc, objref = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface", False, filter)
    if objref and rc == Rhino.Commands.Result.Success: 
        brep = objref.Brep()
        if brep:
            fi = 0
            while fi < brep.Faces.Count:
                SampleTraverseBrepFace(brep, fi)
                fi += 1
