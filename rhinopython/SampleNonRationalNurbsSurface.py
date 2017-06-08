################################################################################
# SampleNonRationalNurbsSurface.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext

def SampleNonRationalNurbsSurface():
    
    dim = 3
    rational = False
    u_degree = 2
    v_degree = 3
    u_count = 3
    v_count = 5
    
    # Construct a new NURBS surface with internal uninitialized arrays
    s = Rhino.Geometry.NurbsSurface.Create(dim, rational, u_degree+1, v_degree+1, u_count, v_count)
    
    # Quadratic knot vector with no interior knots
    s.KnotsU[0] = 0.0
    s.KnotsU[1] = 0.0
    s.KnotsU[2] = 1.0
    s.KnotsU[3] = 1.0
    
    # Cubic knot vector with one simple interior knot
    s.KnotsV[0] = 0.0
    s.KnotsV[1] = 0.0
    s.KnotsV[2] = 0.0
    s.KnotsV[3] = 1.5
    s.KnotsV[4] = 2.0
    s.KnotsV[5] = 2.0
    s.KnotsV[6] = 2.0
    
    # Rational control points can be in either homogeneous
    # or Euclidean form. Non-rational control points do not
    # need to specify a weight.  
    for u in range(0, u_count):
        for v in range(0, v_count):
            pt = Rhino.Geometry.Point3d(u, v, u-v)
            s.Points.SetPoint(u, v, pt)
            
    scriptcontext.doc.Objects.Add(s)
    scriptcontext.doc.Views.Redraw()


# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleNonRationalNurbsSurface()