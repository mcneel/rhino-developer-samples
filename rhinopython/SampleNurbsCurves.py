################################################################################
# SampleAddNurbsCurves.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext

def SampleAddNurbsCurves():
    
    # World 3-D, or Euclidean, locations
    pt0 = Rhino.Geometry.Point3d(-8.0, -3.0, 0.0)
    pt1 = Rhino.Geometry.Point3d(-4.0,  3.0, 2.0)
    pt2 = Rhino.Geometry.Point3d(4.0,  3.0, 2.0)
    pt3 = Rhino.Geometry.Point3d(8.0, -3.0, 0.0)

    # Create NURBS curve:
    #   Dimension = 3
    #   Rational = False
    #   Order (Degree + 1) = 4
    #   Control point count = 4
    #   Knot count = Control point count + degree - 1
    nc0 = Rhino.Geometry.NurbsCurve(3, False, 4, 4)
    # World 3-D, or Euclidean, control points,
    nc0.Points[0] = Rhino.Geometry.ControlPoint(pt0)
    nc0.Points[1] = Rhino.Geometry.ControlPoint(pt1)
    nc0.Points[2] = Rhino.Geometry.ControlPoint(pt2)
    nc0.Points[3] = Rhino.Geometry.ControlPoint(pt3)
    # Clamped knots
    nc0.Knots[0] = 0
    nc0.Knots[1] = 0
    nc0.Knots[2] = 0
    nc0.Knots[3] = 1
    nc0.Knots[4] = 1
    nc0.Knots[5] = 1
    
    
    # Create NURBS curve:
    #   Dimension = 3
    #   Rational = True
    #   Order (Degree + 1) = 4
    #   Control point count = 4
    #   Knot count = Control point count + degree - 1
    nc1 = Rhino.Geometry.NurbsCurve(3, True, 4, 4)
    # Control points from a world 3-D, or Euclidean, locations and a weight
    nc1.Points[0] = Rhino.Geometry.ControlPoint(pt0, 1.0)
    nc1.Points[1] = Rhino.Geometry.ControlPoint(pt1, 2.0)
    nc1.Points[2] = Rhino.Geometry.ControlPoint(pt2, 4.0)
    nc1.Points[3] = Rhino.Geometry.ControlPoint(pt3, 1.0)
    # Clamped knots
    nc1.Knots[0] = 0
    nc1.Knots[1] = 0
    nc1.Knots[2] = 0
    nc1.Knots[3] = 1
    nc1.Knots[4] = 1
    nc1.Knots[5] = 1
    
    
    # Create NURBS curve:
    #   Dimension = 3
    #   Rational = True
    #   Order (Degree + 1) = 4
    #   Control point count = 4
    #   Knot count = Control point count + degree - 1
    nc2 = Rhino.Geometry.NurbsCurve(3, True, 4, 4)
    # Homogeneous control points
    nc2.Points[0] = Rhino.Geometry.ControlPoint(-8.0, -3.0, 0.0, 1.0)
    nc2.Points[1] = Rhino.Geometry.ControlPoint(-4.0,  3.0, 2.0, 2.0)
    nc2.Points[2] = Rhino.Geometry.ControlPoint(4.0,  3.0, 2.0, 4.0)
    nc2.Points[3] = Rhino.Geometry.ControlPoint(8.0, -3.0, 0.0, 1.0)
    # Clamped knots
    nc2.Knots[0] = 0
    nc2.Knots[1] = 0
    nc2.Knots[2] = 0
    nc2.Knots[3] = 1
    nc2.Knots[4] = 1
    nc2.Knots[5] = 1    
    
    
    # Add to document
    scriptcontext.doc.Objects.Add(nc0)
    scriptcontext.doc.Objects.Add(nc1)
    scriptcontext.doc.Objects.Add(nc2)
    scriptcontext.doc.Views.Redraw()


# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleAddNurbsCurves()
