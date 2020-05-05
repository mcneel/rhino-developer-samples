################################################################################
# SampleDraftAngleCurves.py
# Copyright (c) 2020 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext



def SampleDraftAngleCurves():

    sphere = Rhino.Geometry.Sphere(Rhino.Geometry.Point3d.Origin,10)
    brep = sphere.ToBrep()

    direction = Rhino.Geometry.Vector3d.ZAxis * -1

    min_radian = Rhino.RhinoMath.ToRadians(-15)
    max_radian = Rhino.RhinoMath.ToRadians(15)

    tolerance = scriptcontext.doc.ActiveDoc.ModelAbsoluteTolerance
    angle_tolerance = scriptcontext.doc.ActiveDoc.ModelAngleToleranceRadians

    min_draft_silhouettes = Rhino.Geometry.Silhouette.ComputeDraftCurve(brep,min_radian,direction,tolerance,angle_tolerance)

    for s in min_draft_silhouettes:
        scriptcontext.doc.ActiveDoc.Objects.Add(s.Curve)

    max_draft_silhouettes = Rhino.Geometry.Silhouette.ComputeDraftCurve(brep,max_radian,direction,tolerance,angle_tolerance)

    for s in max_draft_silhouettes:
         scriptcontext.doc.ActiveDoc.Objects.Add(s.Curve)

    scriptcontext.doc.ActiveDoc.Objects.AddSphere(sphere)
    scriptcontext.doc.ActiveDoc.Views.Redraw()

    return Rhino.Commands.Result.Success

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == '__main__' ):
    SampleDraftAngleCurves()