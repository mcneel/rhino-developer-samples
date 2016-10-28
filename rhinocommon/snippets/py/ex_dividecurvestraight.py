import rhinoscriptsyntax as rs
import Rhino
from Rhino.Input import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Geometry import *
from Rhino.Geometry.Intersect import *

def nextIntersectParamAndPoint(overlapCurves, intersectPoints, curve):
    intersectParamsAndPoints = [(curve.ClosestPoint(point)[1], point) 
                                for point in intersectPoints]
    for overlapCurve in overlapCurves:
        intersectParamsAndPoints.append(
          (overlapCurve.Domain.Min, overlapCurve.PointAt(overlapCurve.Domain.Min)))
        intersectParamsandPoints.append(
          (overlapCurve.Domain.Max, overlapCurve.PointAt(overlapCurve.Domain.max)))
    return min(intersectParamsAndPoints, key =  lambda t: t[0])
    
def divide_curve():
    # get user input
    res, obj_refs = RhinoGet.GetMultipleObjects("Curves to divide",
        False, ObjectType.EdgeFilter | ObjectType.Curve)
    if res <> Result.Success: return res
    curves = [obj_ref.Curve() for obj_ref in obj_refs]
    
    distance_between_divisions = rs.GetReal(
      message = "Distance between divisions",
      number = 5.0, minimum = 1.0)
    if distance_between_divisions == None: return
    
    # generate the points
    points = []
    for curve in curves:
      t0 = curve.Domain.Min
      points.append(curve.PointAt(t0))
      
      sphere_center = curve.PointAt(t0)
      t = t0
      rest_of_curve = curve
      while True:
        sphere = Sphere(sphere_center, distance_between_divisions)
        b, overlapCurves, intersectPoints = Intersection.CurveBrep(
                               rest_of_curve, sphere.ToBrep(), 0.0)
        if b == False or (overlapCurves.Length == 0 and intersectPoints.Length == 0): 
          break
        t, point = nextIntersectParamAndPoint(
          overlapCurves, intersectPoints, rest_of_curve)
        points.append(point)
        sphere_center = point
        rest_of_curve = curve.Split(t)[1]
          
    rs.AddPoints(points)
    rs.Redraw()
   
if __name__ == "__main__":
    divide_curve()