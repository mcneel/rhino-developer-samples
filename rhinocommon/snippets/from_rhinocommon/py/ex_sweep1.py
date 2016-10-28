import rhinoscriptsyntax as rs
import Rhino
import scriptcontext

def Sweep1():
    rail = rs.GetObject("Select rail curve", rs.filter.curve)
    rail_crv = rs.coercecurve(rail)
    if not rail_crv: return

    cross_sections = rs.GetObjects("Select cross section curves", rs.filter.curve)
    if not cross_sections: return
    cross_sections = [rs.coercecurve(crv) for crv in cross_sections]

    sweep = Rhino.Geometry.SweepOneRail()
    sweep.AngleToleranceRadians = scriptcontext.doc.ModelAngleToleranceRadians
    sweep.ClosedSweep = False
    sweep.SweepTolerance = scriptcontext.doc.ModelAbsoluteTolerance
    sweep.SetToRoadlikeTop()
    breps = sweep.PerformSweep(rail_crv, cross_sections)
    for brep in breps: scriptcontext.doc.Objects.AddBrep(brep)
    scriptcontext.doc.Views.Redraw()

if __name__ == "__main__":
    Sweep1()
