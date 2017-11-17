################################################################################
# SampleRationalNurbsSurface.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext
import System

def SampleRationalNurbsSurface():
    
    # The following defines a NURBS spherical surface
    
    plane = Rhino.Geometry.Plane.WorldXY
    radius = 5.0
    
    x = radius * plane.XAxis
    y = radius * plane.YAxis
    z = radius * plane.ZAxis
    
    p = [Rhino.Geometry.Point3d for i in range(9)]
    p[0] = Rhino.Geometry.Point3d(plane.Origin + x)
    p[1] = Rhino.Geometry.Point3d(plane.Origin + x + y)
    p[2] = Rhino.Geometry.Point3d(plane.Origin + y)
    p[3] = Rhino.Geometry.Point3d(plane.Origin - x + y)
    p[4] = Rhino.Geometry.Point3d(plane.Origin - x)
    p[5] = Rhino.Geometry.Point3d(plane.Origin - x - y)
    p[6] = Rhino.Geometry.Point3d(plane.Origin - y)
    p[7] = Rhino.Geometry.Point3d(plane.Origin + x - y)
    p[8] = Rhino.Geometry.Point3d(plane.Origin + x)
    
    w = 1.0 / System.Math.Sqrt(2.0)
    w13 = w
    
    sp = Rhino.Geometry.Point4d(plane.Origin - z)
    np = Rhino.Geometry.Point4d(plane.Origin + z)
    
    cv = [Rhino.Geometry.Point4d for i in range(45)]
    for i in range(0, 8):
        cv[5*i  ] = Rhino.Geometry.Point4d(sp)
        cv[5*i+1] = Rhino.Geometry.Point4d(p[i] - z)
        cv[5*i+2] = Rhino.Geometry.Point4d(p[i])
        cv[5*i+3] = Rhino.Geometry.Point4d(p[i] + z)
        cv[5*i+4] = Rhino.Geometry.Point4d(np)
        
        if (i%2):
            cv[5*i  ].X *= w
            cv[5*i  ].Y *= w
            cv[5*i  ].Z *= w
            cv[5*i  ].W  = w
            cv[5*i+2].X *= w
            cv[5*i+2].Y *= w
            cv[5*i+2].Z *= w
            cv[5*i+2].W  = w
            cv[5*i+4].X *= w
            cv[5*i+4].Y *= w
            cv[5*i+4].Z *= w
            cv[5*i+4].W  = w
            w13 = 0.5
        else:
            w13 = w
            
        cv[5*i+1].X *= w13
        cv[5*i+1].Y *= w13
        cv[5*i+1].Z *= w13
        cv[5*i+1].W  = w13
        cv[5*i+3].X *= w13
        cv[5*i+3].Y *= w13
        cv[5*i+3].Z *= w13
        cv[5*i+3].W  = w13
        
    cv[40] = cv[0]
    cv[41] = cv[1]
    cv[42] = cv[2]
    cv[43] = cv[3]
    cv[44] = cv[4]
    
	# Construct a new NURBS surface with internal uninitialized arrays
    s = Rhino.Geometry.NurbsSurface.Create(3, True, 3, 3, 9, 5)
    
    s.KnotsU[0] = 0.0
    s.KnotsU[1] = 0.0
    s.KnotsU[2] = 0.5 * System.Math.PI
    s.KnotsU[3] = 0.5 * System.Math.PI
    s.KnotsU[4] = System.Math.PI
    s.KnotsU[5] = System.Math.PI
    s.KnotsU[6] = 1.5 * System.Math.PI
    s.KnotsU[7] = 1.5 * System.Math.PI
    s.KnotsU[8] = 2.0 * System.Math.PI
    s.KnotsU[9] = 2.0 * System.Math.PI

    s.KnotsV[0] = -0.5 * System.Math.PI
    s.KnotsV[1] = -0.5 * System.Math.PI
    s.KnotsV[2] = 0.0
    s.KnotsV[3] = 0.0
    s.KnotsV[4] = 0.5 * System.Math.PI
    s.KnotsV[5] = 0.5 * System.Math.PI
    
    count = 0
    for i in range(0, 9):
        for j in range(0, 5):
            s.Points.SetPoint(i, j, cv[count])
            count += 1
    
    scriptcontext.doc.Objects.Add(s)
    scriptcontext.doc.Views.Redraw()


# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleRationalNurbsSurface()            
